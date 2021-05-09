/*
  Plexus Technology Cybernetic Laboratories;

  Some functions of this library are functional analogs of system
  functions, so their replacement with the original functions should not
  affect the operation of the program in any way.
*/

#include <stdio.h>
#include <stdint.h>

#define HEX_TABLE  1
#define HEX_STRING 0

int genrand(const int min, const int max) {
  return (int)(rand() % (max - min + 1) + min);
}

/* "meminit" always upload in memory and executed */
void * meminit(void * data, const uint8_t simbol, size_t length) {
  volatile uint8_t * temp = (uint8_t *)data;

  if (NULL == data) {
    return data;
  }

  while (length--) {
    *temp = simbol;
     temp++;
  }

  return data;
}

size_t __strnlen(const char * string, size_t length) {

  size_t result = 0;

  if (NULL != string) {
    while (0 != length) {
      if ('\0' == string[result] || '\n' == string[result] ||
          '\t' == string[result] || '\r' == string[result] ||
          '\v' == string[result] || '\a' == string[result] ||
          '\?' == string[result] || '\\' == string[result] ||
          '\'' == string[result] || '\"' == string[result] ) {
      
        break;
      }
      result++;
      length--;
    }
  }
  return result;
}

int readfromfile(const char * filename, void * buffer, const size_t length) {

  FILE * f;
  int result;

  if (NULL == filename || NULL == buffer || 0 == length) {
    return (-1);
  }

  f = fopen(filename, "rb");

  if (NULL == f) {
    return (-1);
  }

  result = (int)fread(buffer, 1, length, f);
  fclose(f);

  return result;
}

void * strxor(uint8_t * output, const uint8_t * input, size_t length) {

  const uint8_t * temp;
  size_t i;

  if (NULL == input || NULL == output || 0 == length) {
    return output;
  }

  temp = input;

  for (i = 0; i < length; ++i) {
    output[i] ^= *temp;
    temp++;
  }
  
  return output;
}

size_t printhex(const int tumbler, const void * data, size_t length) {

  size_t i = 0;
  const uint8_t * temp;

  if (NULL == data || 0 == length) {
    return i;
  }

  temp = (uint8_t *)data;

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
