#include "common.h"
#include "dvrp_cfw.h"
#include "init.h"
#include "lba48.h"
#include <debug.h>
#include <inttypes.h>
#include <kernel.h>
#include <libsecr.h>
#include <ps2sdkapi.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define NEWLIB_PORT_AWARE
#include <fileXio_rpc.h>
#include <fileio.h>
#include <hdd-ioctl.h>
#include <io_common.h>

#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif

char nullHDDID[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int main(int argc, char *argv[]) {
  init_scr();
  scr_clear();
  scr_setCursor(0);
  scr_printf("\n\nDVRPwned Test Application\n");

  uint8_t sceSec[512] = {0};
  int res = 0;
  scr_printf("\tInitializing modules: ");
  if (initModules() != 0) {
    scr_printf("\n\tERROR: Failed to initialize modules\n");
    goto fail;
  }

  res = fileXioDevctl("hdd0:", HDIOC_SCEIDENTIFY, NULL, 0, &sceSec, sizeof(sceSec));
  if (res != 0) {
    logString("\n\tNon-official SCE drive: %d\n", res);
    goto fail;
  } else {
    logString("\n\tOfficial SCE drive");
    if (!memcmp(nullHDDID, sceSec, sizeof(nullHDDID)))
      logString(" with nullkey HDD ID\n");
    else
      logString("\n");
  }

  uint32_t totalSectors;
  totalSectors = fileXioDevctl("hdd0:", HDIOC_TOTALSECTOR, NULL, 0, NULL, 0);
  logString("\tTotal LBA: 0x%lx\n", totalSectors);

  logString("\n\nTesting LBA48 support:\n");
  testLBA48(0x10020000);
  testLBA48(0x12000000);
  sleep(10);

  //
  // Test custom devctls
  //

  scr_clear();
  logString("\n\nDevctl test:\n");

  // Ping command
  dvrp_cfw_ping();

  // Max LBA48 for device 0
  uint64_t maxlba48 = 0;
  dvrp_cfw_get_max_lba48(0, &maxlba48);

  // Max LBA48 for device 1
  maxlba48 = 0;
  dvrp_cfw_get_max_lba48(1, &maxlba48);

  // Max LBA48 for device 2
  maxlba48 = 0;
  dvrp_cfw_get_max_lba48(2, &maxlba48);

  dvrp_cfw_mem_peek_poke_result_t memres = {0};
  dvrp_cfw_mem_poke_params_t memparam = {0};

  // Memory peek (8-bit)
  dvrp_cfw_mem_peek(0x1003216e, 8, &memres);

  // Memory poke (8-bit)
  memparam.width = 8;
  memparam.address = 0x1003216e;
  memparam.value = 0xaa;
  memparam.mask = 0xff;
  dvrp_cfw_mem_poke(&memparam, &memres);

  // Memory poke with mask (8-bit)
  memparam.width = 8;
  memparam.address = 0x1003216e;
  memparam.value = 0xbb;
  memparam.mask = 0x0f;
  dvrp_cfw_mem_poke(&memparam, &memres);

  // Memory peek (16-bit)
  dvrp_cfw_mem_peek(0x10031ef0, 16, &memres);

  // Memory poke (16-bit)
  memparam.width = 16;
  memparam.address = 0x10031ef0;
  memparam.value = 0xdead;
  memparam.mask = 0xffff;
  dvrp_cfw_mem_poke(&memparam, &memres);

  // Memory poke with mask (16-bit)
  memparam.value = 0xf2ff;
  memparam.mask = 0xff00;
  dvrp_cfw_mem_poke(&memparam, &memres);

  // Memory peek (32-bit)
  dvrp_cfw_mem_peek(0x10031ef4, 32, &memres);

  // Memory poke (32-bit)
  memparam.width = 32;
  memparam.address = 0x10031ef4;
  memparam.value = 0x2200f2ff;
  memparam.mask = 0xffffffff;
  dvrp_cfw_mem_poke(&memparam, &memres);

  // Memory poke with mask (32-bit)
  memparam.width = 32;
  memparam.address = 0x10031ef4;
  memparam.value = 0x1002859c;
  memparam.mask = 0xff00ffff;
  dvrp_cfw_mem_poke(&memparam, &memres);

  logString("\n\nDone\n");

fail:
  sleep(20);
  return 0;
}
