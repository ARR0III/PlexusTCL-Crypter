#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>
#include <stddef.h> 

#ifdef __cplusplus
extern "C" {
#endif

uint32_t CRC32(const uint8_t * buffer, size_t len);

#ifdef __cplusplus
}
#endif

#endif
