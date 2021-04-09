/*
  $Id: twofish.h,v 2.12 2001/05/21 17:38:01 ams Exp $
  Copyright 2001 Abhijit Menon-Sen <ams@wiw.org>
  
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
  
    #define ENCRYPT 0x00
    #define DECRYPT 0xDE
    
    size_t twofish_ctx_len = sizeof(TWOFISH_CTX);
    TWOFISH_CTX * ctx = (TWOFISH_CTX *)malloc(twofish_ctx_len);
    
    if (NULL == ctx) {
      return (-1);
    }
    
    twofish_setup(ctx, key_buffer, key_len); - init memory struct
    twofish_crypt(ctx, input, output, ENCRYPT/DECRYPT);
    
    memset(ctx, 0x00, twofish_ctx_len); - security >)
    
    free(ctx);
    ctx = NULL;
    
    return 0;
  };
 */
#include <stdlib.h>

#include "twofish.c"

#ifndef _PLATFORM_H_
#define _PLATFORM_H_
  #include <inttypes.h>
#endif

void twofish_init(TWOFISH_CTX * ctx, uint8_t * key, int len);
void twofish_crypt(TWOFISH_CTX * ctx, uint8_t * input, uint8_t * output, int crypt);
