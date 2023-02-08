#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <stdint.h>
#include <stddef.h> 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint32_t P[18];
  uint32_t S[4][256];
} BLOWFISH_CTX;

void blowfish_init(BLOWFISH_CTX *ctx, const unsigned char *key, const int keyLen);
void blowfish_encrypt(const BLOWFISH_CTX *ctx, uint32_t *xl, uint32_t *xr);
void blowfish_decrypt(const BLOWFISH_CTX *ctx, uint32_t *xl, uint32_t *xr);

#ifdef __cplusplus
}
#endif

#endif
