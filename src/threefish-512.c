#include <stdint.h>

typedef struct {
  uint64_t SUBKEYS[19][8];

  uint64_t f[8];
  uint64_t e[8];
  uint64_t v[8];

  uint64_t y[2];
  uint64_t x[2];

  uint64_t K[8];
  uint64_t T[2];
  uint64_t key[9];

  uint64_t t[3];
} THREEFISH_CTX;

const uint64_t C240 = 0x1BD11BDAA9FC1A22;

uint8_t PI[8]  = {2, 1, 4, 7, 6, 5, 0, 3};
uint8_t RPI[8] = {6, 1, 0, 7, 2, 5, 4, 3};

uint8_t rex[8][4] = {{46, 36, 19, 37},
                     {33, 27, 14, 42},
                     {17, 49, 36, 39},
                     {44,  9, 54, 56},
                     {39, 30, 34, 24},
                     {13, 50, 10, 17},
                     {25, 29, 39, 43},
                     { 8, 35, 56, 22}};

void mix(uint64_t * x, uint8_t * z, uint64_t * y) {
  y[0]  =  x[0] + x[1];
  y[1]  = (x[1] << *z) | (x[1] >> (64 - *z));
  y[1] ^=  y[0];
}

void demix(uint64_t * y, uint8_t * z, uint64_t * x) {
  y[1] ^=  y[0];
  x[1]  = (y[1] << (64 - *z)) | (y[1] >> *z);
  x[0]  =  y[0] - x[1];
}

void threefish_encrypt(THREEFISH_CTX * ctx, const uint64_t * input, uint64_t * output) {
  uint8_t round, i;

  memmove(ctx->v, input, 64);

  for (round = 0; round < 72; round++) {
    if (round % 4 == 0) {
      for (i = 0; i < 8; i++)
        ctx->e[i] = ctx->v[i] + ctx->SUBKEYS[round / 4][i];
    }
    else {
      for (i = 0; i < 8; i++)
        ctx->e[i] = ctx->v[i];
    }

    for (i = 0; i < (8 / 2); i++) {
      ctx->x[0] = ctx->e[i * 2];
      ctx->x[1] = ctx->e[i * 2 + 1];

      mix(ctx->x, &rex[round % 8][i], ctx->y);

      ctx->f[i * 2]     = ctx->y[0];
      ctx->f[i * 2 + 1] = ctx->y[1];
    }

    for (i = 0; i < 8; i++)
      ctx->v[i] = ctx->f[PI[i]];
  }

  for (i = 0; i < 8; i++)
    output[i] = ctx->v[i] + ctx->SUBKEYS[18][i];
}

void threefish_decrypt(THREEFISH_CTX * ctx, const uint64_t * input, uint64_t * output) {
  uint8_t round, i;

  memmove(ctx->v, input, 64);

  for (round = 72; round > 0; round--) {
    if (round % 4 == 0) {
      for (i = 0; i < 8; i++)
        ctx->f[i] = ctx->v[i] - ctx->SUBKEYS[round / 4][i];
    }
    else {
      for (i = 0; i < 8; i++)
        ctx->f[i] = ctx->v[i];
    }

    for (i = 0; i < 8; i++)
      ctx->e[i] = ctx->f[RPI[i]];

    for (i = 0; i < (8 / 2); i++) {
      ctx->y[0] = ctx->e[i * 2];
      ctx->y[1] = ctx->e[i * 2 + 1];

      demix(ctx->y, &rex[(round - 1) % 8][i], ctx->x);

      ctx->v[i * 2]     = ctx->x[0];
      ctx->v[i * 2 + 1] = ctx->x[1];
    }
  }

  for (i = 0; i < 8; i++)
    output[i] = ctx->v[i] - ctx->SUBKEYS[0][i];
}

void threefish_init(THREEFISH_CTX * ctx, const uint64_t * keydata, const uint64_t * vector) {
  uint8_t round, i;

  uint64_t k8 = C240;

  memmove(ctx->K, keydata, 64);
  memmove(ctx->T, vector, 16);

  for (i = 0; i < 8; i++) {
    k8         ^= ctx->K[i];
    ctx->key[i] = ctx->K[i];
  }

  ctx->key[8] = k8;

  ctx->t[0] = ctx->T[0];
  ctx->t[1] = ctx->T[1];
  ctx->t[2] = ctx->T[0] ^ ctx->T[1];

  for (round = 0; round <= 18; round++) {
    for (i = 0; i < 8; i++) {
      ctx->SUBKEYS[round][i] = ctx->key[(round + i) % 9];
      if (i == 5)
        ctx->SUBKEYS[round][i] += ctx->t[round % 3];
      else
      if (i == 6)
        ctx->SUBKEYS[round][i] += ctx->t[(round + 1) % 3];
      else
      if (i == 7)
        ctx->SUBKEYS[round][i] += round;
    }
  }
}

