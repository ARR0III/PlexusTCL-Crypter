#ifndef BASE85_H
#define BASE85_H

#include "base85.c"

int32_t base85_decode_len(int32_t len); /* atob */
int32_t base85_encode_len(int32_t len); /* btoa */

int base85_encode(const uint8_t * input, int inlen, uint8_t * output);
int base85_decode(const uint8_t * input, int inlen, uint8_t * output);

#endif
