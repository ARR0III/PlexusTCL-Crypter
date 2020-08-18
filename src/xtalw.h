/*
  Plexus Technology Cybernetic Laboratories;
  
  Some functions of this library are functional analogs of Windows system
  functions, so their replacement with the original functions should not
  affect the operation of the program in any way.
*/

#include "xtalw.c"

int    genrand(const int min, const int max);
size_t __strnlen(const char * string, size_t length);
void   meminit(void * data, uint8_t simbol, size_t size);
void   strxor (uint8_t * one, const uint8_t * two, const size_t length);
int    readfromfile(const char * filename, void * buffer, const size_t length);
void   printhex(const int32_t tumbler, const void * data, const size_t length);
