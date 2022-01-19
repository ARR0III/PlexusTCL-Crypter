#ifndef CRC32_H
#define CRC32_H

#ifdef __cplusplus
extern "C" {
#endif

#include "crc32.c"

uint32_t CRC32(const uint8_t * buffer, uint64_t len);

#ifdef __cplusplus
}
#endif

#endif
