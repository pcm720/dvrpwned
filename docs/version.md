# Version patch

DVRPwned modifies the original firmware version to reflect the actual CFW version.  
Applications can detect DVRPwned firmware by checking the firmware version reported through the standard dvrmisc version devctl call.

The firmware version is stored as a 16-bit value at the following addresses:

- **1.31 firmware**: Address 0x10031ef0, original value 0x0209
- **2.11 firmware**: Address 0x1005f61c, original value 0x0206

The patch is implemented as a raw byte replacement in the firmware binary.

## Version format

For patched value of 0x0520:

- High byte (0x05): DVRPwned CFW
- Middle nibble (0x2): DVRPwned major version number
- Low nibble (0x0): DVRPwned minor version number
