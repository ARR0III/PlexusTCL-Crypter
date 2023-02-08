#ifndef BASE64_H
#define BASE64_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void base64encode(const uint8_t * input, uint8_t * output, const int size);
void base64decode(const uint8_t * input, uint8_t * output, const int size);

#ifdef __cplusplus
}
#endif

#endif
