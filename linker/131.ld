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
sceAtaExecCmdLBA48_131 = 0x100069b0;
sceAtaExecCmdLBA48_211 = 0x0;
/* ATA emulator SPEED command queue reader location */
ataemuReadSpeedQueue = 0x1000ade4;
/* MISC devctl locations */
miscDevctlCheckCommand = 0x10011394;
miscDevctlDefaultHandler = 0x1001136c;
devctlSendResponse = 0x100149c2;

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

SECTIONS
{
  . = LOADADDR;
  .text : { *(.text*) *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
}
