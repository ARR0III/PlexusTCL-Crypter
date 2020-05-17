#include <stdio.h>
#include <stdint.h>

#define HEX_TABLE  1
#define HEX_STRING 0

int genrand(const int min, const int max) {
  return min + rand() % ((max + 1) - min);
}

void vigenere (uint8_t * data, const int data_len, const uint8_t * key, const int key_len) {
  int i, j;
  uint8_t temp;
  
  for (i = 0, j = 0; i < data_len; i++) {
    temp = key[j];
    data[i] ^= temp;
    
    ++j;
    
    if (j == key_len)
      j = 0;
  }
}

int readfromfile(const char * filename, uint8_t * buffer, const int length) {
  FILE * f = fopen(filename, "rb");

  if (f == NULL)
    return -1;
  
  int result = (int)fread(buffer, 1, length, f);
  fclose(f);
  
  return result;
}

void strxor (uint8_t * one, const uint8_t * two, int length) {
  uint8_t temp;
  for (int i = 0; i < length; i++) {
    temp = two[i];
    one[i] ^= temp;
  }
}

void printhex(const int tumbler, const int t, const uint8_t * data, const int length) {
  for (int i = 0; i < length; i++) {
    if (tumbler)
      printf("%02X%c", data[i], (i + 1) % t ? ' ' : '\n');
    else
      printf("%02X", data[i]);
  }
  putc('\n', stdout);
}