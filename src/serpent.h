#include <stdint.h>
#include "serpent.c"

void serpent_init(SERPENT_CTX * key, const int keylen, const uint8_t * keymaterial);
void serpent_encrypt(SERPENT_CTX * ctx, const unsigned long * plaintext, unsigned long * ciphertext);
void serpent_decrypt(SERPENT_CTX * ctx, const unsigned long * ciphertext, unsigned long * plaintext);
