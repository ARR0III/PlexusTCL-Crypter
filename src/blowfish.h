#include "blowfish.c"

void blowfish_init(BLOWFISH_CTX *ctx, unsigned char *key, int keyLen);
void blowfish_encrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);
void blowfish_decrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);