#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint32_t i, j;
  uint8_t  secret_key[256];
} ARC4_CTX;

void swap (uint8_t * a, uint8_t * b) {
  uint8_t t = *a;

  *a = *b;
  *b = t;
}

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, const size_t length) {

  for (ctx->i = 0; ctx->i < 256; ctx->i++) {
    ctx->secret_key[ctx->i] = (uint8_t)(ctx->i);
  }

  for (ctx->i = ctx->j = 0; ctx->i < 256; ctx->i++) {
    ctx->j = (ctx->j + key[ctx->i % length] + ctx->secret_key[ctx->i]) & 255;
    swap(&ctx->secret_key[ctx->i], &ctx->secret_key[ctx->j]);
  }

  ctx->i = ctx->j = 0;
}

/* MAX size data for encrypt = 4 GiB */
void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, size_t length) {
  register uint32_t position = 0;
  
  while (length--) {
    ctx->i = (ctx->i + 1) & 255;
    ctx->j = (ctx->j + ctx->secret_key[ctx->i]) & 255;
    
    swap(&ctx->secret_key[ctx->i], &ctx->secret_key[ctx->j]);
    
    output[position] = 
      input[position] ^ ctx->secret_key[(ctx->secret_key[ctx->i] +
                                         ctx->secret_key[ctx->j]) & 255];
    position++;
  }
}
