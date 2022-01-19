#ifndef MD5_H
#define MD5_H

#ifdef __cplusplus
extern "C" {
#endif

#include "md5.c"

void md5(const uint8_t * input, size_t input_len, uint8_t * output);

#ifdef __cplusplus
}
#endif

#endif
