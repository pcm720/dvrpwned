/*
 * Direct C references to original firmware routines.
 *
 * Function addresses are supplied as absolute symbols in the linker script (e.g. memcpy = 0x10015a0a).
 */

#ifndef COMMON_FW_REFS_H
#define COMMON_FW_REFS_H

extern void *memcpy(void *dst, const void *src, unsigned int n);
extern void *memset(void *s, int c, unsigned int n);

#endif
