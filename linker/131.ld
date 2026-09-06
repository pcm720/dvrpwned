/* Generic linker script for DVRP FR30 payloads.
 * LOADADDR is substituted at build time with the desired virtual address. */

OUTPUT_FORMAT("elf32-fr30")

/* Absolute symbols for firmware routines referenced by name from C code. */
sceAtaExecCmd    = 0x100067fc;
sceAtaWaitResult = 0x10006e4c;
sceAtaGetSceId   = 0x1000719e;
setupDMA         = 0x10014f7e;
memcpy           = 0x10015a0a;
memset           = 0x100159d0;
sprintf          = 0x1002e0b0;
strlen           = 0x1002e194;
sceAtaExecCmdLBA48_131 = 0x100069b0;
sceAtaExecCmdLBA48_211 = 0x0;
/* ATA emulator SPEED command queue reader location */
ataemuReadSpeedQueue = 0x1000ade4;
/* MISC devctl locations */
miscDevctlCheckCommand = 0x10011394;
miscDevctlDefaultHandler = 0x1001136c;
/* DVR devctl locations */
dvrDevctlDefaultHandler = 0x1000f74c;
/* Common devctl functions */
devctlSendResponse = 0x100149c2;
/* PFS functions */
sceOpen     = 0x1001b854;
sceWrite    = 0x1001ba58;
sceClose    = 0x1001baae;
sceLseek    = 0x1001b900;
sceLseek64  = 0x1001b964;
sceDopen    = 0x1001bbde;
sceMkdir    = 0x1001bd42;
sceDclose   = 0x1001bcbc;
sceRemove   = 0x1001bcc8;
sceRename   = 0x1001bfc2;
sceGetstat  = 0x1001be08;
sceSync     = 0x1001c0fa;
sceMount    = 0x1001c10e;
sceDevctl   = 0x1001c264;
sceRead     = 0x1001ba02;
sceDread    = 0x1001bc70;
sceIoctl    = 0x1001bb10;
sceIoctl2   = 0x1001bb5e;
sceChdir    = 0x1001BFE6;
sceChstat   = 0x1001BE1C;
sceFormat   = 0x1001BE30;
sceReadlink = 0x1001C318;
sceRmdir    = 0x1001BD54;
sceSymlink  = 0x1001BFD4;
sceUmount   = 0x1001C1C2;

/* Custom HDD ID response location */
HDDID_SRC        = 0x10033900;
/* Global wrapper state location */
CMD_STATE        = 0x10033b00;
/* Command pointer location */
ATAEMU_CMD       = 0x10033b04;
/* ATA emulator SPEED command queue location */
ATAEMU_CMD_QUEUE = 0x10040080;
/* atad device info location */
ATAD_DEVICE_INFO = 0x10042ab4;
/* MISC devctl buffer location */
MISC_DEVCTL_BUF = 0x100a4ff4;
/* DVR devctl buffer location */
DVR_DEVCTL_BUF = 0x10121008;

SECTIONS
{
  . = LOADADDR;
  .text : { *(.text*) *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
}
