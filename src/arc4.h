#ifndef ARC4_H
#define ARC4_H

#ifdef __cplusplus
extern "C" {
#endif

#include "arc4.c"

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, const size_t length);
void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, size_t length);

#ifdef __cplusplus
}
#endif

#endif
