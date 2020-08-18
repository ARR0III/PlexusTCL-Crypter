/*
  Developer:    D. J. Bernstein;
  Modification: ARR0III;
*/
#include <stdint.h>

typedef struct {
  uint32_t key[32];
} CHACHA20_CTX;

#define ROUNDS    20
#define BLOCK_LEN 64

#define U8V(v)  ((uint8_t)(v)  & UINT8_C(0xFF))
#define U32V(v) ((uint32_t)(v) & UINT32_C(0xFFFFFFFF))

#define U8TO32_LITTLE(p) \
  (((uint32_t)((p)[0])      ) | \
   ((uint32_t)((p)[1]) <<  8) | \
   ((uint32_t)((p)[2]) << 16) | \
   ((uint32_t)((p)[3]) << 24))

#define U32TO8_LITTLE(p, v) \
  do { \
    (p)[0] = U8V((v)      ); \
    (p)[1] = U8V((v) >>  8); \
    (p)[2] = U8V((v) >> 16); \
    (p)[3] = U8V((v) >> 24); \
  } while (0)

#define XOR(v, w) ((v) ^ (w))
#define PLUSONE(v) (PLUS((v), 1))
#define PLUS(v, w) (U32V((v) + (w)))
#define ROTATE(v, c) (U32V((v) << (c)) | ((v) >> (32 - (c))))

#define QUARTERROUND(a, b, c, d) \
  x[a] = PLUS(x[a], x[b]); x[d] = ROTATE(XOR(x[d], x[a]), 16); \
  x[c] = PLUS(x[c], x[d]); x[b] = ROTATE(XOR(x[b], x[c]), 12); \
  x[a] = PLUS(x[a], x[b]); x[d] = ROTATE(XOR(x[d], x[a]), 8);  \
  x[c] = PLUS(x[c], x[d]); x[b] = ROTATE(XOR(x[b], x[c]), 7);

static void salsa20_wordtobyte(uint8_t output[BLOCK_LEN], const uint32_t input[16]) {
  uint32_t x[16];
  int i;

  for (i = 0; i < 16; ++i)
    x[i] = input[i];

  for (i = ROUNDS; i > 0; i -= 2) {
    QUARTERROUND(0, 4,  8, 12)
    QUARTERROUND(1, 5,  9, 13)
    QUARTERROUND(2, 6, 10, 14)
    QUARTERROUND(3, 7, 11, 15)
    QUARTERROUND(0, 5, 10, 15)
    QUARTERROUND(1, 6, 11, 12)
    QUARTERROUND(2, 7,  8, 13)
    QUARTERROUND(3, 4,  9, 14)
  }

  for (i = 0; i < 16; ++i)
    x[i] = PLUS(x[i], input[i]);

  for (i = 0; i < 16; ++i)
    U32TO8_LITTLE(output + 4 * i, x[i]);
}

/*
  Why this is constant not random ???
*/

static const char sigma[16] = "expand 32-byte k";
static const char tau[16]   = "expand 16-byte k";

void chacha20_keysetup(CHACHA20_CTX * ctx, const uint8_t * key, uint32_t keybits) {
  const char * constants;

  ctx->key[4] = U8TO32_LITTLE(key + 0);
  ctx->key[5] = U8TO32_LITTLE(key + 4);
  ctx->key[6] = U8TO32_LITTLE(key + 8);
  ctx->key[7] = U8TO32_LITTLE(key + 12);

  if (keybits == 256) { /* recommended */
    key += 16;
    constants = sigma;
  }
  else { /* kbits == 128 */
    constants = tau;
  }

  ctx->key[8]  = U8TO32_LITTLE(key + 0);
  ctx->key[9]  = U8TO32_LITTLE(key + 4);
  ctx->key[10] = U8TO32_LITTLE(key + 8);
  ctx->key[11] = U8TO32_LITTLE(key + 12);
  ctx->key[0]  = U8TO32_LITTLE(constants + 0);
  ctx->key[1]  = U8TO32_LITTLE(constants + 4);
  ctx->key[2]  = U8TO32_LITTLE(constants + 8);
  ctx->key[3]  = U8TO32_LITTLE(constants + 12);
}

void chacha20_ivsetup(CHACHA20_CTX * ctx, const uint8_t * iv) {
  ctx->key[12] = 0;
  ctx->key[13] = 0;
  ctx->key[14] = U8TO32_LITTLE(iv + 0);
  ctx->key[15] = U8TO32_LITTLE(iv + 4);
}

void chacha20_crypt(CHACHA20_CTX * ctx, 
                    const uint8_t * in, uint8_t * out, uint32_t length) {
  uint8_t temp[BLOCK_LEN];
  uint32_t i;

  for (; length > 0; length -= BLOCK_LEN) {
    salsa20_wordtobyte(temp, ctx->key);
    ctx->key[12] = PLUSONE(ctx->key[12]);
   
    if (ctx->key[12] == 0) {
      ctx->key[13] = PLUSONE(ctx->key[13]);
      /* !!! stopping at 2^70 bytes per nonce is user's responsibility !!! */
    }

    for (i = 0; i < length; ++i)
      out[i] = in[i] ^ temp[i];

    in  += BLOCK_LEN;
    out += BLOCK_LEN;
  }
}
