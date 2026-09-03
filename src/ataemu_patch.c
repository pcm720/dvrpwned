/*
 * DVR ATAEMU task patches
 * Adds LBA48 support and security response emulation to ATA emulation routines
 */
#include "ata_fw_refs.h"
#include "common_fw_refs.h"
#include "sce_security_patch.h"

// Replaces the original command parser
// 1.31 command parser supports only LBA28 commands
// While the 2.11 parser seems to support LBA48 commands, the implementation is buggy
// and relies on the correct number of writes and specific command order to work correctly
int ataemuParseCommand(cmd_param_data_t *data) {
  data->feature = 0;
  data->nsector = 0;
  data->sector = 0;
  data->lcyl = 0;
  data->hcyl = 0;
  data->select = 0;
  data->command = 0;
  data->ext->feature = 0;
  data->ext->nsector = 0;
  data->ext->sector = 0;
  data->ext->lcyl = 0;
  data->ext->hcyl = 0;
  data->ext->select = 0;
  data->ext->command = 0;

  ATAEMU_CMD = data;

  // The command read from SPEED is essentially a FIFO-queue of
  // <2-byte command tag><2-byte value> entries.
  // Each ATA hwport register has its own tag
  // Commands for device 0 start with 0x2??
  // Commands for device 1 start with 0x3??
  int cmdLength = ataemuReadSpeedQueue(ATAEMU_CMD_QUEUE); // Returns the total number of entries
  if (!cmdLength)
    return -1;

  // Check the last command tag (always the command register value) to determine the device
  switch (ATAEMU_CMD_QUEUE[(cmdLength - 1) * 2]) {
  case 0x274:
    data->device = 0;
    break;
  case 0x374:
    data->device = 1;
    break;
  default:
    return -1;
  }

  int bufIdx = 0;
  unsigned short tag = 0;
  unsigned short *valPtr = 0;
  unsigned short *extValPtr = 0;
  while (cmdLength > 0) {
    // Ignore commands for the other device
    tag = (ATAEMU_CMD_QUEUE[bufIdx] >> 8) & 0xF;
    if (((data->device == 1) && (tag == 0x2)) || ((data->device == 0) && (tag == 0x3)))
      goto next;

    tag = ATAEMU_CMD_QUEUE[bufIdx] & 0xff;
    switch (tag) {
    case 0x58:
      valPtr = &(data->feature);
      extValPtr = &(data->ext->feature);
      break;
    case 0x5c:
      valPtr = &(data->nsector);
      extValPtr = &(data->ext->nsector);
      break;
    case 0x60:
      valPtr = &(data->sector);
      extValPtr = &(data->ext->sector);
      break;
    case 0x64:
      valPtr = &(data->lcyl);
      extValPtr = &(data->ext->lcyl);
      break;
    case 0x68:
      valPtr = &(data->hcyl);
      extValPtr = &(data->ext->hcyl);
      break;
    case 0x6c:
      valPtr = &(data->select);
      extValPtr = &(data->ext->select);
      break;
    case 0x74:
      valPtr = &(data->command);
      extValPtr = &(data->ext->command);
      break;
    default:
      // Unsupported tag
      goto next;
    }

    if (*valPtr != 0x0) // Move LBA48 value (upper 8 bits) for the field to the extended parameter
      *extValPtr = *valPtr;

    *valPtr = ATAEMU_CMD_QUEUE[bufIdx + 1] & 0xff;

  next:
    cmdLength -= 1;
    if (cmdLength == 0)
      break;

    bufIdx += 2;
  }
  return cmdLength;
}

// Intercepts ATAEmu's sceAtaExecCmd() call to respond with the custom HDD ID
int sceAtaExecCmdLBA48Wrapper(void *buf, int blkcount, int feature, short nsector, short sector, short lcyl, short hcyl, unsigned short select,
                              short cmd) {

  if (ATAEMU_CMD == 0x0)
    return ATA_RES_ERR_CMD;

  int res = 0;
  if (sceAtaExecCmdLBA48_131 != 0x0) {
    cmd_lba48_param_t lba48ext = {
        .feature = ATAEMU_CMD->ext->feature,
        .nsector = ATAEMU_CMD->ext->nsector,
        .sector = ATAEMU_CMD->ext->sector,
        .lcyl = ATAEMU_CMD->ext->lcyl,
        .hcyl = ATAEMU_CMD->ext->hcyl,
        .select = 0x0,
        .command = 0x0,
    };
    cmd_lba48_param_t params = {
        .feature = feature,
        .nsector = nsector,
        .sector = sector,
        .lcyl = lcyl,
        .hcyl = hcyl,
        .select = select,
        .command = cmd,
    };
    res = sceAtaExecCmdLBA48_131(buf, blkcount, &lba48ext, &params);
  } else if (sceAtaExecCmdLBA48_211 != 0x0)
    res = sceAtaExecCmdLBA48_211(buf, blkcount, (cmd_param_data_t *)ATAEMU_CMD);

  ATAEMU_CMD = 0x0;
  return res;
}

