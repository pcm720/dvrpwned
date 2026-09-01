# DVRPwned

The first ever working custom firmware for the PSX DESR DVRP.  
This custom firmware:

- bypasses the HDD ID security check that previously restricted PSX to only original Sony HDDs supporting proprietary commands or adapters that support the proprietary IDENTIFY command
- adds full LBA48 support for the PS2 area (up to 2TB)
- adds custom devctl commands

With the custom firmware, almost any HDD can be used with:

- **PSX1** (DESR-5000/5100/7000/7100) - firmware version 1.31
- **PSX2** (DESR-5500/5700/7500/7700) - firmware version 2.11

## Credits

Brought to you by:

- **uyjulian** - concept and bypass idea
- **Tunertom** - DVRP checksum fixer script, exploit development and RE support
- **pcm720** - patching ideas, exploit development, patching toolkit and final implementation
- **DiscoStarslayer** - [FR60 Ghidra plugin](https://github.com/desrdev/ghidra-fr60) and documentation
- **MonkeyBoyJoey** - extensive testing, validation and risking his PSX1 and PSX2 units
- **GhostTownUS-** - testing and moral support
- **PS2SDK developers** - ps2atad implementation and SPEED register documentation, DESR reverse engineering

### In Memoriam

> ~~One brave soul was lost in the fight for custom firmware.~~  
> ~~While in the trenches, MonkeyBoyJoey's DESR-5000 lost its life.~~  
> ~~Bricked to death. May it rest in pieces as a glorified PS2 Slim.~~

Nevermind, we fixed it two hours later.

## How it works

The bypass concept was originally described by @uyjulian in his [research gist](https://gist.github.com/uyjulian/aa88872d2ebb2b569242eb56c1331dd5)

This implementation uses a GCC cross-compiler to compile C code into FR30 assembly, compatible with the FR60 architecture that the MB91302A is based on, and patches the DVRP firmware update file (`.udm`).

The patch:

1. Injects a custom HDD ID file to provide a valid HDD ID response in case the drive does not support it
2. Intercepts the `sceAtaGetSceId` function to ensure it always succeeds
3. Provides custom implementations of `sceAtaExecCmd` and `sceAtaWaitResult` for the DVRP ATA emulation task to always return a valid HDD ID to the PS2 side

## Usage

1. Get the original DVRP_FIRMWARE.udm file for your PSX model and place it in the root of this repository as:
   - `DVRP_FIRMWARE_131.udm` — PSX1
   - `DVRP_FIRMWARE_211.udm` — PSX2
2. Build the `fr30-elf` toolchain in `tools/gcc-fr30-elf/` by running:

   ```sh
   ./tools/build/build-toolchain.sh
   ```

3. Patch the firmware from a manifest:

   ```sh
   python3 build.py manifests/131.json
   ```

   The output is written to `build/dvrpwned_131.udm` with correct checksums.

4. Use the [DVRP flasher](https://github.com/pcm720/psx-dvrp-flasher) to flash the firmware

### Available manifests

- `manifests/131.json` - DVRPwned for DVRP firmware version 1.31 (DESR-5000/5100/7000/7100)
- `manifests/211.json` - DVRPwned for DVRP firmware version 2.11 (DESR-5500/5700/7500/7700)

## Project Structure

```
.
├── src/                    # Payload sources (.c/.S)
├── include/                # C headers with firmware function references
├── manifests/              # JSON patch manifests for different firmware versions
├── linker/                 # Linker scripts for different firmware versions
├── tools/
│   ├── scripts/            # Patching toolkit internals
│   │   ├── build-toolchain.sh
│   │   ├── build_payload.py
│   │   ├── patcher.py
│   │   └── fix_checksum.py
│   └── gcc-fr30-elf/       # Cross toolchain (built via the build-toolchain.sh script)
├── build/                  # Build artifacts and output .udm files
├── docs/                   # Technical documentation
│   ├── 131_ataemu_table.md           # DVRP ATAEmu command table for firmware 1.31
│   ├── 211_ataemu_table.md           # DVRP ATAEmu command table for firmware 2.11
│   ├── devctl_extensions.md          # DVRPwned devctl extensions details
│   ├── dvrp_dvr_speed_workaround.md  # HDD authentication workaround details
│   ├── hm91301CM71-10114-3E.pdf      # MB91302A hardware manual
│   ├── lba48.md                      # DVRPwned LBA48 implementation details
│   └── version.md                    # DVRPwned version patch details
└── build.py                # Main build script
```

## Disclaimer

This is an experimental firmware modification. Use at your own risk.
