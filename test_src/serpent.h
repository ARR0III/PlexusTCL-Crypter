#include "serpent.c"

void serpent_init(SERPENT_CTX * key, const int keylen, const uint8_t * keymaterial);
void serpent_encrypt(SERPENT_CTX * ctx, uint32_t * plaintext, uint32_t * ciphertext);
void serpent_decrypt(SERPENT_CTX * ctx, uint32_t * ciphertext, uint32_t * plaintext);
