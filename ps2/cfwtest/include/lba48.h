#ifndef _LBA48_H_
#define _LBA48_H_

#include <stdint.h>

// Tests for LBA48 support by writing to lba48Sector and comparing it against its LBA28 counterpart
void testLBA48(uint32_t lba48Sector);

#endif
