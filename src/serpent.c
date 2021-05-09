#include <string.h>
#include <stdint.h>
#include "serpentsboxes.h"

typedef struct {
  int32_t  keylen;

  uint32_t key[8];
  uint32_t w[132], k[132];
  uint32_t subkeys[33][4];
} SERPENT_CTX;

void serpent_init(SERPENT_CTX * ctx, const int keylen, const uint8_t * keymaterial) {
  int i, j, tempkeylen;

  ctx->keylen = keylen; /* only 128, 192 or 256 number */

  memmove((void*)ctx->key, (void*)keymaterial, keylen / 8); /* copy 16, 24 or 32 byte */

  tempkeylen = keylen / 32;

  for (i = 0; i < tempkeylen; i++) {
    ctx->w[i] = ctx->key[i];
  }

  if (keylen < 256) {
    ctx->w[i] = (ctx->key[i] & ((1L << ((keylen & 31))) - 1)) | (1L << ((keylen & 31)));
  }

  for(i++; i < 8; i++) {
    ctx->w[i] = 0;
  }

  for(i = 8; i < 16; i++) {
    ctx->w[i] = ROL(ctx->w[i - 8] ^
                    ctx->w[i - 5] ^
                    ctx->w[i - 3] ^
                    ctx->w[i - 1] ^ PHI ^ (i - 8), 11);
  }

  for(i = 0; i < 8; i++) {
    ctx->w[i] = ctx->w[i + 8];
  }

  for(i = 8; i < 132; i++) {
    ctx->w[i] = ROL(ctx->w[i - 8] ^
                    ctx->w[i - 5] ^
                    ctx->w[i - 3] ^
                    ctx->w[i - 1] ^ PHI ^ i, 11);
  }

  RND03(ctx->w[  0], ctx->w[  1], ctx->w[  2], ctx->w[  3], ctx->k[  0], ctx->k[  1], ctx->k[  2], ctx->k[  3]);
  RND02(ctx->w[  4], ctx->w[  5], ctx->w[  6], ctx->w[  7], ctx->k[  4], ctx->k[  5], ctx->k[  6], ctx->k[  7]);
  RND01(ctx->w[  8], ctx->w[  9], ctx->w[ 10], ctx->w[ 11], ctx->k[  8], ctx->k[  9], ctx->k[ 10], ctx->k[ 11]);
  RND00(ctx->w[ 12], ctx->w[ 13], ctx->w[ 14], ctx->w[ 15], ctx->k[ 12], ctx->k[ 13], ctx->k[ 14], ctx->k[ 15]);
  RND31(ctx->w[ 16], ctx->w[ 17], ctx->w[ 18], ctx->w[ 19], ctx->k[ 16], ctx->k[ 17], ctx->k[ 18], ctx->k[ 19]);
  RND30(ctx->w[ 20], ctx->w[ 21], ctx->w[ 22], ctx->w[ 23], ctx->k[ 20], ctx->k[ 21], ctx->k[ 22], ctx->k[ 23]);
  RND29(ctx->w[ 24], ctx->w[ 25], ctx->w[ 26], ctx->w[ 27], ctx->k[ 24], ctx->k[ 25], ctx->k[ 26], ctx->k[ 27]);
  RND28(ctx->w[ 28], ctx->w[ 29], ctx->w[ 30], ctx->w[ 31], ctx->k[ 28], ctx->k[ 29], ctx->k[ 30], ctx->k[ 31]);
  RND27(ctx->w[ 32], ctx->w[ 33], ctx->w[ 34], ctx->w[ 35], ctx->k[ 32], ctx->k[ 33], ctx->k[ 34], ctx->k[ 35]);
  RND26(ctx->w[ 36], ctx->w[ 37], ctx->w[ 38], ctx->w[ 39], ctx->k[ 36], ctx->k[ 37], ctx->k[ 38], ctx->k[ 39]);
  RND25(ctx->w[ 40], ctx->w[ 41], ctx->w[ 42], ctx->w[ 43], ctx->k[ 40], ctx->k[ 41], ctx->k[ 42], ctx->k[ 43]);
  RND24(ctx->w[ 44], ctx->w[ 45], ctx->w[ 46], ctx->w[ 47], ctx->k[ 44], ctx->k[ 45], ctx->k[ 46], ctx->k[ 47]);
  RND23(ctx->w[ 48], ctx->w[ 49], ctx->w[ 50], ctx->w[ 51], ctx->k[ 48], ctx->k[ 49], ctx->k[ 50], ctx->k[ 51]);
  RND22(ctx->w[ 52], ctx->w[ 53], ctx->w[ 54], ctx->w[ 55], ctx->k[ 52], ctx->k[ 53], ctx->k[ 54], ctx->k[ 55]);
  RND21(ctx->w[ 56], ctx->w[ 57], ctx->w[ 58], ctx->w[ 59], ctx->k[ 56], ctx->k[ 57], ctx->k[ 58], ctx->k[ 59]);
  RND20(ctx->w[ 60], ctx->w[ 61], ctx->w[ 62], ctx->w[ 63], ctx->k[ 60], ctx->k[ 61], ctx->k[ 62], ctx->k[ 63]);
  RND19(ctx->w[ 64], ctx->w[ 65], ctx->w[ 66], ctx->w[ 67], ctx->k[ 64], ctx->k[ 65], ctx->k[ 66], ctx->k[ 67]);
  RND18(ctx->w[ 68], ctx->w[ 69], ctx->w[ 70], ctx->w[ 71], ctx->k[ 68], ctx->k[ 69], ctx->k[ 70], ctx->k[ 71]);
  RND17(ctx->w[ 72], ctx->w[ 73], ctx->w[ 74], ctx->w[ 75], ctx->k[ 72], ctx->k[ 73], ctx->k[ 74], ctx->k[ 75]);
  RND16(ctx->w[ 76], ctx->w[ 77], ctx->w[ 78], ctx->w[ 79], ctx->k[ 76], ctx->k[ 77], ctx->k[ 78], ctx->k[ 79]);
  RND15(ctx->w[ 80], ctx->w[ 81], ctx->w[ 82], ctx->w[ 83], ctx->k[ 80], ctx->k[ 81], ctx->k[ 82], ctx->k[ 83]);
  RND14(ctx->w[ 84], ctx->w[ 85], ctx->w[ 86], ctx->w[ 87], ctx->k[ 84], ctx->k[ 85], ctx->k[ 86], ctx->k[ 87]);
  RND13(ctx->w[ 88], ctx->w[ 89], ctx->w[ 90], ctx->w[ 91], ctx->k[ 88], ctx->k[ 89], ctx->k[ 90], ctx->k[ 91]);
  RND12(ctx->w[ 92], ctx->w[ 93], ctx->w[ 94], ctx->w[ 95], ctx->k[ 92], ctx->k[ 93], ctx->k[ 94], ctx->k[ 95]);
  RND11(ctx->w[ 96], ctx->w[ 97], ctx->w[ 98], ctx->w[ 99], ctx->k[ 96], ctx->k[ 97], ctx->k[ 98], ctx->k[ 99]);
  RND10(ctx->w[100], ctx->w[101], ctx->w[102], ctx->w[103], ctx->k[100], ctx->k[101], ctx->k[102], ctx->k[103]);
  RND09(ctx->w[104], ctx->w[105], ctx->w[106], ctx->w[107], ctx->k[104], ctx->k[105], ctx->k[106], ctx->k[107]);
  RND08(ctx->w[108], ctx->w[109], ctx->w[110], ctx->w[111], ctx->k[108], ctx->k[109], ctx->k[110], ctx->k[111]);
  RND07(ctx->w[112], ctx->w[113], ctx->w[114], ctx->w[115], ctx->k[112], ctx->k[113], ctx->k[114], ctx->k[115]);
  RND06(ctx->w[116], ctx->w[117], ctx->w[118], ctx->w[119], ctx->k[116], ctx->k[117], ctx->k[118], ctx->k[119]);
  RND05(ctx->w[120], ctx->w[121], ctx->w[122], ctx->w[123], ctx->k[120], ctx->k[121], ctx->k[122], ctx->k[123]);
  RND04(ctx->w[124], ctx->w[125], ctx->w[126], ctx->w[127], ctx->k[124], ctx->k[125], ctx->k[126], ctx->k[127]);
  RND03(ctx->w[128], ctx->w[129], ctx->w[130], ctx->w[131], ctx->k[128], ctx->k[129], ctx->k[130], ctx->k[131]);

  for(i = 0; i <= 32; i++) { /* Make keys */
    for(j = 0; j < 4; j++) {
      ctx->subkeys[i][j] = ctx->k[4 * i + j];
    }
  }
}

