#include <string.h>

const uint32_t k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void to_bytes(uint32_t val, uint8_t *bytes) {
  bytes[0] = (uint8_t) val;
  bytes[1] = (uint8_t) (val >> 8);
  bytes[2] = (uint8_t) (val >> 16);
  bytes[3] = (uint8_t) (val >> 24);
}

uint32_t to_int32(const uint8_t *bytes) {
  return (uint32_t) bytes[0]
      | ((uint32_t) bytes[1] << 8)
      | ((uint32_t) bytes[2] << 16)
      | ((uint32_t) bytes[3] << 24);
}

void md5(const uint8_t * input, size_t input_len, uint8_t * output) {

  uint32_t w[16];
  size_t new_len, offset;
  uint32_t h0, h1, h2, h3;
  uint32_t a, b, c, d, i, f, g, temp;

  h0 = 0x67452301;
  h1 = 0xefcdab89;
  h2 = 0x98badcfe;
  h3 = 0x10325476;

  for (new_len = input_len + 1; new_len % (512/8) != 448/8; new_len++);

  uint8_t * msg = (uint8_t *)calloc(1, new_len + 8);

  if (msg != NULL) {
    memcpy(msg, input, input_len);
    msg[input_len] = 0x80;

    for (offset = input_len + 1; offset < new_len; offset++)
      msg[offset] = 0x00;

    to_bytes(input_len * 8, msg + new_len);
    to_bytes(input_len >> 29, msg + new_len + 4);

    for(offset = 0; offset < new_len; offset += (512 / 8)) {
      for (i = 0; i < 16; i++)
        w[i] = to_int32(msg + offset + i * 4);

        a = h0;
        b = h1;
        c = h2;
        d = h3;

        for (i = 0; i < 64; i++) {
          if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
          }
          else
          if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
          }
          else
          if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
          }
          else {
            f = c ^ (b | (~d));
            g = (7 * i) % 16;
          }

          temp = d;
          d = c;
          c = b;
          b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
          a = temp;
        }

      h0 += a;
      h1 += b;
      h2 += c;
      h3 += d;
    }

    free(msg);
    msg = NULL;

    to_bytes(h0, output);
    to_bytes(h1, output + 4);
    to_bytes(h2, output + 8);
    to_bytes(h3, output + 12);
  }
}