#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stddef.h>
//#include <stdlib.h>
//#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SHA256_BLOCK_SIZE 32

typedef struct {
  uint8_t  data[64];
  uint32_t datalen;
  uint64_t bitlen;
  uint32_t state[8];

  /* this is buffer for control sum */
  uint8_t  hash[SHA256_BLOCK_SIZE];
} SHA256_CTX;

void sha256_init(SHA256_CTX * ctx);
void sha256_update(SHA256_CTX * ctx, const uint8_t * data, const size_t len);
void sha256_final(SHA256_CTX * ctx); /* hash sum in ctx->hash */

#ifdef __cplusplus
}
#endif


#endif