void serpent_encrypt(SERPENT_CTX * ctx, const uint32_t * plaintext, uint32_t * ciphertext) {
  register uint32_t x0, x1, x2, x3;
  register uint32_t y0, y1, y2, y3;

  x0 = plaintext[0];
  x1 = plaintext[1];
  x2 = plaintext[2];
  x3 = plaintext[3];

  keying(x0, x1, x2, x3, ctx->subkeys[ 0]);
  RND00(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 1]);
  RND01(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 2]);
  RND02(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 3]);
  RND03(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 4]);
  RND04(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 5]);
  RND05(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 6]);
  RND06(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 7]);
  RND07(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 8]);
  RND08(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[ 9]);
  RND09(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[10]);
  RND10(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[11]);
  RND11(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[12]);
  RND12(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[13]);
  RND13(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[14]);
  RND14(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[15]);
  RND15(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[16]);
  RND16(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[17]);
  RND17(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[18]);
  RND18(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[19]);
  RND19(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[20]);
  RND20(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[21]);
  RND21(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[22]);
  RND22(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[23]);
  RND23(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[24]);
  RND24(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[25]);
  RND25(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[26]);
  RND26(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[27]);
  RND27(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[28]);
  RND28(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[29]);
  RND29(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[30]);
  RND30(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, ctx->subkeys[31]);
  RND31(x0, x1, x2, x3, y0, y1, y2, y3);
  x0 = y0; x1 = y1; x2 = y2; x3 = y3;
  keying(x0, x1, x2, x3, ctx->subkeys[32]);

  ciphertext[0] = x0;
  ciphertext[1] = x1;
  ciphertext[2] = x2;
  ciphertext[3] = x3;
}

