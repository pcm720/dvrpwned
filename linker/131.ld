/* Generic linker script for DVRP FR30 payloads.
 * LOADADDR is substituted at build time with the desired virtual address. */

OUTPUT_FORMAT("elf32-fr30")

/* Absolute symbols for firmware routines referenced by name from C code. */
sceAtaExecCmd    = 0x100067fc;
sceAtaWaitResult = 0x10006e4c;
sceAtaGetSceId   = 0x1000719e;
memcpy           = 0x10015a0a;
memset           = 0x100159d0;

/* Custom HDD ID response location */
HDDID_SRC        = 0x10033900;
/* Global wrapper state location */
CMD_STATE        = 0x10033B00;

SECTIONS
{
  . = LOADADDR;
  .text : { *(.text*) *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
}
