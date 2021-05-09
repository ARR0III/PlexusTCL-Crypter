#include "rijndael.c"

int rijndael_key_encrypt_init(uint32_t * table, const uint8_t * cipher_key, int key_bits);
int rijndael_key_decrypt_init(uint32_t * table, const uint8_t * cipher_key, int key_bits);

void rijndael_encrypt(const uint32_t * table, const uint8_t * pt, uint8_t * ct);
void rijndael_decrypt(const uint32_t * table, const uint8_t * ct, uint8_t * pt);

