#ifndef SERPENT_H
#define SERPENT_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int32_t  keylen;

  uint32_t key[8];
  uint32_t w[132], k[132];
  uint32_t subkeys[33][4];
} SERPENT_CTX;

void serpent_init(SERPENT_CTX * key, const int keylen, const uint8_t * keymaterial);
void serpent_encrypt(SERPENT_CTX * ctx, uint32_t * plaintext, uint32_t * ciphertext);
void serpent_decrypt(SERPENT_CTX * ctx, uint32_t * ciphertext, uint32_t * plaintext);

#ifdef __cplusplus
}
#endif


#endif
