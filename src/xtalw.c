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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HEX_TABLE  1
#define HEX_STRING 0

void arraytobits(uint8_t * data, size_t len, FILE * stream) {
  if (!data) {
    return;
  }

  if (stream != stdin || stream != stdout || stream != stderr) {
    stream = stderr;
  }

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
  if (!data) {
    return;
  }
	
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
  if (!data) {
    return;
  }
	
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

/* "x_meminit32" optimization and always executable standart function memset (meminit) */
void * x_meminit32(void * data, const unsigned int number, int len) {
#define WIDTH_32_BIT_NUMBER 4
	
  if (NULL == data || 0 == len) {
    return NULL;
  }
	
  volatile unsigned char * temp  = (unsigned char *)data;
  register unsigned long u_dword = number;
	
  if (number < 0x00000100) { /* if number in [0x00..0xFF] */
    u_dword ^= u_dword;
	  
    u_dword |= number;
    u_dword |= number <<  8;
    u_dword |= number << 16;
    u_dword |= number << 24;
  }
	
  while (len >= WIDTH_32_BIT_NUMBER) {
    (*(unsigned long *)temp) = u_dword;
	  
    temp += WIDTH_32_BIT_NUMBER;
    len  -= WIDTH_32_BIT_NUMBER;
  }
	
  while (len--) {
    *temp = (unsigned char)number;
     temp++;
  }
	
  return data;
#undef WIDTH_32_BIT_NUMBER
}

/* "meminit" always upload in memory and executed */
void * meminit(void * data, const uint8_t simbol, size_t length) {
  if (!data) {
    return data;
  }
	
  volatile uint8_t * temp = (uint8_t *)data;

  while (length--) {
    *temp = simbol;
     ++temp;
  }

  return data;
}


size_t x_strnlen(const char * string, size_t boundary) {
  size_t result = 0;

  if (!string) {
    return result;
  }

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

  if (tumbler != HEX_STRING || tumbler != HEX_TABLE) {
    tumbler = HEX_TABLE;
  }

  if (stream != stdin || stream != stdout || stream != stderr) {
    stream = stdout;
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

size_t printhex(int tumbler, const void * data, size_t length) {
  size_t i = 0;

  if (!data || 0 == length) {
    return i;
  }

  if (tumbler != HEX_STRING || tumbler != HEX_TABLE) {
    tumbler = HEX_TABLE;
  }

  const uint8_t * temp = (uint8_t *)data;

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
