#include <stdint.h>

int i, j;
uint8_t secret_key[256] = {0};

void swap (uint8_t * a, uint8_t * b) {
  uint8_t t = *a;

  *a = *b;
  *b = t;
}

void arc4_init(const uint8_t * key, int length) {

  for (i = 0; i < 256; i++)
    secret_key[i] = (uint8_t)i;

  for (i = j = 0; i < 256; i++) {
    j = (j + key[i % length] + secret_key[i]) % 256;
    swap(&secret_key[i], &secret_key[j]);
  }

  i = j = 0;
}

void arc4(uint8_t * data, uint8_t * post, int length) {
  register int z;
    for (z = 0; z < length; z++) {
      i = (i + 1) % 256;
      j = (j + secret_key[i]) % 256;
      swap(&secret_key[i], &secret_key[j]);
      post[z] = data[z] ^ secret_key[(secret_key[i] + secret_key[j]) % 256];
    }
}
