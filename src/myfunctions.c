#include <stdio.h>
#include <stdint.h>

int genrand(int min, int max) {
  return min + rand() % ((max + 1) - min);
}

void vigenere (uint8_t * data, int length_data, const uint8_t * key, int length_key) {
  int i, j;
  
  for (i = 0, j = 0; i < length_data; i++) {
    data[i] ^= key[j];
    j = (j == length_key) ? 0 : ++j;
  }
}

int readfromfile(const uint8_t * filename, uint8_t * buffer, int length) {
  FILE * f = fopen(filename, "rb");

  if (f == NULL)
    return -1;
  
  int result = (int)fread(buffer, 1, length, f);
  fclose(f);
  
  return result;
}

void strxor (uint8_t * one, const uint8_t * two, int length) {
  for (int i = 0; i < length; i++) {
    one[i] ^= two[i];
  }
}

void hexprint(int tumbler, const uint8_t * data, int length) {
  for (int i = 0; i < length; i++) {
    switch (tumbler) {
      case 0: printf("%2.2X", *(data + i));
              break;
      case 1: printf("%2.2X%c", *(data + i), (i + 1) % 16 ? ' ' : '\n');
              break;
    }
  }
  
  if (tumbler == 0)
    putc('\n', stdout);
}