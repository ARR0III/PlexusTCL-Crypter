#ifndef ARC4_H
#define ARC4_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint32_t i, j;
  uint8_t  secret_key[256];
} ARC4_CTX;

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, size_t length);
void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, size_t length);

#ifdef __cplusplus
}
#endif

#endif
