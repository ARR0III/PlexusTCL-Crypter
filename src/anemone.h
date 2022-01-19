#ifndef ANEMONE_H
#define ANEMONE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "anemone.c"

void anemone_init(ANEMONE_CTX * ctx, uint8_t * key, int key_len, int operation);

void anemone_encrypt(ANEMONE_CTX * ctx, uint8_t * in, uint8_t * out);
void anemone_decrypt(ANEMONE_CTX * ctx, uint8_t * in, uint8_t * out);

#ifdef __cplusplus
}
#endif

#endif ANEMONE_H