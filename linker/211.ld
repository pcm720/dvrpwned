/* Generic linker script for DVRP FR30 payloads.
 * LOADADDR is substituted at build time with the desired virtual address. */

OUTPUT_FORMAT("elf32-fr30")

/* Absolute symbols for firmware routines referenced by name from C code. */
sceAtaExecCmd    = 0x10007a98;
sceAtaWaitResult = 0x100080c0;
sceAtaGetSceId   = 0x10008412;
setupDMA         = 0x10019910;
memcpy           = 0x1001a434;
memset           = 0x1001a3fa;
sprintf          = 0x10059e2a;
strlen           = 0x10059f0e;
sceAtaExecCmdLBA48_131 = 0x0;
sceAtaExecCmdLBA48_211 = 0x10007c4c;
/* ATA emulator SPEED command queue reader location */
ataemuReadSpeedQueue = 0x1000d946;
/* MISC devctl locations */
miscDevctlCheckCommand = 0x10015952;
miscDevctlDefaultHandler = 0x1001592a;
/* DVR devctl locations */
dvrDevctlDefaultHandler = 0x10013698;
/* Common devctl functions */
devctlSendResponse = 0x10019338;
/* PFS functions */
sceOpen     = 0x1002027e;
sceWrite    = 0x10020482;
sceClose    = 0x100204d8;
sceLseek    = 0x1002032a;
sceLseek64  = 0x1002038e;
sceDopen    = 0x10020608;
sceMkdir    = 0x1002076c;
sceDclose   = 0x100206e6;
sceRemove   = 0x100206f2;
sceRename   = 0x100209ec;
sceGetstat  = 0x10020832;
sceSync     = 0x10020b24;
sceMount    = 0x10020b38;
sceDevctl   = 0x10020c8e;
sceRead     = 0x1002042c;
sceDread    = 0x1002069a;
sceIoctl    = 0x1002053a;
sceIoctl2   = 0x10020588;
sceChdir    = 0x10020a10;
sceChstat   = 0x10020846;
sceFormat   = 0x1002085a;
sceReadlink = 0x10020d42;
sceRmdir    = 0x1002077e;
sceSymlink  = 0x100209fe;
sceUmount   = 0x10020bec;

/* Custom HDD ID response location */
HDDID_SRC        = 0x1006a400;
/* Global wrapper state location */
CMD_STATE        = 0x1006a600;
/* Command pointer location */
ATAEMU_CMD       = 0x1006a604;
/* ATA emulator SPEED command queue location */
ATAEMU_CMD_QUEUE = 0x10070074;
/* atad device info location */
ATAD_DEVICE_INFO = 0x10073fb8;
/* MISC devctl buffer location */
MISC_DEVCTL_BUF = 0x100d6720;
/* DVR devctl buffer location */
DVR_DEVCTL_BUF = 0x10152840;

SECTIONS
{
  . = LOADADDR;
  .text : { *(.text*) *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
}
