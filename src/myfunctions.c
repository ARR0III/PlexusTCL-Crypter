#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HEX_TABLE  1
#define HEX_STRING 0

int genrand(const int min, const int max) {
  return min + rand() % ((max + 1) - min);
}

int readfromfile(const char * filename, uint8_t * buffer, const size_t length) {
  FILE * f = fopen(filename, "rb");

  if (f == NULL)
    return -1;

  int result = (int)fread(buffer, 1, length, f);
  fclose(f);

  return result;
}

void strxor (uint8_t * one, const uint8_t * two, const size_t length) {
  uint8_t temp;

  for (int i = 0; i < length; i++) {
    temp = two[i];
    one[i] ^= temp;
  }
}

void printhex(const int tumbler, const uint8_t * data, const size_t length) {
  for (int i = 0; i < length; i++) {
    if (tumbler == HEX_TABLE)
      printf("%02X%c", data[i], (((i + 1) % 16) ? ' ' : '\n'));
    else
      printf("%02X", data[i]);
  }
  putc('\n', stdout);
}
