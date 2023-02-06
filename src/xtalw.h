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

/*
 WARNING: ASSEMBLER CODE ONLY FOR C++ BUILDER 6.0
 DO NOT CHANGE ASSEMBLER CODE IF YOU WOODPECKER!
 
 If defined __ASM_32_X86_CPP_BUILDER__ const equal 1 (TRUE)
 then Borland C++ 6.0 compiling assembler langiage x86 code
 for maximization program speed!
*/
#ifndef __ASM_32_X86_CPP_BUILDER__
#define __ASM_32_X86_CPP_BUILDER__ 1

#ifdef __cplusplus
extern "C" {
#endif

#include "xtalw.c"

void * meminit(void * data, const unsigned int number, const unsigned int len);
  
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

#endif /* __ASM_32_X86_CPP_BUILDER__ */

#endif /* XTALW_H */
