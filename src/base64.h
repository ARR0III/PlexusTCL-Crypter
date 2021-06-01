#ifndef BASE64_H
#define BASE64_H

#include "base64.c"

void base64encode(const uint8_t * input, uint8_t * output, const int size);
void base64decode(const uint8_t * input, uint8_t * output, const int size);

#endif
