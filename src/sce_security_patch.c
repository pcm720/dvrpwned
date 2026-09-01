/*
 * Implements the DVRP security workaround to make the console accept non-original HDDs
 *
 * https://gist.github.com/uyjulian/aa88872d2ebb2b569242eb56c1331dd5
 */

#include "ata_fw_refs.h"
#include "common_fw_refs.h"
#include "sce_security_patch.h"
#include "utron_syscalls.h"

// Seeds the SPEED crypto engine and tricks it into unlocking the UDMA
// communication by feeding the HDD ID
int unlockSPEED(int device, void *data) {
  int res;

  // Copy the HDD ID response into the HDD init data buffer
  memcpy(data, HDDID_SRC, 0x200);

  wai_sem(3);

  // Send write memory command with HDD ID contents
  res = sceAtaExecCmd(data, 1, 0, 0, 0, 0, 0, (device << 4) & 0xffff, ATA_C_WRITE_BUFFER);
  if (res != 0) {
    goto out;
  }
  res = sceAtaWaitResult();
  if (res != 0) {
    goto out;
  }
  // Read back the data
  res = sceAtaExecCmd(data, 1, 0, 0, 0, 0, 0, (device << 4) & 0xffff, ATA_C_READ_BUFFER);
  if (res != 0) {
    goto out;
  }

  // Send ATA_C_SCE_SECURITY_CONTROL ATA_SCE_IDENTIFY_DRIVE to secondary device
  ATA_HWPORT->r_feature = ATA_SCE_IDENTIFY_DRIVE & 0xff;
  ATA_HWPORT->r_nsector = 0 & 0xff;
  ATA_HWPORT->r_sector = 0 & 0xff;
  ATA_HWPORT->r_lcyl = 0 & 0xff;
  ATA_HWPORT->r_hcyl = 0 & 0xff;
  ATA_HWPORT->r_select = (((((device & 1) ^ 1) << 4) & 0xffff) | ATA_SEL_LBA) & 0xff;
  ATA_HWPORT->r_command = ATA_C_SCE_SECURITY_CONTROL & 0xff;
  // Immediately switch to primary device
  ATA_HWPORT->r_select = (((((device & 1) ^ 0) << 4) & 0xffff) | ATA_SEL_LBA) & 0xff;
  res = sceAtaWaitResult();

out:
  sig_sem(3);
  return 0;
}

// Intercepts sceAtaGetSceId() to run unlockSPEED if HDD does not support the
// security command
int sceAtaGetSceIdWrapper(int device, void *data) {
  // Call the original function
  int res = sceAtaGetSceId(device, data);
  if (res == 0) {
    // Replace the custom HDD ID if sceAtaGetSceId succeeded
    memcpy(HDDID_SRC, data, 0x200);
    return 0;
  }
  // If failed, unlock the SPEED
  return unlockSPEED(device, data);
}