void serpent_decrypt(SERPENT_CTX * ctx, const uint32_t * ciphertext, uint32_t * plaintext) {
  register uint32_t x0, x1, x2, x3;
  register uint32_t y0, y1, y2, y3;

  x0 = ciphertext[0];
  x1 = ciphertext[1];
  x2 = ciphertext[2];
  x3 = ciphertext[3];

  keying(x0, x1, x2, x3, ctx->subkeys[32]);
  InvRND31(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[31]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND30(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[30]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND29(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[29]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND28(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[28]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND27(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[27]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND26(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[26]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND25(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[25]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND24(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[24]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND23(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[23]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND22(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[22]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND21(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[21]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND20(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[20]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND19(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[19]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND18(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[18]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND17(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[17]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND16(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[16]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND15(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[15]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND14(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[14]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND13(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[13]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND12(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[12]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND11(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[11]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND10(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[10]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND09(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 9]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND08(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 8]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND07(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 7]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND06(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 6]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND05(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 5]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND04(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 4]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND03(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 3]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND02(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 2]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND01(x0, x1, x2, x3, y0, y1, y2, y3);
  keying(y0, y1, y2, y3, ctx->subkeys[ 1]);
  inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
  InvRND00(x0, x1, x2, x3, y0, y1, y2, y3);

  x0 = y0;
  x1 = y1;
  x2 = y2;
  x3 = y3;

  keying(x0, x1, x2, x3, ctx->subkeys[0]);

  plaintext[0] = x0;
  plaintext[1] = x1;
  plaintext[2] = x2;
  plaintext[3] = x3;
}
