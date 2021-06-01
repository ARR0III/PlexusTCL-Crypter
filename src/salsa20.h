#ifndef SALSA20_H
#define SALSA20_H

#include "salsa20.c"

void salsa20(uint8_t * message, uint64_t mlen, uint8_t key[32], uint64_t nonce);

#endif
