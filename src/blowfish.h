#include "blowfish.c"

void blowfish_init(BLOWFISH_CTX *ctx, const unsigned char *key, const int keyLen);
void blowfish_encrypt(const BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);
void blowfish_decrypt(const BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);
