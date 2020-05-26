#include "arc4.c"

void arc4_init(const uint8_t * key, const int length);
void arc4(const uint8_t * data, uint8_t * post, const int length);
