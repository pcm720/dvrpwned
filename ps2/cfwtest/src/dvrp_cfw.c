#include "dvrp_cfw.h"
#include "common.h"
#include <ps2sdkapi.h>
#include <stdint.h>
#include <string.h>
#define NEWLIB_PORT_AWARE
#include <fileXio_rpc.h>

// Can be used to test if the DVRP firmware supports custom devctls
// Returns 0 on success, negative value on error
int dvrp_cfw_ping() {
  int res = fileXioDevctl("dvr_misc:", DVRMISC_DEVCTL_CFW_PING, NULL, 0, NULL, 0);
  logString("DVRMISC_DEVCTL_CFW_PING: cmd res %d\n", res);
  return res;
}

// Sets the result to the total sector count for the device
// Returns 0 on success, negative value on error
int dvrp_cfw_get_max_lba48(uint16_t device, uint64_t *result) {
  if (!result)
    return -EINVAL;
  *result = 0;

  uint16_t reqBuf[20] = {0};
  uint16_t respBuf[20] = {0};

  reqBuf[0] = device;
  int res = fileXioDevctl("dvr_misc:", DVRMISC_DEVCTL_CFW_GET_MAX_LBA48, reqBuf, 2, respBuf, 10);
  if (res == 0) {
    // Parse the 64-bit LBA value from the response
    *result = ((u64)respBuf[3] << 48) | ((u64)respBuf[2] << 32) | ((u64)respBuf[1] << 16) | (u64)respBuf[0];
  }

  logString("DVRMISC_DEVCTL_CFW_GET_MAX_LBA48 for device %d: cmd res %d, result: 0x%llx\n", device, res, *result);
  return -1;
}

// Performs a memory peek (read) operation
// Returns 0 on success, negative value on error
int dvrp_cfw_mem_peek(uint32_t address, uint8_t width, dvrp_cfw_mem_peek_poke_result_t *result) {
  if (!result)
    return -EINVAL;

  uint16_t reqBuf[20] = {0};
  uint16_t respBuf[20] = {0};

  // Set operation flags (peek = 0)
  switch (width) {
  case 8:
    reqBuf[0] = 0x0008; // peek (0), 8-bit (8)
    break;
  case 16:
    reqBuf[0] = 0x0010; // peek (0), 16-bit (16)
    break;
  case 32:
    reqBuf[0] = 0x0020; // peek (0), 32-bit (20)
    break;
  default:
    return -EINVAL; // Unsupported width
  }

  // Set address (split into 16-bit parts)
  reqBuf[1] = address & 0xFFFF;         // address low 16 bits
  reqBuf[2] = (address >> 16) & 0xFFFF; // address high 16 bits

  // Execute the devctl command
  int res = fileXioDevctl("dvr_misc:", DVRMISC_DEVCTL_CFW_MEM_PEEK_POKE, reqBuf, sizeof(reqBuf), respBuf, sizeof(respBuf));

  // Fill the result structure
  result->result = res;
  if (res == 0) {
    result->old = ((uint32_t)respBuf[1] << 16) | respBuf[0];
    result->cur = ((uint32_t)respBuf[3] << 16) | respBuf[2];
  }

  logString("%d-bit mem peek @ 0x%08x:\n\t\tcmd res %d; old value: 0x%08x, cur value: 0x%08x\n", width, address, res, result->old, result->cur);
  return res;
}

// Performs a memory poke (write) operation
// Returns 0 on success, negative value on error
int dvrp_cfw_mem_poke(dvrp_cfw_mem_poke_params_t *params, dvrp_cfw_mem_peek_poke_result_t *result) {
  if (!params || !result)
    return -EINVAL;

  uint16_t reqBuf[20] = {0};
  uint16_t respBuf[20] = {0};

  // Set operation flags (poke = 1)
  switch (params->width) {
  case 8:
    reqBuf[0] = 0x0108; // poke (1), 8-bit (8)
    break;
  case 16:
    reqBuf[0] = 0x0110; // poke (1), 16-bit (16)
    break;
  case 32:
    reqBuf[0] = 0x0120; // poke (1), 32-bit (20)
    break;
  default:
    return -EINVAL; // Unsupported width
  }

  // Set address (split into 16-bit parts)
  reqBuf[1] = params->address & 0xFFFF;         // address low 16 bits
  reqBuf[2] = (params->address >> 16) & 0xFFFF; // address high 16 bits

  // Set value (split into 16-bit parts)
  reqBuf[3] = params->value & 0xFFFF;         // value low 16 bits
  reqBuf[4] = (params->value >> 16) & 0xFFFF; // value high 16 bits

  // Set mask (split into 16-bit parts)
  reqBuf[5] = params->mask & 0xFFFF;         // mask low 16 bits
  reqBuf[6] = (params->mask >> 16) & 0xFFFF; // mask high 16 bits

  // Execute the devctl command
  int res = fileXioDevctl("dvr_misc:", DVRMISC_DEVCTL_CFW_MEM_PEEK_POKE, reqBuf, sizeof(reqBuf), respBuf, sizeof(respBuf));

  // Fill the result structure
  result->result = res;
  if (res == 0) {
    result->old = ((uint32_t)respBuf[1] << 16) | respBuf[0];
    result->cur = ((uint32_t)respBuf[3] << 16) | respBuf[2];
  }

  logString("%d-bit mem poke @ 0x%08x with 0x%08x and mask 0x%08x:\n\t\tcmd res %d; old value: 0x%08x, cur value: 0x%08x\n", params->width, params->address,
            params->value, params->mask, res, result->old, result->cur);
  return res;
}
