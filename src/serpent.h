#include <stdint.h>

#include "serpent.c"

void serpent_init(SERPENT_CTX * key, int keylen, uint8_t * key_buffer);
void serpent_encrypt(SERPENT_CTX * ctx, unsigned long * plaintext, unsigned long * ciphertext);
void serpent_decrypt(SERPENT_CTX * ctx, unsigned long * ciphertext, unsigned long * plaintext);
