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
sceAtaExecCmdLBA48_131 = 0x0;
sceAtaExecCmdLBA48_211 = 0x10007c4c;
/* ATA emulator SPEED command queue reader location */
ataemuReadSpeedQueue = 0x1000d946;
/* MISC devctl locations */
miscDevctlCheckCommand = 0x10015952;
miscDevctlDefaultHandler = 0x1001592a;
devctlSendResponse = 0x10019338;

/* Custom HDD ID response location */
HDDID_SRC        = 0x1006a400;
/* Global wrapper state location */
CMD_STATE        = 0x1006a600;
/* Command pointer location */
ATAEMU_CMD       = 0x1006a604;
/* ATA emulator SPEED command queue location */
ATAEMU_CMD_QUEUE = 0x10070074;
/* atad device info location */
ATAD_DEVICE_INFO = 0x10073fc0;
/* MISC devctl buffer location */
MISC_DEVCTL_BUF = 0x100d6720;

SECTIONS
{
  . = LOADADDR;
  .text : { *(.text*) *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
}
