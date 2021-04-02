#include "blowfish.c"

void blowfish_init(BLOWFISH_CTX *ctx, const unsigned char *key, const int keyLen);
void blowfish_encrypt(const BLOWFISH_CTX *ctx, uint32_t *xl, uint32_t *xr);
void blowfish_decrypt(const BLOWFISH_CTX *ctx, uint32_t *xl, uint32_t *xr);
