/*
 * Copyright 1999 Dr. Brian Gladman <brian.gladman@btinternet.com>
 * Copyright 2001 Abhijit Menon-Sen <ams@wiw.org>
 *
  
  correct {
    change by: ARR0III;
    data:      10.04.2021
  
    optimized compiler - not good!!!
  
    key_size - this is byte const (not bits!!!);
    key_len == (ONLY 128, 192, 256 / 8);
  
    TWOFISH_CTX - pointer to memory struct;
  };
  
  USED {
    #include <stdlib.h> - for call malloc function
    
    size_t twofish_ctx_len = sizeof(TWOFISH_CTX);
    TWOFISH_CTX * ctx = (TWOFISH_CTX *)malloc(twofish_ctx_len);
    
    if (NULL == ctx) {
      return (-1);
    }
    
    twofish_setup(ctx, key_buffer, key_len); - init memory struct
    
    twofish_encrypt(ctx, input, output); - encrypt data in input
    twofish_decrypt(ctx, output, input); - decrypt data in output
    
    memset(ctx, 0x00, twofish_ctx_len); - security >)
    
    free(ctx);
    ctx = NULL;
    
    return 0;
  };
*/

#ifndef TWOFISH_H
#define TWOFISH_H

#include "twofish.c"

void twofish_init(TWOFISH_CTX * ctx, uint8_t * key, int len);
void twofish_encrypt(TWOFISH_CTX * ctx, uint8_t * input, uint8_t * output);
void twofish_decrypt(TWOFISH_CTX * ctx, uint8_t * input, uint8_t * output);

#endif
