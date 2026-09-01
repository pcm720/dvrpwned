# Devctl extensions

DVRPwned extends MISC devctls with custom commands using the 0x513x command range.  
The extension handler replaces the default case handler in the MISC devctl 0x51xx command handler, routing 0x513x commands to the custom handler.

The extensions are accessed through the standard devctl interface by calling the MISC task handler with commands in the 0x513x range.  
Each command supports the standard fileXio devctl protocol with request and response data buffers.

## Available commands

### Ping (DVRP command 0x5130, PS2SDK devctl 0x5690)

Returns immediate acknowledgment to verify custom firmware presence.

- No parameters required
- Returns success status if CFW is active

#### PS2 usage[^1]

```c
// Simple ping to verify CFW presence
int res = fileXioDevctl("dvr_misc:", 0x5690, NULL, 0, NULL, 0);
```

### Total number of sectors (DVRP command 0x5131, PS2SDK devctl 0x5691)

Returns the full 64-bit total sector count for specified device.

- **Parameters**:
  - `uint16 device` (0 or 1)
- **Response**:
  - `int16 status`
  - `uint64 totalSectors` [byte order: `low16, mid16, high16, upper16`]

#### PS2 usage[^1]

```c
// Query LBA48 sector count for device 0
uint16_t reqBuf[1] = {0};  // device 0
uint16_t respBuf[5];       // response buffer
int res = fileXioDevctl("dvr_misc:", 0x5691, reqBuf, 2, respBuf, 10);
if (res == 0) {
    // Parse 64-bit LBA from response
    uint64_t sectors = ((u64)respBuf[3] << 48) | ((u64)respBuf[2] << 32) |
                       ((u64)respBuf[1] << 16) | (u64)respBuf[0];
}
```

### Memory operations (DVRP command 0x5132, PS2SDK devctl 0x5692)

Provides direct memory access capabilities for debugging and testing.

- **Parameters**:
  - `uint16 union`
    - `uint8 type` (`0 - peek, 1 - poke`)
    - `uint8 size` (`8, 16, 32`)
  - `uint32 address` [byte order: `low16, high16`]
  - `uint32 value` [byte order: `low16, high16`] (poke only)
  - `uint32 mask` [byte order: `low16, high16`] (poke only)
- **Response**:
  - `int16 status`
  - `uint32 old_value` [byte order: `low16, high16`]
  - `uint32 current_value` [byte order: `low16, high16`]

#### PS2 usage[^1]

```c
// 16-bit memory peek example
uint16_t reqBuf[3] = {0x0010, 0x1ef0, 0x1003};  // peek, 16-bit, address 0x10031ef0
uint16_t respBuf[5];
int res = fileXioDevctl("dvr_misc:", 0x5692, reqBuf, 6, respBuf, 10);

// 16-bit memory poke example with mask
uint16_t reqBuf[7] = {0x0110, 0x1ef0, 0x1003, 0xdead, 0x0000, 0xffff, 0x0000};
// poke, 16-bit, address 0x10031ef0, value 0xdead, mask 0xffff
int res = fileXioDevctl("dvr_misc:", 0x5692, reqBuf, 14, respBuf, 10);
```

---

[^1]: See the [cfwtest application](../ps2/cfwtest/src/dvrp_cfw.c) for complete implementation example.
