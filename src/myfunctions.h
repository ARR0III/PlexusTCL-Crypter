#include "myfunctions.c"

int    genrand(const int min, const int max);
size_t readfromfile(const char * filename, uint8_t * buffer, const size_t length);
void   strxor(uint8_t * one, const uint8_t * two, const size_t length);
void   printhex(const int tumbler, const uint8_t * data, const size_t length);
