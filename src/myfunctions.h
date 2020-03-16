#include "myfunctions.c"

int  genrand(const int min, const int max);
void vigenere (uint8_t * data, const int length_data, const uint8_t * key, const int length_key);
int  readfromfile(const char * filename, uint8_t * buffer, const int length);
void strxor (uint8_t * one, const uint8_t * two, const int length);
void hexprint(const int tumbler, const uint8_t * data, const int length);