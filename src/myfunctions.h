#include "myfunctions.c"

int  genrand(int min, int max);
void vigenere (uint8_t * data, int length_data, const uint8_t * key, int length_key);
int  readfromfile(const uint8_t * filename, uint8_t * buffer, int length);
void strxor (uint8_t * one, const uint8_t * two, int length);
void hexprint(int tumbler, const uint8_t * data, int length);