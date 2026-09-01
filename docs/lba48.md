# LBA48 support

The DVRP firmware implements a subset of ATA commands and lacks native LBA48 support.  
This document describes the patches added to enable full LBA48 functionality for PS2 HDDs larger than 137GB.

## LBA48 command parsing

The original 1.31 firmware command parser only supports LBA28 commands.  
While 2.11 includes a parser that appears to support LBA48, it contains bugs that require specific command ordering to function correctly.

The patched implementation processes the SPEED FIFO command queue which contains `<2-byte command tag><2-byte value>` entries.  
Each ATA hardware port register has its own tag (0x2xx for device 0, 0x3xx for device 1). The parser extracts LBA48 extended values by detecting non-zero values in the upper 8 bits and moving them to extended parameter structures.

## LBA48 command execution

The patched implementation intercepts `sceAtaExecCmd()` calls to:

1. Intercept the `ATA_SCE_IDENTIFY_DRIVE` command to respond with the valid HDD ID
2. Route LBA48 commands to the LBA48 handler
3. Maintain compatibility with existing LBA28 command processing

The DMA setup routine call in the original ATAEmu DMA handler is intercepted.  
The 2.11 firmware natively supports LBA48 DMA transfers, so this part is left untouched.

## IDENTIFY response

The firmware modifies IDENTIFY responses to properly indicate LBA48 capabilities:

- Injects "PS2LBA48" signature in reserved words 121-124 to indicate LBA48 support
- For LBA48-capable drives: reports max LBA28 value (0xfffffff) while storing actual PS2 area size in LBA48 fields
- For LBA28-only drives: uses standard `total_sectors` value from drive IDENTIFY response
- Disables bit 10 of word 83 when PS2 area size fits in LBA28 to prevent LBA48-aware PS2 applications from using LBA48 commands

## Command table

### [1.31](131_ataemu_table.md)

The following LBA48 commands are enabled through raw patches to the command table:

- 0x24 (READ SECTORS EXT)
- 0x25 (READ DMA EXT)
- 0x26 (READ DMA QUEUED EXT)
- 0x27 (READ NATIVE MAX ADDRESS EXT)
- 0x29 (READ MULTIPLE EXT)
- 0x2f (READ LOG EXT)
- 0x34 (WRITE SECTORS EXT)
- 0x35 (WRITE DMA EXT)
- 0x36 (WRITE DMA QUEUED EXT)
- 0x37 (SET MAX ADDRESS EXT)
- 0x39 (WRITE MULTIPLE EXT)
- 0x3f (WRITE LOG EXT)

### [2.11](211_ataemu_table.md)

All LBA48 commands are already enabled in the original firmware
