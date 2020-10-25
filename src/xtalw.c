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
  uint64_t new_min = (uint64_t)min + rand();
  uint64_t new_max = (uint64_t)max - min + 1;
  
  return (int)(new_min % new_max);
}

/* "meminit" always upload in memory and executed */
void * meminit(void * data, const uint8_t simbol, size_t length) {

  if (NULL == data || 0 == length) {
    return data;
  }

  volatile uint8_t * temp = (uint8_t *)data;

  while (length) {
    *temp = simbol;

     --length;
     ++temp;
  }

  return data;
}

size_t __strnlen(const char * string, size_t length) {

  size_t result = 0;

  if (NULL == string) {
    return result;
  }

  while (length && ('\0' != (string[result]))) {
    ++result;
    --length;
  }

  return result;
}

int readfromfile(const char * filename, void * buffer, const size_t length) {

  if (NULL == filename || NULL == buffer || 0 == length) {
    return (-1);
  }

  FILE * f = fopen(filename, "rb");

  if (NULL == f) {
    return (-1);
  }

  int result = (int)fread(buffer, 1, length, f);
  fclose(f);

  return result;
}

void * strxor(uint8_t * output, const uint8_t * input, size_t length) {

  if (NULL == input || NULL == output || 0 == length) {
    return output;
  }

  const uint8_t * temp;

  for (size_t i = 0; i < length; ++i) {
    temp       = &(input[i]);
    output[i] ^=  (*temp);
  }
  return output;
}

size_t printhex(const int tumbler, const void * data, size_t length) {

  size_t i = 0;

  if (NULL == data || 0 == length) {
    return i;
  }

  const uint8_t * temp = (uint8_t *)data;

  if (tumbler == HEX_TABLE) {
    for (; i < length; ++i) {
      (void)printf("%02X%c", temp[i], (((i + 1) % 16) ? ' ' : '\n'));
    }
  }
  else
  if (tumbler == HEX_STRING) {
    for (; i < length; ++i) {
      (void)printf("%02X",  temp[i]);
    }
  }

  putc('\n', stdout);
  return i;
}
