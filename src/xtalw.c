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

void memxor(void * output, const void * input, size_t length) { /* DEBUG = OK */
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov esi, input
  mov edi, output
  mov ecx, length
  mov ebx, 4

  test esi, edi
  jz _exit

  cmp edi, esi
  jb _memxor_16_normal /* if edi < esi */
/****************************************************************************/
  add esi, ecx
  add edi, ecx
  dec esi
  dec edi
  
_memxor_byte_reverse:
  test ecx, ecx
  jz _exit
  mov eax, ecx
  xor edx, edx
  div ebx
  test edx, edx
  jz _memxor_16_reverse_normal
  mov al, byte [esi]
  xor byte [edi], al
  dec esi
  dec edi
  dec ecx
  jmp _memxor_byte_reverse  
  
_memxor_16_reverse_normal:
  sub esi, 3
  sub edi, 3
  
_memxor_16_reverse:
  cmp ecx, 16
  jb _memxor_4_reverse
  
  mov eax, dword [esi]
  xor dword [edi], eax
  mov eax, dword [esi-4]
  xor dword [edi-4], eax
  mov eax, dword [esi-8]
  xor dword [edi-8], eax
  mov eax, dword [esi-12]
  xor dword [edi-12], eax

  sub esi, 16
  sub edi, 16
  sub ecx, 16
  jmp _memxor_16_reverse

_memxor_4_reverse:
  test ecx, ecx
  jz _exit
  mov eax, dword [esi]
  xor dword [edi], eax
  sub esi, 4
  sub edi, 4
  sub ecx, 4
  jmp _memxor_4_reverse
/****************************************************************************/  
_memxor_16_normal:
  mov ebx, esi
  sub ebx, edi
  cmp ebx, 4
  jb _memxor_byte
  
_memxor_16:
  cmp ecx, 16
  jb _memxor_4
  
  mov eax, dword [esi]
  xor dword [edi], eax
  mov eax, dword [esi+4]
  xor dword [edi+4], eax
  mov eax, dword [esi+8]
  xor dword [edi+8], eax
  mov eax, dword [esi+12]
  xor dword [edi+12], eax

  add esi, 16
  add edi, 16
  sub ecx, 16
  jmp _memxor_16

_memxor_4:
  cmp ecx, 4
  jb _memxor_byte
  mov eax, dword [esi]
  xor dword [edi], eax
  add esi, 4
  add edi, 4
  sub ecx, 4
  jmp _memxor_4

_memxor_byte:
  test ecx, ecx
  jz _exit
  mov al, byte [esi]
  xor byte [edi], al
  inc esi
  inc edi
  dec ecx
  jmp _memxor_byte

_exit:
}
#else
  if (!output || !input || output == input || 0 == length) {
    return;
  }

  if ((output + length) < output || (input + length) < input) /* IF POINTERS OWERFLOW */
    return;

  if (output < input) {
    while(length) {
      *((uint8_t*)output) ^= *((uint8_t*)input);

       output++;
       input++;
       length--;
    }
  }
  else {
    while(length) {
      *((uint8_t*)output) ^= *((uint8_t*)input);

       output--;
       input--;
       length--;
    }
  }
#endif
}

void memxormove(void * output, const void * input, size_t length) { /* DEBUG = OK */
  if (!output || !input || output == input || 0 == length) {
    return;
  }

  if (output < input) {
    if ((output + length) < output || (input + length) < input) /* IF POINTERS OWERFLOW */
      return;

    if (((size_t)input - (size_t)output) < sizeof(size_t)) {
      memxor(output, input, length);
      return;
    }

    while (length >= (sizeof(size_t) * 4)) {
      *((size_t *)output + 0) ^= *((size_t *)input + 0);
      *((size_t *)output + 1) ^= *((size_t *)input + 1);
      *((size_t *)output + 2) ^= *((size_t *)input + 2);
      *((size_t *)output + 3) ^= *((size_t *)input + 3);

      output += (sizeof(size_t) * 4);
      input  += (sizeof(size_t) * 4);

      length -= (sizeof(size_t) * 4);
    }

    while (length) {
      *((uint8_t*)output) ^= *((uint8_t*)input);

      output++;
      input++;

      length--;
    }
  }
  else {
    if ((output - length) > output || (input - length) > input) /* IF POINTERS OWERFLOW */
      return;

    if (((size_t)output - (size_t)input) < sizeof(size_t)) {
      memxor(output, input, length);
      return;
    }

    output += (length - 1);
    input  += (length - 1);

    while (length) {
      if (0 == length % sizeof(size_t)) /* max 3 or 7 iteration */
        break;

      *(uint8_t *)output ^= *(uint8_t *)input;

      output--;
      input--;

      length--;
    }

    /* Align pointers to 32 or 64 bits for read from end! */
    output -= (sizeof(size_t) - 1);
    input  -= (sizeof(size_t) - 1);

    while (length >= (sizeof(size_t) * 4)) {
      *((size_t *)output - 0) ^= *((size_t *)input - 0);
      *((size_t *)output - 1) ^= *((size_t *)input - 1);
      *((size_t *)output - 2) ^= *((size_t *)input - 2);
      *((size_t *)output - 3) ^= *((size_t *)input - 3);

      output -= (sizeof(size_t) * 4);
      input  -= (sizeof(size_t) * 4);
      length -= (sizeof(size_t) * 4);
    }
  }
}

