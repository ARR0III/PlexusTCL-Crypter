#include "arc4.h"

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, const size_t length) {
  uint8_t t;
  size_t i, j, k;
	  
  for (i = 0; i < 256; i++) {
    ctx->arc4_key[i] = (uint8_t)(i);
  }
 
  for (i = j = k = 0; i < 256; i++) {
    j = (j + key[k] + ctx->arc4_key[i]) & 255;

    k++;
	
    if (k >= length) {
      k = 0;
    }
     
    t = ctx->arc4_key[i];
    ctx->arc4_key[i] = ctx->arc4_key[j];
    ctx->arc4_key[j] = t;
  }
}

/* MAX size data for encrypt = 4 GiB */
void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, size_t length) {
  uint8_t t;  
  register size_t pos = 0;
  size_t i, j;
  
  i = j = 0;
  
  while (length--) {
    i = (i + 1) & 255;
    j = (j + ctx->arc4_key[i]) & 255;
    
    t = ctx->arc4_key[i];
    ctx->arc4_key[i] = ctx->arc4_key[j];
    ctx->arc4_key[j] = t;
    
    output[pos] =
     input[pos] ^ ctx->arc4_key[(ctx->arc4_key[i] +
                                 ctx->arc4_key[j]) & 255];
    pos++;
  }
}
