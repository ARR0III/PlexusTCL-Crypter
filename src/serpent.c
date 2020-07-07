#include <string.h>
#include <stdint.h>
#include "serpentsboxes.h"

typedef struct {
  int           keylen;
  unsigned long key[8];
  unsigned long w[132], k[132];
  unsigned long subkeys[33][4];
} SERPENT_CTX;

void serpent_init(SERPENT_CTX * key, const int keylen, const uint8_t * keymaterial) {
  int i, j, tempkeylen;

  key->keylen = keylen; /* only 128, 192 or 256 number */

  memmove((void*)key->key, (void*)keymaterial, keylen / 8); /* copy 16, 24 or 32 byte */

  tempkeylen = keylen / 32;

  for (i = 0; i < tempkeylen; i++)
    key->w[i] = key->key[i];

  if (keylen < 256)
    key->w[i] = (key->key[i] & ((1L << ((keylen & 31))) - 1)) | (1L << ((keylen & 31)));

  for(i++; i < 8; i++)
    key->w[i] = 0;

  for(i = 8; i < 16; i++)
    key->w[i] = ROL(key->w[i - 8] ^ key->w[i - 5] ^ key->w[i - 3] ^ key->w[i - 1] ^ PHI ^ (i - 8), 11);

  for(i = 0; i < 8; i++)
    key->w[i] = key->w[i + 8];

  for(i = 8; i < 132; i++)
    key->w[i] = ROL(key->w[i - 8] ^ key->w[i - 5] ^ key->w[i - 3] ^ key->w[i - 1] ^ PHI ^ i, 11);

  RND03(key->w[  0], key->w[  1], key->w[  2], key->w[  3], key->k[  0], key->k[  1], key->k[  2], key->k[  3]);
  RND02(key->w[  4], key->w[  5], key->w[  6], key->w[  7], key->k[  4], key->k[  5], key->k[  6], key->k[  7]);
  RND01(key->w[  8], key->w[  9], key->w[ 10], key->w[ 11], key->k[  8], key->k[  9], key->k[ 10], key->k[ 11]);
  RND00(key->w[ 12], key->w[ 13], key->w[ 14], key->w[ 15], key->k[ 12], key->k[ 13], key->k[ 14], key->k[ 15]);
  RND31(key->w[ 16], key->w[ 17], key->w[ 18], key->w[ 19], key->k[ 16], key->k[ 17], key->k[ 18], key->k[ 19]);
  RND30(key->w[ 20], key->w[ 21], key->w[ 22], key->w[ 23], key->k[ 20], key->k[ 21], key->k[ 22], key->k[ 23]);
  RND29(key->w[ 24], key->w[ 25], key->w[ 26], key->w[ 27], key->k[ 24], key->k[ 25], key->k[ 26], key->k[ 27]);
  RND28(key->w[ 28], key->w[ 29], key->w[ 30], key->w[ 31], key->k[ 28], key->k[ 29], key->k[ 30], key->k[ 31]);
  RND27(key->w[ 32], key->w[ 33], key->w[ 34], key->w[ 35], key->k[ 32], key->k[ 33], key->k[ 34], key->k[ 35]);
  RND26(key->w[ 36], key->w[ 37], key->w[ 38], key->w[ 39], key->k[ 36], key->k[ 37], key->k[ 38], key->k[ 39]);
  RND25(key->w[ 40], key->w[ 41], key->w[ 42], key->w[ 43], key->k[ 40], key->k[ 41], key->k[ 42], key->k[ 43]);
  RND24(key->w[ 44], key->w[ 45], key->w[ 46], key->w[ 47], key->k[ 44], key->k[ 45], key->k[ 46], key->k[ 47]);
  RND23(key->w[ 48], key->w[ 49], key->w[ 50], key->w[ 51], key->k[ 48], key->k[ 49], key->k[ 50], key->k[ 51]);
  RND22(key->w[ 52], key->w[ 53], key->w[ 54], key->w[ 55], key->k[ 52], key->k[ 53], key->k[ 54], key->k[ 55]);
  RND21(key->w[ 56], key->w[ 57], key->w[ 58], key->w[ 59], key->k[ 56], key->k[ 57], key->k[ 58], key->k[ 59]);
  RND20(key->w[ 60], key->w[ 61], key->w[ 62], key->w[ 63], key->k[ 60], key->k[ 61], key->k[ 62], key->k[ 63]);
  RND19(key->w[ 64], key->w[ 65], key->w[ 66], key->w[ 67], key->k[ 64], key->k[ 65], key->k[ 66], key->k[ 67]);
  RND18(key->w[ 68], key->w[ 69], key->w[ 70], key->w[ 71], key->k[ 68], key->k[ 69], key->k[ 70], key->k[ 71]);
  RND17(key->w[ 72], key->w[ 73], key->w[ 74], key->w[ 75], key->k[ 72], key->k[ 73], key->k[ 74], key->k[ 75]);
  RND16(key->w[ 76], key->w[ 77], key->w[ 78], key->w[ 79], key->k[ 76], key->k[ 77], key->k[ 78], key->k[ 79]);
  RND15(key->w[ 80], key->w[ 81], key->w[ 82], key->w[ 83], key->k[ 80], key->k[ 81], key->k[ 82], key->k[ 83]);
  RND14(key->w[ 84], key->w[ 85], key->w[ 86], key->w[ 87], key->k[ 84], key->k[ 85], key->k[ 86], key->k[ 87]);
  RND13(key->w[ 88], key->w[ 89], key->w[ 90], key->w[ 91], key->k[ 88], key->k[ 89], key->k[ 90], key->k[ 91]);
  RND12(key->w[ 92], key->w[ 93], key->w[ 94], key->w[ 95], key->k[ 92], key->k[ 93], key->k[ 94], key->k[ 95]);
  RND11(key->w[ 96], key->w[ 97], key->w[ 98], key->w[ 99], key->k[ 96], key->k[ 97], key->k[ 98], key->k[ 99]);
  RND10(key->w[100], key->w[101], key->w[102], key->w[103], key->k[100], key->k[101], key->k[102], key->k[103]);
  RND09(key->w[104], key->w[105], key->w[106], key->w[107], key->k[104], key->k[105], key->k[106], key->k[107]);
  RND08(key->w[108], key->w[109], key->w[110], key->w[111], key->k[108], key->k[109], key->k[110], key->k[111]);
  RND07(key->w[112], key->w[113], key->w[114], key->w[115], key->k[112], key->k[113], key->k[114], key->k[115]);
  RND06(key->w[116], key->w[117], key->w[118], key->w[119], key->k[116], key->k[117], key->k[118], key->k[119]);
  RND05(key->w[120], key->w[121], key->w[122], key->w[123], key->k[120], key->k[121], key->k[122], key->k[123]);
  RND04(key->w[124], key->w[125], key->w[126], key->w[127], key->k[124], key->k[125], key->k[126], key->k[127]);
  RND03(key->w[128], key->w[129], key->w[130], key->w[131], key->k[128], key->k[129], key->k[130], key->k[131]);

  for(i = 0; i <= 32; i++) { /* Make keys */
    for(j = 0; j < 4; j++)
      key->subkeys[i][j] = key->k[4 * i + j];
  }
}

void serpent_encrypt(SERPENT_CTX * ctx, const unsigned long * plaintext, unsigned long * ciphertext) {
  register unsigned long x0, x1, x2, x3;
  register unsigned long y0, y1, y2, y3;

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

void serpent_decrypt(SERPENT_CTX * ctx, const unsigned long * ciphertext, unsigned long * plaintext) {
  register unsigned long x0, x1, x2, x3;
  register unsigned long y0, y1, y2, y3;

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
