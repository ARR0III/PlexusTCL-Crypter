/*
 * Copyright 1999 Dr. Brian Gladman <brian.gladman@btinternet.com>
 * Copyright 2001 Abhijit Menon-Sen <ams@wiw.org>
 */

#ifndef TWOFISH_H
#define TWOFISH_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _TWOFISH_H_
#define _TWOFISH_H_
  typedef struct {
    int len;
    uint32_t K[40];
    uint32_t S[4][256];
  } TWOFISH_CTX;
#endif

uint32_t mds_rem(uint32_t a, uint32_t b);
uint32_t h(int len, const int x, uint8_t *key, int odd);

void twofish_init(TWOFISH_CTX * ctx, uint8_t * key, int len);
void twofish_encrypt(TWOFISH_CTX * ctx, uint8_t * input, uint8_t * output);
void twofish_decrypt(TWOFISH_CTX * ctx, uint8_t * input, uint8_t * output);

#ifdef __cplusplus
}
#endif

#endif
