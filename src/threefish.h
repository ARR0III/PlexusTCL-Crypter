#ifndef THREEFISH_H
#define THREEFISH_H

#include <stdint.h>

#include "threefish.c"

void threefish_init(THREEFISH_CTX * keyCtx, threefishkeysize_t stateSize,
                    uint64_t * keyData, uint64_t * tweak);
  
void threefishEncrypt256(THREEFISH_CTX * ctx,
                         uint64_t * input, uint64_t * output);
void threefishEncrypt512(THREEFISH_CTX * ctx,
                         uint64_t * input, uint64_t * output);
void threefishEncrypt1024(THREEFISH_CTX * ctx,
                          uint64_t * input, uint64_t * output);
  
void threefishDecrypt256(THREEFISH_CTX * ctx,
                         uint64_t * input, uint64_t * output);
void threefishDecrypt512(THREEFISH_CTX * ctx,
                         uint64_t * input, uint64_t * output);
void threefishDecrypt1024(THREEFISH_CTX * ctx,
                          uint64_t * input, uint64_t * output);
                            
void threefish_encrypt(THREEFISH_CTX * ctx,
                       uint64_t * in, uint64_t * out);
                         
void threefish_decrypt(THREEFISH_CTX * ctx,
                       uint64_t * in, uint64_t * out);
#endif
