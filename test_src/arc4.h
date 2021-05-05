#include "arc4.c"

void arc4_init(ARC4_CTX * ctx, const uint8_t * key, const size_t length);
void arc4(ARC4_CTX * ctx, const uint8_t * input, uint8_t * output, const size_t length);
