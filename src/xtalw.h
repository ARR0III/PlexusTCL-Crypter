/*
  Plexus Technology Cybernetic Laboratories;

  Some functions of this library are functional analogs of Windows system
  functions, so their replacement with the original functions should not
  affect the operation of the program in any way.
*/

#ifndef XTALW_H
#define XTALW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xtalw.c"

void   chartobits(uint8_t * data, int len, FILE * stream);

void   strinc(uint8_t * data, int len);
void   strdec(uint8_t * data, int len);

int    genrand(const int min, const int max);
int    readfromfile(const char * filename, void * buffer, const size_t length);

void   phex(int tumbler, const uint8_t * data, size_t length, FILE * stream);

size_t printhex(const int tumbler, const void * data, size_t length);
size_t x_strnlen(const char * string, size_t length);

void * meminit(void * data, const uint8_t simbol, size_t length);
void * strxormove(void * output, const void * input, size_t length);
void * strxor(uint8_t * output, const uint8_t * input, size_t length);

#ifdef __cplusplus
}
#endif

#endif
