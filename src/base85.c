#include <stdint.h>

#define base85_code(c) (base85_codes[(c) & 0x7f])

const char * base85_chars =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
    "ghijklmnopqrstuvwxyz$%&()*+-:;<=>@[]^_`{|}~";

const signed char base85_codes[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, 62, 63, 64, -1, 65, 66, 67, 68, -1, 69, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 70, 71, 72, 73, 74, -1,
    75, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 76, -1, 77, 78, 79,
    80, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 81, 82, 83, 84, -1};

const uint32_t pow85[5] = {1, 85, 7225,
                           614125, 52200625};

int32_t base85_encode_len(int32_t len) {
  int32_t n = (len / 4) * 5 + ((len % 4 > 0) ? (1 + len % 4) : 0);

  if ((len < 0) || ((n / 5) != (n / 4))) {
    return -1;
  }

  return n;
}

int32_t base85_decode_len(int32_t len) {
  if (len < 0) {
    return -1;
  }

  return ((len / 5) * 4 + ((len % 5 > 1) ? (len % 5 - 1) : 0));
}

int32_t base85_real_decode_len(char *buf, int32_t len) {
  int32_t n = 0;

  while (len-- > 0) {
    n += (base85_code(*buf++) >= 0);
  }

  return base85_decode_len(n);
}

int32_t base85_encode(const uint8_t * input, int32_t inlen, uint8_t * output) {

  if (NULL == input || NULL == output || 0 == inlen) {
    return -1;
  }
  
  int32_t  n, m;
  uint32_t word, temp_len;

  const uint8_t * in  = input;
        uint8_t * out = output;

  temp_len = inlen / 4;

  for (n = 0; n < temp_len; ++n) {
    word = ((uint32_t)in[0] <<  0) | ((uint32_t)in[1] <<  8) |
           ((uint32_t)in[2] << 16) | ((uint32_t)in[3] << 24);

    in += 4;

    for (m = 0; m < 5; ++m) {
      *out++ = base85_chars[word % 85];
      word  /= 85;
    }
  }

  temp_len = inlen % 4;

  if (temp_len > 0) {
    word = 0;

    for (n = 0; n < temp_len; ++n)
      word |= (uint32_t)in[n] << 8 * n;

    for (n = 0; n < (temp_len + 1); ++n) {
      *out++ = base85_chars[word % 85];
       word /= 85;
    }
  }

  return (out - output);
}

int32_t base85_decode(const uint8_t * input, int32_t inlen, uint8_t * output) {

  if (NULL == input || NULL == output || 0 == inlen) {
    return -1;
  }

  int32_t  n, m, c;
  uint32_t word;

  const uint8_t * in  = input;
        uint8_t * out = output;

  m    = 0;
  word = 0;

  while (inlen-- > 0) {
    if (0x20 == *in || '\n' == *in || '\t' == *in) {
      in++;
      continue;
    }

    if ((c = base85_code((*in++) & 0x7f)) >= 0) {
      word += pow85[m++] * c;
      if (m == 5) {
        for (m = 0; m < 4; ++m) {
          *out++ = word & 255;
          word >>= 8;
        }
        m    = 0;
        word = 0;
      }
    }
  }

  for (n = 0; n < (m - 1); ++n) {
    *out++ = word & 255;
    word >>= 8;
  }

  return (out - output);
}
