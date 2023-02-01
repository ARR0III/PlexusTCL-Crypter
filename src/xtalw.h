/*
  Plexus Technology Cybernetic Laboratory;

  This library of functions was created as a set of functions
  that I need to replace some of the functions in the C
  programming language standard library that I think could be
  better implemented. Some functions are needed for debugging
  programs, such as "phex", which prints the contents of memory
  in hexadecimal. The functions were debugged, tested and
  implemented into the program just in case.
*/

#ifndef XTALW_H
#define XTALW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xtalw.c"

void * x_meminit32(void * data, const unsigned int number, int len);
void * meminit(void * data, const uint8_t simbol, size_t length);

void * strxormove(void * output, const void * input, size_t length);
void * strxor(uint8_t * output, const uint8_t * input, size_t length);

void strinc(uint8_t * data, size_t len);
void strdec(uint8_t * data, size_t len);
void arraytobits(uint8_t * data, size_t len, FILE * stream);
void phex(int tumbler, const uint8_t * data, size_t length, FILE * stream);

size_t x_strnlen(const char * string, size_t boundary);
size_t printhex(const int tumbler, const void * data, size_t length);

int readfromfile(const char * filename, void * buffer, const size_t length);

#ifdef __cplusplus
}
#endif

#endif
