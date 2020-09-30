/*
  Plexus Technology Cybernetic Laboratories;

  Some functions of this library are functional analogs of Windows system
  functions, so their replacement with the original functions should not
  affect the operation of the program in any way.
*/

#include "xtalw.c"

int    genrand(const int min, const int max);
int    readfromfile(const char * filename, void * buffer, const size_t length);

size_t printhex(const int tumbler, const void * data, size_t length);
size_t __strnlen(const char * string, size_t length);

void * meminit(void * data, const uint8_t simbol, size_t length);
void * strxor(uint8_t * output, const uint8_t * input, size_t length);
