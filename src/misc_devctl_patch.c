/*
 * CFW dvrmisc devctl extensions
 * Adds 0x513? commands to the MISC devctl handler
 */
#include "ata_fw_refs.h"
#include "common_fw_refs.h"
#include "devctl_fw_refs.h"

// Sets command buffer pointer to cmdBuf
// and checks the input param length.
// Always initializes the first byte of responseData to zero.
// Returns -1 on mismatch, 0 on success
extern int miscDevctlCheckCommand(unsigned short command, unsigned short expectedLength);

// Default devctl handler for the MISC task
extern void miscDevctlDefaultHandler(unsigned short command, short status);

// MISC devctl data buffer location
extern devctl_cmd_data_t MISC_DEVCTL_BUF;

// DvrdrvExecCmdAck command. Can be used to check for CFW
void miscDevctlCFWPing() {
  miscDevctlCheckCommand(MISC_DEVCTL_BUF.requestCommand, 0x0);
  devctlSendResponse(&MISC_DEVCTL_BUF, DVRP_CMD_ACK);
  return;
}

// DvrdrvExecCmdAck command. Returns total sector count for the connected device
// Arguments:
//   - uint16 device
// Returns:
//   - int16 status
//   - uint64 totalSectors [byte order: 10 32 54 76]
void miscDevctlCFWGetMaxLBA48() {
  int res = miscDevctlCheckCommand(MISC_DEVCTL_BUF.requestCommand, 0x1);
  if (!res) {
    unsigned short device = MISC_DEVCTL_BUF.requestData[0];
    if ((device < 0) || (device > 1)) {
      MISC_DEVCTL_BUF.responseData[0] = -22;
      goto end;
    }

    if ((&ATAD_DEVICE_INFO)[device].exists == 0) {
      MISC_DEVCTL_BUF.responseData[0] = -19;
      goto end;
    }

    unsigned int totalSectorsHi = (&ATAD_DEVICE_INFO)[device].lba48_total_sectors_hi;
    unsigned int totalSectorsLo = (&ATAD_DEVICE_INFO)[device].lba48_total_sectors_lo;

    if ((&ATAD_DEVICE_INFO)[device].lba48 == 0) {
        // DVRP firmware stores the PS2 area size in LBA28 sector count only when the drive supports LBA48
        // If the drive supports only LBA28, it will copy total sector count from the drive's IDENTIFY response
        // Use LBA28 value for LBA28-only drive
        totalSectorsHi = 0;
        totalSectorsLo = (&ATAD_DEVICE_INFO)[device].total_sectors;
    }

    // Store the 64-bit LBA48 sector count in the response data as four 16-bit values
    MISC_DEVCTL_BUF.responseData[1] = (unsigned short)(totalSectorsLo & 0xFFFF);         // bits 0-15
    MISC_DEVCTL_BUF.responseData[2] = (unsigned short)((totalSectorsLo >> 16) & 0xFFFF); // bits 16-31
    MISC_DEVCTL_BUF.responseData[3] = (unsigned short)(totalSectorsHi & 0xFFFF);         // bits 32-47
    MISC_DEVCTL_BUF.responseData[4] = (unsigned short)((totalSectorsHi >> 16) & 0xFFFF); // bits 48-63
    MISC_DEVCTL_BUF.responseLength = 0x5;
  }

end:
  devctlSendResponse(&MISC_DEVCTL_BUF, DVRP_CMD_ACK);
  return;
}

// DvrdrvExecCmdAck command. Peeks/pokes DVRP memory
// Arguments (in big-endian):
//   - uint16 union {
//      uint8 type (0 - peek, 1 - poke)
//      uint8 size (8, 16, 32)
//   }
//   - uint32 address [byte order: 10 32]
//   - uint32 value [byte order: 10 32]
//   - uint32 mask [byte order: 10 32]
// Value and mask are ignored for the peek operation
// Response
//   - int16 status
//   - uint32 old_value [byte order: 10 32]
//   - uint32 value [byte order: 10 32]
// Example
//   - Set 0x10031ef0 to 0x02ad with mask 0xff00
//     0110 1ef0 1003 02ad 0000 ff00 0000
void miscDevctlCFWMemPeekPoke() {
  miscDevctlCheckCommand(MISC_DEVCTL_BUF.requestCommand, 0x7);

  int isPoke = (MISC_DEVCTL_BUF.requestData[0] >> 8) & 0xFF;
  unsigned short size = MISC_DEVCTL_BUF.requestData[0] & 0xFF;
  unsigned int address = (MISC_DEVCTL_BUF.requestData[2] << 16) | MISC_DEVCTL_BUF.requestData[1];
  unsigned int value = (MISC_DEVCTL_BUF.requestData[4] << 16) | MISC_DEVCTL_BUF.requestData[3];
  unsigned int mask = (MISC_DEVCTL_BUF.requestData[6] << 16) | MISC_DEVCTL_BUF.requestData[5];

  unsigned int old_value = 0;
  unsigned int current_value = 0;

  switch (size) {
  case 8: {
    volatile unsigned char *ptr = (volatile unsigned char *)address;
    old_value = *ptr;
    if (isPoke) {
      unsigned char new_val = (old_value & ~mask) | (value & mask);
      *ptr = new_val;
    }
    current_value = *ptr;
    break;
  }
  case 16: {
    volatile unsigned short *ptr = (volatile unsigned short *)address;
    old_value = *ptr;
    if (isPoke) {
      unsigned short new_val = (old_value & ~mask) | (value & mask);
      *ptr = new_val;
    }
    current_value = *ptr;
    break;
  }
  case 32: {
    volatile unsigned int *ptr = (volatile unsigned int *)address;
    old_value = *ptr;
    if (isPoke) {
      unsigned int new_val = (old_value & ~mask) | (value & mask);
      *ptr = new_val;
    }
    current_value = *ptr;
    break;
  }
  default:
    MISC_DEVCTL_BUF.responseData[0] = -22; // Error indicator
    MISC_DEVCTL_BUF.responseLength = 0x1;
    goto end;
  }

  // Store response in little-endian format
  MISC_DEVCTL_BUF.responseData[1] = old_value & 0xFFFF;             // Low 16 bits of old_value
  MISC_DEVCTL_BUF.responseData[2] = (old_value >> 16) & 0xFFFF;     // High 16 bits of old_value
  MISC_DEVCTL_BUF.responseData[3] = current_value & 0xFFFF;         // Low 16 bits of current_value
  MISC_DEVCTL_BUF.responseData[4] = (current_value >> 16) & 0xFFFF; // High 16 bits of current_value
  MISC_DEVCTL_BUF.responseLength = 0x5;                             // status word + 4 words = 10 bytes

end:
  devctlSendResponse(&MISC_DEVCTL_BUF, DVRP_CMD_ACK);
  return;
}

void miscDevctlCFWExtension() {
  switch (MISC_DEVCTL_BUF.requestCommand & 0x3f) {
  case 0x30:
    miscDevctlCFWPing();
    break;
  case 0x31:
    miscDevctlCFWGetMaxLBA48();
    break;
  case 0x32:
    miscDevctlCFWMemPeekPoke();
    break;
  default:
    miscDevctlDefaultHandler(MISC_DEVCTL_BUF.requestCommand, -12);
  }

  return;
}
