#include <stdint.h>

uint32_t R(uint32_t x, uint8_t n) {
  return ((x << n) | (x >> (32 - n)));
}

void quarter(uint32_t * a, uint32_t * b, uint32_t * c, uint32_t * d) {
  *b = *b ^ R(*d + *a,  7);
  *c = *c ^ R(*a + *b,  9);
  *d = *d ^ R(*b + *c, 13);
  *a = *a ^ R(*c + *d, 18);
}

void salsa20_words(uint32_t * out, uint32_t in[16]) {
  uint32_t x[4][4];
  int i;

  for (i = 0; i < 16; ++i)
    x[i / 4][i % 4] = in[i];

  for (i = 0; i < 10; ++i) {
    quarter(&x[0][0], &x[1][0], &x[2][0], &x[3][0]);
    quarter(&x[1][1], &x[2][1], &x[3][1], &x[0][1]);
    quarter(&x[2][2], &x[3][2], &x[0][2], &x[1][2]);
    quarter(&x[3][3], &x[0][3], &x[1][3], &x[2][3]);

    quarter(&x[0][0], &x[0][1], &x[0][2], &x[0][3]);
    quarter(&x[1][1], &x[1][2], &x[1][3], &x[1][0]);
    quarter(&x[2][2], &x[2][3], &x[2][0], &x[2][1]);
    quarter(&x[3][3], &x[3][0], &x[3][1], &x[3][2]);
  }

  for (i = 0; i < 16; ++i)
    out[i] = x[i / 4][i % 4] + in[i];
}

static const char expand[16] = "expand 32-byte k";

void salsa20_block(uint8_t * out, uint8_t key[32], uint64_t nonce, uint64_t index) {
  #define LE(p) ((p)[0] | ((p)[1] << 8) | ((p)[2] << 16) | ((p)[3] << 24))
  uint32_t in[16] = {LE(expand),         LE(key),        LE(key + 4),        LE(key + 8),
                     LE(key + 12),       LE(expand + 4), nonce & 0xffffffff, nonce >> 32,
                     index & 0xffffffff, index >> 32,    LE(expand + 8),     LE(key + 16),
                     LE(key + 20),       LE(key + 24),   LE(key + 28),       LE(expand + 12)};
  uint32_t wordout[16];
  salsa20_words(wordout, in);

  for (int i = 0; i < 64; ++i)
    out[i] = 0xff & (wordout[i / 4] >> (8 * (i % 4)));
}

void salsa20(uint8_t * message, uint64_t mlen, uint8_t key[32], uint64_t nonce) {
  uint8_t block[64];

  for (uint64_t i = 0; i < mlen; i++) {
    if (i % 64 == 0)
      salsa20_block(block, key, nonce, i / 64);
   
    message[i] ^= block[i % 64];
  }
}
