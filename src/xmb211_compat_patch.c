/*
 * Patches DVRP 1.31 firmware to make recordings compatible with 2.11
 *
 * 1.31 writes result metadata and EPG data to `dvr_pfs0:`.
 * XMB 2.11 expects those files on `dvr_pfs1:`.
 *
 * The patch detects commands only supported by the 2.11 XMB and
 * patches the paths from "dvr_pfs0:" to "dvr_pfs1:"
 *
 */
#include "common_fw_refs.h"
#include "devctl_fw_refs.h"
#include "utron_syscalls.h"

// MISC devctl data buffer location
extern devctl_cmd_data_t DVR_DEVCTL_BUF;

// Addresses of the '0' in every `dvr_pfs0:` string used by the
// DVR and EPG writers in the 1.31 firmware.
static const unsigned int PFS0_PATH_ADDRS_131[] = {
    /* DVR result paths */
    0x1003204F,
    0x10031FFF,
    0x10031FEB,
    0x10031FDF,
    0x10031F8F,
    0x10031FB7,
    0x10031FCB,
    0x10031F83,
    /* EPG file paths */
    0x10031C87,
    0x10031CBB,
    0x10031CEB,
    0x10031D2B,
    0x10031D53,
    0x10031D7B,
    0x10031D9F,
    0x10031DCB,
    0x10031DF7,
    0x10031E1F,
    0x10031E47,
    0x10031E73,
    0x10031E9B,
    0x10031EBF,
    0x10031F33,
    0x10031ED7,
    0x10031EE7,
    0x10032307,
    0x10032317,
    0x10031C5B,
    0x10031C9F,
    0x10031CD3,
    0x10031D0B,
    0x10031F1B,
};

// Replaces the default DVR task devctl handler for 0x2??? commands
void dvrDevctlDefaultHandlerReplacement() {
  unsigned short cmd = DVR_DEVCTL_BUF.requestCommand;
  unsigned short type = (cmd >> 8) & 0xf;
  unsigned short ctl = DVR_DEVCTL_BUF.requestCommand & 0x3f;

  switch (ctl) {
  case 0x1b:
  case 0x1c:
  case 0x1d:
    // PSX2-specific devctl commands
    // Patch path strings to point to dvr_hdd1
    if (*(volatile unsigned char *)PFS0_PATH_ADDRS_131[0] != '1') {
      for (int i = 0; i < sizeof(PFS0_PATH_ADDRS_131) / sizeof(unsigned int); i++) {
        volatile unsigned char *p = (volatile unsigned char *)PFS0_PATH_ADDRS_131[i];
        if (*p == '0') {
          *p = '1';
        }
      }
    }
    break;
  }

  // Reimplement RECEIVE TIMER EVENT DMA (0x231b)
  // Seems to be already handled by the DVR task in 1.31
  // Might be related to 0x2310 present in 1.31 (it calls set_flag(8, 0x400))
  if (type == 3 && ctl == 0x1b) {
    set_flg(8, 0x8000);
    return;
  }

  DVR_DEVCTL_BUF.responseCommand = cmd;
  DVR_DEVCTL_BUF.responseLength = 0x1;
  DVR_DEVCTL_BUF.responseData[0] = -18;

  devctlSendResponse(&DVR_DEVCTL_BUF, DVRP_CMD_ACK);
  return;
}
