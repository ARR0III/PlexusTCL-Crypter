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

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*
 WARNING: ASSEMBLER CODE ONLY FOR C++ BUILDER 6.0
 DO NOT CHANGE ASSEMBLER CODE IF YOU WOODPECKER!
 
 1 - FOR C++ BUILDER 6.0
 0 - OTHER COMPILERS
 
 If defined __ASM_32_X86_CPP_BUILDER__ const equal 1 (ON)
 then Borland C++ 6.0 compiling assembler x86 code for
 maximization program speed on 32 bits processor!
*/

#define ON  1
#define OFF 0

#define HEX_TABLE  1
#define HEX_STRING 0

#ifndef __ASM_32_X86_CPP_BUILDER__
#define __ASM_32_X86_CPP_BUILDER__ ON

#define BIG_ENDING    1
#define LITTLE_ENDING 0

#ifdef __cplusplus
extern "C" {
#endif

/* 0 = LITTLE */
/* 1 = BIG    */
size_t little_or_big_ending(void);

/* C code + Assembly code */
void * strxormove(void * output, const void * input, size_t length);
void * meminit(void * data, const size_t number, const unsigned int length);
void * strxor(uint8_t * output, const uint8_t * input, size_t length);

size_t x_strnlen(const char * string, size_t boundary);

/* C code */
void   strinc(uint8_t * data, size_t length);
void   strdec(uint8_t * data, size_t length);
void   arraytobits(const uint8_t * data, const size_t length, FILE * stream);
void   phex(int tumbler, const uint8_t * data, size_t length, FILE * stream);
size_t printhex(const int tumbler, const void * data, size_t length);

int genrand(const int min, const int max);
int readfromfile(const char * filename, void * buffer, const size_t length);

#ifdef __cplusplus
}
#endif

#endif /* __ASM_32_X86_CPP_BUILDER__ */

#endif /* XTALW_H */
