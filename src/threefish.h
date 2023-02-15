#ifndef THREEFISH_H
#define THREEFISH_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ThreefishSize {
    Threefish256  = 256,
    Threefish512  = 512,
    Threefish1024 = 1024
} threefishkeysize_t;

typedef struct ThreefishKey {
    uint64_t keysize;
    uint64_t key[17];
    uint64_t tweak[3];
} THREEFISH_CTX;

void threefish_init(THREEFISH_CTX * keyCtx, threefishkeysize_t stateSize,
                    const uint64_t * keyData, const uint64_t * tweak);
					
					
					
void threefish_encrypt(THREEFISH_CTX * ctx,
                       const uint64_t * in, uint64_t * out);          
void threefish_decrypt(THREEFISH_CTX * ctx,
                       const uint64_t * in, uint64_t * out);

 
 
void threefishEncrypt256(THREEFISH_CTX * ctx,
                         const uint64_t * input, uint64_t * output);
void threefishEncrypt512(THREEFISH_CTX * ctx,
                         const uint64_t * input, uint64_t * output);
void threefishEncrypt1024(THREEFISH_CTX * ctx,
                         const uint64_t * input, uint64_t * output);
						  
						  
void threefishDecrypt256(THREEFISH_CTX * ctx,
                         const uint64_t * input, uint64_t * output);
void threefishDecrypt512(THREEFISH_CTX * ctx,
                         const uint64_t * input, uint64_t * output);
void threefishDecrypt1024(THREEFISH_CTX * ctx,
                         const uint64_t * input, uint64_t * output);   
						 
					   
#ifdef __cplusplus
}
#endif

#endif
