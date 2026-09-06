# XMB 2.11 compatibility patch for PSX1

While adding LBA48 makes it possible to boot into XMB 2.11 on PSX1, the recordings do not persist.  
This patch fixes this by redirecting file paths used by the DVRP firmware.

## The issue

DVRP firmware 1.31 writes recording metadata and EPG data to `dvr_pfs0:` (`dvr_hdd0:__xcontents`), while XMB 2.11 expects these files on `dvr_pfs1:` (`dvr_hdd0:__xdata`).  
This mismatch causes recordings made with patched 1.31 firmware to be invisible in the XMB interface.

## Implementation

The patch detects DVR devctl commands that are only supported by the 2.11 XMB (`0x211b`, `0x211c`, and `0x211d`) and patches the filesystem paths at runtime. The devctls themselves do not seem to be required to work.

The patch contains a static array of memory addresses pointing to the character '0' in every `dvr_pfs0:` string used by the DVR and EPG writers in the 1.31 firmware. When a compatible XMB command is detected, each of these addresses is checked and modified from '0' to '1', changing `dvr_pfs0:` to `dvr_pfs1:`.

The patch affects the following files:
- Recording result metadata files (`.RES` files)
- EPG data files (`vbidata.dat`, `filelist.dat`)
