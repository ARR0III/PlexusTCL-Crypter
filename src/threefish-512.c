#include <stdint.h>

typedef unsigned long int      U32;
typedef unsigned long long int U64;

typedef struct {
  U64 SUBKEYS[19][8];
} THREEFISH_CTX;

const U64 C240  =   0x1BD11BDAA9FC1A22;

uint8_t PI[8]   =  {2, 1, 4, 7, 6, 5, 0, 3};
uint8_t RPI[8]  =  {6, 1, 0, 7, 2, 5, 4, 3};

uint8_t r[8][4] = {{46, 36, 19, 37},
                   {33, 27, 14, 42},
                   {17, 49, 36, 39},
                   {44,  9, 54, 56},
                   {39, 30, 34, 24},
                   {13, 50, 10, 17},
                   {25, 29, 39, 43},
                   { 8, 35, 56, 22}};

U64 t[3];

void mix(U64 * x, uint8_t * z, U64 * y) {
  y[0]  =  x[0] + x[1];
  y[1]  = (x[1] << *z) | (x[1] >> (64 - *z));
  y[1] ^=  y[0];
}

void demix(U64 * y, uint8_t * z, U64 * x) {
  y[1] ^=  y[0];
  x[1]  = (y[1] << (64 - *z)) | (y[1] >> *z);
  x[0]  =  y[0] - x[1];
}

void threefish_encrypt(THREEFISH_CTX * keys, U64 * input, U64 * output) {
  uint8_t round;
  uint8_t i;

  U64 f[8];
  U64 e[8];
  U64 v[8];

  U64 y[2];
  U64 x[2];

  memcpy(&v[0], input, 64);

  for (round = 0; round < 72; round++) {
    if (round % 4 == 0) {
      for (i = 0; i < 8; i++)
        e[i] = v[i] + keys->SUBKEYS[round / 4][i];
    } 
    else {
      for (i = 0; i < 8; i++)
        e[i] = v[i];
    }

    for (i = 0; i < (8 / 2); i++) {
      x[0] = e[i * 2];
      x[1] = e[i * 2 + 1];

      mix(x, &r[round % 8][i], y);

      f[i * 2]     = y[0];
      f[i * 2 + 1] = y[1];
    }

    for (i = 0; i < 8; i++)
      v[i] = f[PI[i]];
  }

  for (i = 0; i < 8; i++)
    output[i] = v[i] + keys->SUBKEYS[72 / 4][i];
}

void threefish_decrypt(THREEFISH_CTX * keys, U64 * input, U64 * output) {
  uint8_t round;
  uint8_t i;
  
  U64 f[8];
  U64 e[8];
  U64 v[8];

  U64 y[2];
  U64 x[2];

  memcpy(&v[0], input, 64);

  for (round = 72; round > 0; round--) {
    if (round % 4 == 0) {
      for (i = 0; i < 8; i++)
        f[i] = v[i] - keys->SUBKEYS[round / 4][i];
    }
    else {
      for (i = 0; i < 8; i++)
        f[i] = v[i];
    }

    for (i = 0; i < 8; i++)
      e[i] = f[RPI[i]];

    for (i = 0; i < (8 / 2); i++) {
      y[0] = e[i * 2];
      y[1] = e[i * 2 + 1];

      demix(y, &r[(round - 1) % 8][i], x);

      v[i * 2]     = x[0];
      v[i * 2 + 1] = x[1];
    }
  }

  for (i = 0; i < 8; i++)
    output[i] = v[i] - keys->SUBKEYS[0][i];
}

void threefish_init(THREEFISH_CTX * keys, U64 * keydata, U64 * vector) {
  uint8_t round;
  uint8_t i;

  U64 K[8];
  U64 T[2];
  U64 key[9];

  U64 k8 = C240;

  memcpy(&K[0], keydata, 64);
  memcpy(&T[0], vector, 16);

  for (i = 0; i < 8; i++) {
    k8   ^= K[i];
    key[i] = K[i];
  }

  key[8] = k8;

  t[0] = T[0];
  t[1] = T[1];
  t[2] = T[0] ^ T[1];

  for (round = 0; round <= (72 / 4); round++) {
    for (i = 0; i < 8; i++) {
      keys->SUBKEYS[round][i] = key[(round + i) % (8 + 1)];
      if (i == (8 - 3))
        keys->SUBKEYS[round][i] += t[round % 3];
      else
      if (i == (8 - 2))
        keys->SUBKEYS[round][i] += t[(round + 1) % 3];
      else
      if (i == (8 - 1))
        keys->SUBKEYS[round][i] += round;
    }
  }
}