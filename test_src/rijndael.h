#include "rijndael.c"

void rijndael_init(const uint8_t *key, uint8_t *w);
void rijndael_encrypt(uint8_t *w, const uint8_t *in, uint8_t *out);
void rijndael_decrypt(uint8_t *w, const uint8_t *in, uint8_t *out);
