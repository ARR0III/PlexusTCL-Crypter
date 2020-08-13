#include <stdio.h>
#include <stdint.h>

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

  for (size_t i = 0; i < length; i++) {
    temp = two[i];
    one[i] ^= temp;
  }
}

void printhex(const int tumbler, const uint8_t * data, const size_t length) {
  size_t i;

  if (tumbler == HEX_TABLE) {
    for (i = 0; i < length; i++)
      printf("%02X%c", data[i], (((i + 1) % 16) ? ' ' : '\n'));
  }
  else
  if (tumbler == HEX_STRING) {
    for (i = 0; i < length; i++)
      printf("%02X", data[i]);
  }
  else {
    printf("Parameter \"%d\" not correct!\n", tumbler);
    return;
  }

  putc('\n', stdout);
}
