#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

// extern const uint32_t crc32_table[256];

/* Return a 32-bit CRC of the contents of the buffer. */
uint32_t CalcCRC32(uint32_t val, const void *ss, int len);

#endif
