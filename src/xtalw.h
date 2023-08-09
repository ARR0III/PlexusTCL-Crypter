/*
  Plexus Technology Cybernetic Laboratory;
  
  Developer:  ARR0III;
  License:    GNU GPL version 3;

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
 DO NOT CHANGE ASSEMBLER CODE IF YOU ARE WOODPECKER!
 
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
#define __ASM_32_X86_CPP_BUILDER__ OFF

#ifdef __cplusplus
extern "C" {
#endif

/* C language source code + assembler source code for Intel x86 */
void memxor(void * output, const void * input, size_t length);
void memxormove(void * output, const void * input, size_t length);
void meminit(void * data, const size_t number, size_t length); /* <-- memset function analog */

/* Only C language source code */
void arraytobits(const uint8_t * data, const size_t length, FILE * stream);

void strinc(uint8_t * data, size_t len);
void strdec(uint8_t * data, size_t len);

void phex(int tumbler, const uint8_t * data, size_t length, FILE * stream);

int  genrand(const int min, const int max);
int  readfromfile(const char * filename, void * buffer, const size_t length);

size_t x_strnlen(const char * s, size_t b);
size_t printhex(int tumbler, const void * data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* __ASM_32_X86_CPP_BUILDER__ */

#endif /* XTALW_H */
