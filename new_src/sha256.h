#include <stdint.h>
#include "sha256.c"

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t * data, const size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t * hash);
