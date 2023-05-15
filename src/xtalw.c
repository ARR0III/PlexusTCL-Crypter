/*
 * Plexus Technology Cybernetic Laboratory;
 *
 * This library of functions was created as a set of functions
 * that I need to replace some of the functions in the C
 * programming language standard library that I think could be
 * better implemented. Some functions are needed for debugging
 * programs, such as "phex", which prints the contents of memory
 * in hexadecimal. The functions were debugged, tested and
 * implemented into the program just in case.
 */

#include "xtalw.h"

size_t little_or_big_ending(void) {
  unsigned short x = 0x00FF;

  return (*((unsigned short *)&x) == 0 ? 1 : 0);
}

void select_sort(size_t * data, const size_t size) {
  size_t j, i = 0;
  size_t min, tmp, pos;

  while (i < (size - 1)) {
    min = data[i];
    j   = i + 1;
    pos = 0;

    while (j < size) {     /* search minimal number in static array */
      if (data[j] < min) {
        min = data[j];
        pos = j;
      }

      j++;
    }

    if (pos > i) {
      tmp       = data[i];
      data[i]   = data[pos];
      data[pos] = tmp;
    }

    if (i == (size - 2)) return;

    i++;
  }
}

void * memxor(void * output, const void * input, size_t length) {

        uint8_t * local_output = (uint8_t *)output;
  const uint8_t * local_input  = (const uint8_t *)input;

        uint8_t * last_output = local_output + (length - 1);
  const uint8_t * last_input  = local_input  + (length - 1);

  if ((NULL == output) || (NULL == input) || (output == input) || (0 == length)) {
    return NULL;
  }

  if (local_output < local_input) {
    while(length--) {
      *local_output ^= *local_input;

       local_output++;
       local_input++;
    }
  }
  else {
    while(length--) {
      *last_output ^= *last_input;

       last_output--;
       last_input--;
    }
  }

  return output;
}

void * memxormove(void * output, const void * input, size_t length) {
  const size_t width_register = sizeof(size_t);

        uint8_t * local_output = (uint8_t *)output;
  const uint8_t * local_input  = (const uint8_t *)input;

        uint8_t * last_output = local_output + (length - 1);
  const uint8_t * last_input  = local_input  + (length - 1);

  if ((NULL == output) || (NULL == input) || (output == input) || (0 == length)) {
    return NULL;
  }

  if (local_output < local_input) {
    if (((size_t)local_input - (size_t)local_output) < width_register) {
      return memxor(output, input, length);
    }

    while (length >= width_register) {
      (*(size_t *)local_output) ^= (*(size_t *)local_input);

      local_output += width_register;
      local_input  += width_register;

      length -= width_register;
    }

    while (length) {
      *local_output ^= *local_input;

      local_output++;
      local_input++;

      length--;
    }
  }
  else {
    if (((size_t)last_output - (size_t)last_input) < width_register) {
      return memxor(output, input, length);
    }

    while (length && (length % width_register)) { /* max 7 iteration */
      *last_output ^= *last_input;

      last_output--;
      last_input--;

      length--;
    }

    /* Align pointers to 32 bits for read from end! */
    last_output -= (width_register - 1);
    last_input  -= (width_register - 1);

    while (length) {
      (*(size_t *)last_output) ^= (*(size_t *)last_input);

      last_output -= width_register;
      last_input  -= width_register;

      length -= width_register;
    }
  }

  return output;
}

/* "meminit32" optimization and always executable standart function memset */
void * meminit(void * data, const size_t number, size_t length) {
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov eax, data
  cmp eax, 0
  je _exit
  
  mov ecx, length
  cmp ecx, 0
  je _exit

  mov edx, number
  cmp edx, 0xFF
  ja _memset_x32

  mov ebx, edx
  shl ebx, 8
  or edx, ebx
  or ebx, edx
  shl ebx, 16
  or edx, ebx

_memset_x32:
  cmp ecx, 4
  jb _memset_x8
  mov dword [eax], edx
  add eax, 4
  sub ecx, 4
  jmp _memset_x32

_memset_x8:
  cmp ecx, 0
  je _exit
  mov byte [eax], dl
  inc eax
  dec ecx
  jmp _memset_x8
  
_exit:
}
#else
  const size_t width_register = sizeof(size_t);

  volatile uint8_t * temp  = (uint8_t *)data;
  register size_t u_dword = number;

  if ((NULL == data) || (0 == length)) {
    return NULL;
  }

  if (u_dword < 0x100) {
    u_dword |= u_dword <<  8;
    u_dword |= u_dword << 16;
  }
	
  while (length >= width_register) {
    (*(size_t *)temp) = u_dword;
	  
    temp   += width_register;
    length -= width_register;
  }
	
  while (length) {
    *temp = (uint8_t)u_dword;
     temp++;
     length--;
  }
#endif
  return data;
}

size_t x_strnlen(const char * s, size_t b) { /* DEBUG = OK */
  size_t r = 0;
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov eax, r
  mov ecx, b
  
  mov edx, s
  cmp edx, 0
  je _exit         /* check NULL pointer */
  
_strnlen:
  cmp ecx, 0
  je _exit
  cmp byte[edx], 0
  je _exit
  inc edx          /* str++ */
  inc eax          /* result++ */
  dec ecx          /* bounder-- */
  jmp _strnlen  

_exit:
  mov r, eax
}
#else
  if (s) {
    while (b && s[r]) {
      b--;
      r++;
    }
  }
#endif
  return r;
}

void arraytobits(const uint8_t * data, const size_t length, FILE * stream) {
  uint8_t c;
  size_t j;
  int k;

  if ((NULL == data) || (0 == length)) {
    return;
  }

  if (stream != stdin || stream != stdout || stream != stderr) {
    stream = stderr;
  }

  for (j = 0; j < length; j++) {
    c = data[j];

    for(k = 7; k >= 0; --k) {
      putc((48 + ((c >> k) & 0x01)), stream);
    }
  }

  putc('\n', stream);
}

void strinc(uint8_t * data, size_t len) {
  if (NULL == data) {
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
  if (NULL == data) {
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

int readfromfile(const char * filename, void * buffer, const size_t length) {

  FILE * f;
  int result;

  if ((NULL == filename) || (NULL == buffer) || (0 == length)) {
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

void phex(int tumbler, const uint8_t * data, size_t length, FILE * stream) {
  size_t i;
  int left, right, symbol;
  const char digits[] = "0123456789ABCDEF";

  if ((NULL == data) || (0 == length)) {
    return;
  }

  if (tumbler != HEX_STRING || tumbler != HEX_TABLE) {
    tumbler = HEX_TABLE;
  }

  if (stream != stdin || stream != stdout || stream != stderr) {
    stream = stderr;
  }

  for (i = 0; i < length; ++i) {
    symbol = (int)data[i];

    left  = symbol >> 0x04; /* 11000011 >> 0x04 = 00001100 */
    right = symbol  & 0x0F; /* 11000011  & 0x0F = 00000011 */

    putc(digits[left],  stream);
    putc(digits[right], stream);

    if (HEX_TABLE == tumbler) {
      putc(((i + 1) % 16) ? ' ' : '\n', stream);
    }
  }

  putc('\n', stream);
}

size_t printhex(int tumbler, const void * data, size_t length) {
  size_t i = 0;
  const uint8_t * temp = (uint8_t *)data;

  if ((NULL == data) || (0 == length)) {
    return i;
  }

  if (tumbler != HEX_STRING || tumbler != HEX_TABLE) {
    tumbler = HEX_TABLE;
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

  putc('\n', stdout);

  return i;
}
