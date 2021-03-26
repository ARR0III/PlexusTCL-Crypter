#include <stdint.h>
#include <stddef.h>

size_t i, j;

typedef struct {
  uint8_t secret_key[256];
} ARC4_CTX;

void swap (uint8_t * a, uint8_t * b) {
  if ((*a) != (*b)) {
    uint8_t t = *a;

    *a = *b;
    *b = t;
  }
}

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, const size_t length) {

  for (i = 0; i < 256; i++) {
    ctx->secret_key[i] = (uint8_t)i;
  }

  for (i = j = 0; i < 256; i++) {
    j = (j + key[i % length] + ctx->secret_key[i]) % 256;
    swap(&ctx->secret_key[i], &ctx->secret_key[j]);
  }

  i = j = 0;
}

void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, const size_t length) {
  for (register size_t k = 0; k < length; k++) {
    i = (i + 1) % 256;
    j = (j + ctx->secret_key[i]) % 256;
    swap(&ctx->secret_key[i], &ctx->secret_key[j]);
    output[k] = input[k] ^ ctx->secret_key[(ctx->secret_key[i] + ctx->secret_key[j]) % 256];
  }
}
