#ifndef _DVRP_CFW_H_
#define _DVRP_CFW_H_

#include <stdint.h>

#define DVRMISC_DEVCTL_CFW_PING 0x5690
#define DVRMISC_DEVCTL_CFW_GET_MAX_LBA48 0x5691
#define DVRMISC_DEVCTL_CFW_MEM_PEEK_POKE 0x5692

// Struct for memory poke operations
typedef struct {
  uint8_t width;     // 8, 16 or 32-bit operations
  uint32_t address;  // Memory address to access
  uint32_t value;    // Value for poke operations
  uint32_t mask;     // Mask for poke operations
} dvrp_cfw_mem_poke_params_t;

// Struct for the response from peek/poke operations
typedef struct {
  int result;   // Command result code
  uint32_t old; // Previous value at the address
  uint32_t cur; // Current value at the address
} dvrp_cfw_mem_peek_poke_result_t;

// Can be used to test if the DVRP firmware supports custom devctls
// Returns 0 on success, negative value on error
int dvrp_cfw_ping();

// Sets the result to the total sector count for the device
// Returns 0 on success, negative value on error
int dvrp_cfw_get_max_lba48(uint16_t device, uint64_t *result);

// Performs a memory peek (read) operation
// Returns 0 on success, negative value on error
int dvrp_cfw_mem_peek(uint32_t address, uint8_t width, dvrp_cfw_mem_peek_poke_result_t *result);

// Performs a memory poke (write) operation
// Returns 0 on success, negative value on error
int dvrp_cfw_mem_poke(dvrp_cfw_mem_poke_params_t *params, dvrp_cfw_mem_peek_poke_result_t *result);

#endif