// Intercepts sceAtaExecCmd() calls in ATAEmu to respond with the custom HDD ID for ATA_SCE_IDENTIFY_DRIVE command
// and reroutes LBA48 commands to the LBA48 handler
int sceAtaExecCmdWrapper(void *buf, int blkcount, int feature, short nsector, short sector, short lcyl, short hcyl, unsigned short select,
                         short cmd) {
  if (cmd == 0x8e && feature == 0xec) {
    CMD_STATE = 1;
    memcpy(buf, HDDID_SRC, 0x200);
    return 0;
  }

  switch (cmd) {
  case 0x24:
  case 0x25:
  case 0x26:
  case 0x27:
  case 0x29:
  case 0x2f:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x39:
  case 0x3f:
  case 0x42:
  case 0xea:
  case 0xef:
    // Fix LBA48 block count if 16-bit nsector value is 0
    if ((blkcount == 0) && ((unsigned int)((ATAEMU_CMD->ext->nsector << 8) | (ATAEMU_CMD->nsector)) == 0))
      blkcount = 0x10000;
    return sceAtaExecCmdLBA48Wrapper(buf, blkcount, feature, nsector, sector, lcyl, hcyl, select, cmd);
  }

  if (blkcount == 0)
    blkcount = 0x100;
  return sceAtaExecCmd(buf, blkcount, feature, nsector, sector, lcyl, hcyl, select, cmd);
}

// Intercepts ATAEmu's sceAtaWaitResult() call to immediately return
// if the sceAtaExecCmd interceptor was triggered before the call
int sceAtaWaitResultWrapper(void) {
  if (CMD_STATE != 0) {
    CMD_STATE = 0;
    return 0;
  }

  return sceAtaWaitResult();
}

void ataemuPatchIdentifyResponse(int device, unsigned short *buf) {
  unsigned int ps2AreaSectors = (&ATAD_DEVICE_INFO)[device].total_sectors;
  // Byte-swap total sectors
  unsigned short ps2AreaSectorsLo = (unsigned short)((ps2AreaSectors & 0xffff) >> 0x8) | (unsigned short)((ps2AreaSectors & 0xffff) << 0x8);
  unsigned short ps2AreaSectorsHi = (unsigned short)(ps2AreaSectors >> 0x18) | (unsigned short)((ps2AreaSectors >> 0x10) << 0x8);

  // The commented patches, while making the response correct,
  // break XMB 2.xx because it expects an LBA48-capable drive
  // that reports the full drive capacity in the LBA48 sector count
  //
  // Patch IDENTIFY response
  // if (ps2AreaSectors <= 0xFFFFFFF) {
  //   // Set total LBA28 sectors
  //   buf[ATA_ID_SECTOTAL_LO] = ps2AreaSectorsLo;
  //   buf[ATA_ID_SECTOTAL_HI] = ps2AreaSectorsHi;
  //   // Clear LBA48 total sectors
  //   buf[ATA_ID_48BIT_SECTOTAL_LO] = 0x0;
  //   buf[ATA_ID_48BIT_SECTOTAL_MI] = 0x0;
  //   buf[ATA_ID_48BIT_SECTOTAL_HI] = 0x0;
  //   buf[ATA_ID_48BIT_SECTOTAL_UI] = 0x0;
  //   // Clear bit 10 of word 83 to remove the LBA48 flag
  //   buf[ATA_ID_COMMAND_SETS_SUPPORTED] = buf[ATA_ID_COMMAND_SETS_SUPPORTED] & 0xFFFB;
  // } else {
  //   // Set max LBA28
  //   buf[ATA_ID_SECTOTAL_LO] = 0xffff;
  //   buf[ATA_ID_SECTOTAL_HI] = 0xff0f;
  //   // Overwrite LBA48 sectors to reflect the PS2 area size
  //   buf[ATA_ID_48BIT_SECTOTAL_LO] = ps2AreaSectorsLo;
  //   buf[ATA_ID_48BIT_SECTOTAL_MI] = ps2AreaSectorsHi;
  //   buf[ATA_ID_48BIT_SECTOTAL_HI] = 0x0;
  //   buf[ATA_ID_48BIT_SECTOTAL_UI] = 0x0;
  // }

  // Set total LBA28 sectors to the PS2 area size
  buf[ATA_ID_SECTOTAL_LO] = ps2AreaSectorsLo;
  buf[ATA_ID_SECTOTAL_HI] = ps2AreaSectorsHi;
  // Inject "PS2LBA48" ASCII into reserved words 121-124 (as "SPL2AB84")
  // to indicate that this firmware supports LBA48 for PS2 area
  buf[ATA_ID_DVRPCFW_ID_LO] = 0x5644;
  buf[ATA_ID_DVRPCFW_ID_MI] = 0x4c52;
  buf[ATA_ID_DVRPCFW_ID_HI] = 0x4142;
  buf[ATA_ID_DVRPCFW_ID_UI] = 0x3834;
}

// Fixes command direction and sector count to be LBA48-aware
void setupDMAWrapper(unsigned int type, unsigned int dir, void *buf, unsigned int sectorCount) {
  if (!ATAEMU_CMD) {
    setupDMA(type, dir, buf, sectorCount);
    return;
  }

  unsigned int maxSectorCount = 0x10000;

  switch (ATAEMU_CMD->command) {
    // Read commands
  case 0x25:
    dir = 0x0;
    maxSectorCount = 0x10000;
    break;
  case 0xc8:
    dir = 0x0;
    maxSectorCount = 0x100;
    break;
    // Write commands
  case 0x35:
    dir = 0x1;
    maxSectorCount = 0x10000;
  case 0xca:
    dir = 0x1;
    maxSectorCount = 0x100;
    break;
  }

  // Fix the sector count when 0
  sectorCount = (unsigned int)((ATAEMU_CMD->ext->nsector << 8) | (ATAEMU_CMD->nsector));
  if (sectorCount == 0)
    sectorCount = maxSectorCount;

  setupDMA(type, dir, buf, (sectorCount << 9));
}
