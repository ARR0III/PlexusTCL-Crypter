#ifndef THREEFISHAPI_H
  #define THREEFISHAPI_H

  #ifndef _C_STDINT_H_
  #define _C_STDINT_H_
    #include <stdint.h>
  #endif  

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
                         uint64_t * in, uint64_t * out,
                         threefishkeysize_t keysize);
                         
  void threefish_decrypt(THREEFISH_CTX * ctx,
                         uint64_t * in, uint64_t * out,
                         threefishkeysize_t keysize);
#endif
