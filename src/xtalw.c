/*
  Plexus Technology Cybernetic Laboratories;

  Some functions of this library are functional analogs of Windows system
  functions, so their replacement with the original functions should not
  affect the operation of the program in any way.
*/

#include <stdio.h>
#include <stdint.h>

#define HEX_TABLE  1
#define HEX_STRING 0

int genrand(const int min, const int max) {
  return (int)(min + rand() % ((max + 1) - min));
}

/* "meminit" always upload in memory and executed */
void meminit(void * data, const uint8_t simbol, size_t size) {

  if (data == NULL) {
    return;
  }

  volatile uint8_t * temp = (uint8_t *)data;

  while (size) {
    *temp = simbol;

     ++temp;
     --size;
  }
}

size_t __strnlen(const char * string, size_t length) {
  size_t result = 0;

  while (('\0' != string[result]) && length) {
    ++result;
    --length;
  }

  return result;
}

int readfromfile(const char * filename, void * buffer, const size_t length) {

  if (filename == NULL || buffer == NULL || length == 0) {
    return -1;
  }

  FILE * f = fopen(filename, "rb");

  if (f == NULL) {
    return -1;
  }

  int result = (int)fread(buffer, 1, length, f);
  fclose(f);

  return result;
}

void strxor(uint8_t * output, const uint8_t * input, size_t length) {

  if (output == NULL || input == NULL) {
    return;
  }

  uint8_t temp;

  for (size_t i = 0; i < length; ++i) {
    temp  = input[i];
    output[i] ^= temp;
  }
}

void printhex(const int32_t tumbler, const void * data, size_t length) {

  if (data == NULL || length == 0) {
    return;
  }

  if (tumbler == HEX_TABLE) {
    for (size_t i = 0; i < length; ++i) {
      printf("%02X%c", ((uint8_t *)data)[i], (((i + 1) % 16) ? ' ' : '\n'));
    }
  }
  else
  if (tumbler == HEX_STRING) {
    for (size_t i = 0; i < length; ++i) {
      printf("%02X", ((uint8_t *)data)[i]);
    }
  }
  else {
    return;
  }

  putc('\n', stdout);
}
