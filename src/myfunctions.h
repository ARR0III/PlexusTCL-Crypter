#include "myfunctions.c"

int  genrand(const int min, const int max);
int  readfromfile(const char * filename, uint8_t * buffer, const int length);
void strxor(uint8_t * one, const uint8_t * two, const int length);
void printhex(const int tumbler, const uint8_t * data, const int length);
