#include "arc4.c"

void arc4_init(const uint8_t * key, int length);
void arc4(uint8_t * data, uint8_t * post, int length);