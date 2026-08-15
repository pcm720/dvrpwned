/* Generic linker script for DVRP FR30 payloads.
 * LOADADDR is substituted at build time with the desired virtual address. */

OUTPUT_FORMAT("elf32-fr30")

/* Absolute symbols for firmware routines referenced by name from C code. */
sceAtaExecCmd    = 0x10007a98;
sceAtaWaitResult = 0x100080c0;
sceAtaGetSceId   = 0x10008412;
memcpy           = 0x1001a434;
memset           = 0x1001a3fa;

/* Custom HDD ID response location */
HDDID_SRC        = 0x1006a400;
/* Global wrapper state location */
CMD_STATE        = 0x1006a600;

SECTIONS
{
  . = LOADADDR;
  .text : { *(.text*) *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
}
