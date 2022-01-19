#ifndef THREEFISH_512_H
#define THREEFISH_512_H

#ifdef __cplusplus
extern "C" {
#endif

#include "threefish-512.c"

void threefish_init(THREEFISH_CTX * ctx, const uint64_t * keydata, const uint64_t * vector);

void threefish_encrypt(THREEFISH_CTX * ctx, const uint64_t * input, uint64_t * output);
void threefish_decrypt(THREEFISH_CTX * ctx, const uint64_t * input, uint64_t * output);

#ifdef __cplusplus
}
#endif

#endif
