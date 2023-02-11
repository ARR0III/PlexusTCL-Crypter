#include "arc4.h"

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, const size_t length) {
  uint8_t t;

  for (ctx->i = 0; ctx->i < 256; ctx->i++) {
    ctx->secret_key[ctx->i] = (uint8_t)(ctx->i);
  }
 
  for (ctx->i = ctx->j = 0; ctx->i < 256; ctx->i++) {
    ctx->j = (ctx->j + key[ctx->i % length] + ctx->secret_key[ctx->i]) & 255;

     
    t = ctx->secret_key[ctx->i];
    ctx->secret_key[ctx->i] = ctx->secret_key[ctx->j];
    ctx->secret_key[ctx->j] = t;
    
  }

  ctx->i = ctx->j = 0;
}

/* MAX size data for encrypt = 4 GiB */
void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, size_t length) {
  uint8_t t;  
  register size_t position = 0;
  
  while (length--) {
    ctx->i = (ctx->i + 1) & 255;
    ctx->j = (ctx->j + ctx->secret_key[ctx->i]) & 255;
    
    t = ctx->secret_key[ctx->i];
    ctx->secret_key[ctx->i] = ctx->secret_key[ctx->j];
    ctx->secret_key[ctx->j] = t;
    
    output[position] = 
      input[position] ^ ctx->secret_key[(ctx->secret_key[ctx->i] +
                                         ctx->secret_key[ctx->j]) & 255];
    position++;
  }
}
