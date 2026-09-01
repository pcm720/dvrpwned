/*
 * Direct C references to original firmware routines.
 *
 * Function addresses are supplied as absolute symbols in the linker script (e.g. devctlSendResponse = 0x100149c2).
 */

#ifndef MISC_DEVCTL_REFS_H
#define MISC_DEVCTL_REFS_H

typedef struct {
  unsigned short responseCommand;  // Initialized by miscDevctlCheckCommand
  unsigned short responseData[64]; // First byte is always the command status
  unsigned short responseLength;   // Total response length
  unsigned short requestCommand;
  unsigned short requestData[64];
  unsigned short requestLength;
} devctl_cmd_data_t;

enum DVRP_CMD_STATUS {
  DVRP_CMD_ACK = 0x2,  // DvrdrvExecCmdAck commands can simply return DVRP_CMD_ACK with status and data
  DVRP_CMD_COMP = 0x4, // Used only for DvrdrvExecCmdAckComp commands ("long" commands, e.g., RAM testing)
                       // Commands that fail precondition checks can return just DVRP_CMD_ACK with negative status
                       // If precondition checks succeed, always respond with DVRP_CMD_ACK and status 0x0 before doing anything
                       // After completing the command, respond with DVRP_CMD_COMP
};

// Sends the response. Common for all devctls
extern void devctlSendResponse(devctl_cmd_data_t *responseBuffer, enum DVRP_CMD_STATUS cmdStatus);

#endif
