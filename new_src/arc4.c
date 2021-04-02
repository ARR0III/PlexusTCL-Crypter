#include <stdint.h>
#include <stddef.h>

size_t arc4_i, arc4_j;

typedef struct {
  uint8_t secret_key[256];
} ARC4_CTX;

void swap (uint8_t * a, uint8_t * b) {
  uint8_t t = *a;

  *a = *b;
  *b = t;
}

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, const size_t length) {

  for (arc4_i = 0; arc4_i < 256; arc4_i++) {
    ctx->secret_key[arc4_i] = (uint8_t)arc4_i;
  }

  for (arc4_i = arc4_j = 0; arc4_i < 256; arc4_i++) {
    arc4_j = (arc4_j + key[arc4_i % length] + ctx->secret_key[arc4_i]) % 256;
    swap(&ctx->secret_key[arc4_i], &ctx->secret_key[arc4_j]);
  }

  arc4_i = arc4_j = 0;
}

void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, const size_t length) {
  for (register size_t k = 0; k < length; k++) {
    arc4_i = (arc4_i + 1) % 256;
    arc4_j = (arc4_j + ctx->secret_key[arc4_i]) % 256;
    
    swap(&ctx->secret_key[arc4_i], &ctx->secret_key[arc4_j]);
    
    output[k] = 
     input[k] ^ ctx->secret_key[(ctx->secret_key[arc4_i] +
                                 ctx->secret_key[arc4_j]) % 256];
  }
}