/* "meminit32" optimization and always executable standart function memset */
void meminit(void * data, const size_t number, size_t length) { /* DEBUG = OK */
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov ecx, length
  mov edx, number
  mov eax, data
  
  cmp edx, 0xFF
  ja _memset_x32

  mov dh, dl
  mov ebx, edx
  shl ebx, 16
  or edx, ebx

_memset_x32:
  cmp ecx, 32
  jb _memset_x16
  mov dword [eax+0],  edx
  mov dword [eax+4],  edx
  mov dword [eax+8],  edx
  mov dword [eax+12], edx
  mov dword [eax+16], edx
  mov dword [eax+20], edx
  mov dword [eax+24], edx
  mov dword [eax+28], edx
  add eax, 32
  sub ecx, 32
  jmp _memset_x32
  
_memset_x16:
  cmp ecx, 16
  jb _memset_x8
  mov dword [eax+0],  edx
  mov dword [eax+4],  edx
  mov dword [eax+8],  edx
  mov dword [eax+12], edx
  add eax, 16
  sub ecx, 16
  jmp _memset_x16

_memset_x8:
  cmp ecx, 8
  jb _memset_x4
  mov dword [eax+0], edx
  mov dword [eax+4], edx
  add eax, 8
  sub ecx, 8
  jmp _memset_x8

_memset_x4:
  cmp ecx, 4
  jb _memset_xb
  mov dword [eax], edx
  add eax, 4
  sub ecx, 4
  jmp _memset_x4

_memset_xb:
  test ecx, ecx
  jz _exit
  mov byte [eax], dl
  inc eax
  dec ecx
  jmp _memset_xb
  
_exit:
}
#else
  volatile uint8_t * temp = (uint8_t *)data;
  register size_t u_dword = number;

  if (!data || 0 == length || (data + length) < data) {
    return;
  }

  if (u_dword < 0x100) {
    u_dword |= u_dword <<  8;
#if (SIZE_MAX > 0x0000FFFF)
    u_dword |= u_dword << 16;
#endif
#if (SIZE_MAX > 0xFFFFFFFF)
    u_dword |= u_dword << 32;
#endif
  }

  while (length >= (sizeof(size_t) * 8)) {
    *((size_t *)temp + 0) = u_dword;
    *((size_t *)temp + 1) = u_dword;
    *((size_t *)temp + 2) = u_dword;
    *((size_t *)temp + 3) = u_dword;

    *((size_t *)temp + 4) = u_dword;
    *((size_t *)temp + 5) = u_dword;
    *((size_t *)temp + 6) = u_dword;
    *((size_t *)temp + 7) = u_dword;

    temp   += (sizeof(size_t) * 8);
    length -= (sizeof(size_t) * 8);
  }

  while (length >= (sizeof(size_t) * 4)) {
    *((size_t *)temp + 0) = u_dword;
    *((size_t *)temp + 1) = u_dword;
    *((size_t *)temp + 2) = u_dword;
    *((size_t *)temp + 3) = u_dword;

    temp   += (sizeof(size_t) * 4);
    length -= (sizeof(size_t) * 4);
  }

  while (length) {
    *temp = (uint8_t)number;
     temp++;
     length--;
  }
#endif
}

size_t x_strnlen(const char * s, size_t b) { /* DEBUG = OK */
  size_t r = 0;
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov eax, r
  mov ecx, b
  mov edx, s
  test edx, edx
  jz _exit         /* check NULL pointer */
  
_strnlen:
  test ecx, ecx
  jz _exit
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
  size_t j;
  int k;

  if (!data || 0 == length || (data + length) < data) {
    return;
  }

  if (stream == stdin || (stream != stdout && stream != stderr)) {
    stream = stderr;
  }

  for (j = 0; j < length; j++) {
    for(k = 7; k >= 0; --k) {
      putc(('0' + (((uint8_t)data[j] >> k) & 0x01)), stream);
    }
  }

  putc('\n', stream);
}

void strinc(uint8_t * data, size_t len) {
  if (!data || (data + len) < data) {
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
  if (!data || (data + len) < data) {
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

  if (!filename || !buffer || filename == buffer || 0 == length || (buffer + length) < buffer) {
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
  int left, right;
  const char digits[] = "0123456789ABCDEF";

  if (!data || 0 == length || (data + length) < data) {
    return;
  }

  if (tumbler != HEX_STRING && tumbler != HEX_TABLE) {
    tumbler = HEX_TABLE;
  }

  if (stream == stdin || (stream != stdout && stream != stderr)) {
    stream = stderr;
  }

  for (i = 0; i < length; ++i) {
    left  = (int)data[i] >> 0x04; /* 11000011 >> 0x04 = 00001100 */
    right = (int)data[i]  & 0x0F; /* 11000011  & 0x0F = 00000011 */

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

  if (!data || 0 == length || (data + length) < data) {
    return i;
  }

  if (tumbler != HEX_STRING && tumbler != HEX_TABLE) {
    tumbler = HEX_STRING;
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
