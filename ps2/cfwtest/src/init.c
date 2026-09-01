
#include "init.h"
#include <debug.h>
#include <iopcontrol.h>
#include <kernel.h>
#include <libcdvd-common.h>
#include <loadfile.h>
#include <ps2sdkapi.h>
#include <sbv_patches.h>
#include <sifrpc.h>
#include <stdio.h>
#include <string.h>

// Macros for loading embedded IOP modules
#define IRX_DEFINE(mod)                                                                                                                              \
  extern unsigned char mod##_irx[] __attribute__((aligned(16)));                                                                                     \
  extern uint32_t size_##mod##_irx

// Defines moduleList entry for embedded and external modules
#define INT_MODULE(mod) {#mod, NULL, mod##_irx, &size_##mod##_irx}

// Embedded IOP modules
IRX_DEFINE(iomanX);
IRX_DEFINE(fileXio);
IRX_DEFINE(ps2dev9);
IRX_DEFINE(smap_udptty);
IRX_DEFINE(ps2atad);
IRX_DEFINE(ps2hdd_osd);
IRX_DEFINE(dvrdrv);
IRX_DEFINE(dvrmisc);

typedef struct ModuleListEntry {
  char *name;         // Module name
  char *path;         // Module path for external modules
  unsigned char *irx; // Pointer to IRX module
  uint32_t *size;     // IRX size
} ModuleListEntry;

// List of modules to load
static ModuleListEntry moduleList[] = {
    INT_MODULE(iomanX),      //
    INT_MODULE(fileXio),     //
    INT_MODULE(ps2dev9),     //
    INT_MODULE(smap_udptty), //
    INT_MODULE(ps2atad),     //
    INT_MODULE(ps2hdd_osd),  //
    INT_MODULE(dvrdrv),      //
    INT_MODULE(dvrmisc),     //
};
#define MODULE_COUNT sizeof(moduleList) / sizeof(ModuleListEntry)

int custom_sceCdNoticeGameStart(int mode, u32 *result);
int custom_sceCdXDVRPReset(uint8_t arg, u32 *result);

// Initializes IOP modules
int initModules() {
  int ret = 0;
  int iopret = 0;

  // Initialize the RPC manager and reboot the IOP with OSDSYS modules
  scr_printf("switching PSX into PSX mode");
  sceSifInitRpc(0);
  while (!SifIopReset("rom0:UDNL rom0:OSDCNF", 0)) {
  };
  while (!SifIopSync()) {
  };
  sceSifInitRpc(0);

  // Execute the "notice game start" SCMD to disable the PS2 mode
  sceCdInit(SCECdINoD);
  u32 stat;
  int result;
  do {
    result = custom_sceCdNoticeGameStart(0, &stat);
  } while ((result == 0) || (stat & 0x80));

  scr_printf(", resetting DVRP\n");
  result = custom_sceCdXDVRPReset(0, &stat);
  sceCdInit(SCECdEXIT);

  // Reboot the IOP
  while (!SifIopReset("", 0)) {
  };
  while (!SifIopSync()) {
  };

  // Initialize the RPC manager
  sceSifInitRpc(0);

  // Apply patches required to load modules from EE RAM
  if ((ret = sbv_patch_enable_lmb()))
    return ret;
  if ((ret = sbv_patch_disable_prefix_check()))
    return ret;
  if ((ret = sbv_patch_fileio()))
    return ret;

  // Load modules
  for (int i = 0; i < MODULE_COUNT; i++) {
    ret = 0;
    iopret = 0;

    if (!strcmp(moduleList[i].name, "smap_udptty")) {
      char ip[] = "ip=192.168.0.10";
      ret = SifExecModuleBuffer(moduleList[i].irx, *moduleList[i].size, sizeof(ip), ip, &iopret);
      scr_printf("\tWaiting for udptty:");
      sleep(5);
      scr_printf(" ready\n");
      printf("Ready\n");
    } else
      ret = SifExecModuleBuffer(moduleList[i].irx, *moduleList[i].size, 0, NULL, &iopret);

    if (ret >= 0)
      ret = 0;
    if (iopret == 1)
      ret = iopret;

    if (ret) {
      scr_printf("\tFailed to initialize module %s: %d\n", moduleList[i].name, ret);
      return ret;
    }
  }
  return 0;
}

// PSX-only SCMDs
#define CD_SCMD_NOTICE_GAME_START 0x2F

// libcdvd externs
extern int bindSCmd;
extern SifRpcClientData_t clientSCmd;
extern int sCmdSemaId;
extern u8 sCmdRecvBuff[];
extern u8 sCmdSendBuff[];
extern int sCmdNum;
extern int _CdCheckSCmd(int cmd);

// Enables/disables the "Quit Game" button
int custom_sceCdNoticeGameStart(int mode, u32 *result) {
  int status;

  if (_CdCheckSCmd(CD_SCMD_NOTICE_GAME_START) == 0)
    return 0;

  *(u32 *)sCmdSendBuff = mode;
  if (SifCallRpc(&clientSCmd, CD_SCMD_NOTICE_GAME_START, 0, sCmdSendBuff, 4, sCmdRecvBuff, 8, NULL, NULL) >= 0) {
    *result = *(u32 *)UNCACHED_SEG(&sCmdRecvBuff[4]);
    status = *(int *)UNCACHED_SEG(sCmdRecvBuff);
  } else {
    status = 0;
  }

  SignalSema(sCmdSemaId);
  return status;
}

int custom_sceCdXDVRPReset(uint8_t arg, u32 *result) {
  int res;
  u8 out[16];
  u8 in[16];

  if (result) {
    *result = 0;
  }
  {
    in[0] = arg;
    res = sceCdApplySCmd(0x33u, in, 1, out);
    if (result) {
      *result = (u8)out[0];
    }
  }
  return res;
}
