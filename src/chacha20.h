#ifndef CHACHA20_h
#define CHACHA20_h

#ifdef __cplusplus
extern "C" {
#endif

#include "chacha20.c"

void chacha20_keysetup(CHACHA20_CTX * ctx, const uint8_t * key, uint32_t keybits);
void chacha20_ivsetup(CHACHA20_CTX * ctx, const uint8_t * iv);
void chacha20_crypt(CHACHA20_CTX * ctx, const uint8_t * in, uint8_t * out, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
