/*
  Plexus Technology Cybernetic Laboratories;

  Some functions of this library are functional analogs of system
  functions, so their replacement with the original functions should not
  affect the operation of the program in any way.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HEX_TABLE  1
#define HEX_STRING 0

void chartobits(uint8_t * data, size_t len, FILE * stream) {
  if (!data)
    return;

  for (size_t j = 0; j < len; j++) {
    uint8_t c = data[j];

    for(int k = 7; k >= 0; --k) {
      putc((48 + ((c >> k) & 0x01)), stream);
    }
  }

  if (len > 0)
    putc('\n', stream);
}

void strinc(uint8_t * data, size_t len) {
  while(--len) {
    if (0xFF == data[len]) {
      data[len] = 0x00;
      continue;
    }
    else {
      data[len] += 1;
      break;
    }
  }
}

void strdec(uint8_t * data, size_t len) {
  while(--len) {
    if (0x00 == data[len]) {
      data[len] = 0xFF;
      continue;
    }
    else {
      data[len] -= 1;
      break;
    }
  }
}

int genrand(const int min, const int max) {
  return (int)(rand() % (max - min + 1) + min);
}

/* "meminit" always upload in memory and executed */
void * meminit(void * data, const uint8_t simbol, size_t length) {
  volatile uint8_t * temp = (uint8_t *)data;

  if (!data) {
    return data;
  }

  while (length--) {
    *temp = simbol;
     temp++;
  }

  return data;
}


size_t x_strnlen(const char * string, size_t boundary) {

  size_t result = 0;

  while (boundary && string[result]) {
    boundary--;
    result++;
  }
  
  return result;
}

int readfromfile(const char * filename, void * buffer, const size_t length) {

  FILE * f;
  int result;

  if (!filename || !buffer || 0 == length) {
    return (-1);
  }

  f = fopen(filename, "rb");

  if (!f) {
    return (-1);
  }

  result = (int)fread(buffer, 1, length, f);
  fclose(f);

  return result;
}

void * strxormove(void * output, const void * input, size_t length) {
  if (!output || !input || (output == input)) {
    return output;
  }

        unsigned char * local_output = (unsigned char *)output;
  const unsigned char * local_input  = (const unsigned char *)input;

  if (local_output < local_input) {
    while(length--) {
      *local_output ^= *local_input;

      ++local_output;
      ++local_input;
    }
  }
  else {
          unsigned char * last_output = local_output + (length - 1);
    const unsigned char * last_input  = local_input  + (length - 1);
     
     while (length--) {
       *last_output ^= *last_input;

       --last_output;
       --last_input;
     }
  }

  return output;
}

void * strxor(uint8_t * output, const uint8_t * input, size_t length) {

  if (!input || !output || (input == output)) {
    return output;
  }

        uint8_t * local_output = output;
  const uint8_t * local_input  = input;

  while (length--) {
    *local_output ^= *local_input;
    
    local_output++;
    local_input++;
  }
  
  return output;
}

void phex(int tumbler, const uint8_t * data, size_t length, FILE * stream) {

  if (!data) {
    return;
  }

  if (stream != stdin || stream != stdout || stream != stderr) {
    stream = stderr;
  }

  int left, right, symbol;
  const char digits[] = "0123456789ABCDEF";

  for (size_t i = 0; i < length; ++i) {
    symbol = (int)data[i];

    left  = symbol >> 0x04; /* 11000011 >> 0x04 = 00001100 */
    right = symbol  & 0x0F; /* 11000011  & 0x0F = 00000011 */

    putc(digits[left],  stream);
    putc(digits[right], stream);

    if (HEX_TABLE == tumbler) {
      putc(((i + 1) % 16) ? ' ' : '\n', stream);
    }
  }

  if (length > 0)
    putc('\n', stream);
}

size_t printhex(const int tumbler, const void * data, size_t length) {

  size_t i = 0;
  const uint8_t * temp = (uint8_t *)data;

  if (!data || 0 == length) {
    return i;
  }

  if (HEX_TABLE == tumbler) {
    for (i = 0; i < length; ++i) {
      (void)printf("%02X%c", temp[i], (((i + 1) % 16) ? ' ' : '\n'));
    }
  }
  else
  if (HEX_STRING == tumbler) {
    for (; i < length; ++i) {
      (void)printf("%02X",  temp[i]);
    }
  }

  if (length > 0)
    putc('\n', stdout);

  return i;
}
