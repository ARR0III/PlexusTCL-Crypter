#include "threefish-512.c"

void threefish_encrypt(THREEFISH_CTX * keys, U64 * input, U64 * output);
void threefish_decrypt(THREEFISH_CTX * keys, U64 * input, U64 * output);

void threefish_init(THREEFISH_CTX * keys, U64 * keydata, U64 * vector);