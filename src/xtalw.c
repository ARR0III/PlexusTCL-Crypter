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

#include "xtalw.h"

#define HEX_TABLE  1
#define HEX_STRING 0

size_t little_or_big_ending(void) {
  unsigned short x = 0x00FF;

  return ((*(unsigned char *)(&x)) == 0 ? 1 : 0);
}

void * strxormove(void * output, const void * input, size_t length) { /* DEBUG = CHECK */
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov eax, output
  cmp eax, 0
  je _exit

  mov ebx, input
  cmp ebx, 0
  je _exit

  mov ecx, length

  cmp eax, ebx
  je _exit
  jb _normal_word            /* if eax < ebx (jump if below) */
  
/***REVERSE*******************************************************************/
  dec ecx
  add eax, ecx               /* local_output = local_output + (len-1) */
  add ebx, ecx               /* local_input  = local_input  + (len-1) */
  inc ecx

_reverse_byte:
  cmp ecx, 0
  je _exit
  test ecx, 03h
  jz _reverse_word_init      /* if (length % 4) = 0 */
  
  mov dl, byte[ebx]
  xor byte[eax], dl
  
  dec eax
  dec ebx
  dec ecx
  jmp _reverse_byte

_reverse_word_init:          /* Align pointers to 32 bits for read from end! */
  sub eax, 3
  sub ebx, 3

_reverse_word:
  mov edx, dword[ebx]
  xor dword[eax], edx
  sub eax, 4
  sub ebx, 4
  sub ecx, 4
  jnz _reverse_word
  jmp _exit

/***NORMAL*******************************************************************/
_normal_word:
  cmp ecx, 4
  jb _normal_byte
  mov edx, dword[ebx]
  xor dword[eax], edx
  add eax, 4
  add ebx, 4
  sub ecx, 4
  jmp _normal_word
  
_normal_byte:
  cmp ecx, 0
  je _exit
  mov dl, byte[ebx]
  xor byte[eax], dl
  inc eax
  inc ebx
  dec ecx
  jmp _normal_byte
  
/****************************************************************************/
_exit:
}
#else
  const size_t width_register = sizeof(size_t);

        uint8_t * local_output = (uint8_t *)output;
  const uint8_t * local_input  = (const uint8_t *)input;

        uint8_t * last_output = local_output + (length - 1);
  const uint8_t * last_input  = local_input  + (length - 1);

  if (!output || !input || (output == input)) {
    return NULL;
  }

  if (local_output < local_input) {
    while (length >= width_register) {
      (*(size_t *)local_output) ^= (*(size_t *)local_input);

      local_output += width_register;
      local_input  += width_register;
	  
      length       -= width_register;
    }

    while (length) {
      *local_output ^= *local_input;

      local_output++;
      local_input++;
	  
      length--;
    }
  }
  else {
    while ((length) && (0 != (length % width_register))) { /* max 7 iteration */
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

      length      -= width_register;
    }
  }
#endif
  return output;
}

/* "meminit32" optimization and always executable standart function memset */
void * meminit(void * data, const size_t number, size_t len) { /* DEBUG = OK */
#if __ASM_32_X86_CPP_BUILDER__
 __asm {
    mov edx, data
    cmp edx, 0
    je _exit            /* if (NULL == data) goto _exit */
    
    mov ecx, len
    cmp ecx, 0
    je _exit            /* if (0 == len) goto _exit */
	
/* START FUNCTIONAL */
    push ecx            /* save length data */
 
    mov  eax, number
    cmp  eax, 0
    je  _while_start    /* if 0 == number goto _while_start */
    cmp  eax, 255
    ja  _while_start    /* if (number > 255) goto _while_start */

    mov  ecx, eax
    shl  ecx, 8
    or   eax, ecx
    or   ecx, eax       /* if register CL == 0 -> OR operation */
                        /* if register CL  > 0 -> MOV operation */
    shl  ecx, 16
    or   eax, ecx

 _while_start:
    pop ecx             /* load length data in register */

 _while_word:
    cmp ecx, 4
    jb _while_byte      /* if ecx < 4 */

    mov dword[edx], eax /* (*(unsigned int *)data) = number */
    add edx, 4          /* data += 4 */
    sub ecx, 4          /* len  -= 4 */
    jmp _while_word

 _while_byte:
    cmp ecx, 0
    je _exit            /* if ecx == 0 */
    mov byte[edx], al   /* (*(unsigned char *)data) = (unsigned char)number */
    add edx, 1          /* data += 1 */
    sub ecx, 1          /* len  -= 1 */
    jmp _while_byte

 _exit:
 }
#else
  const size_t width_register = sizeof(size_t);

  volatile unsigned char * temp  = (unsigned char *)data;
  register size_t u_dword = number;

  if (NULL == data || 0 == len) {
    return NULL;
  }

  if (u_dword < 0x100) {
    u_dword |= u_dword <<  8;
    u_dword |= u_dword << 16;
#if (SIZE_MAX > 0xFFFFFFFF)
    u_dword |= u_dword << 32;
#endif
  }
	
  while (len >= width_register) {
    (*(size_t *)temp) = u_dword;
	  
    temp += width_register;
    len  -= width_register;
  }
	
  while (len) {
    *temp = (uint8_t)u_dword;
     temp++;
     len--;
  }
#endif
  return data;
}

void * strxor(uint8_t * output, const uint8_t * input, size_t length) { /* DEBUG = OK */
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov eax, output
  cmp eax, 0
  je _exit
  
  mov ebx, input
  cmp ebx, 0
  je _exit  
  
  mov ecx, length
  cmp ecx, 0
  je _exit             /* if length == 0 then */
  cmp ecx, 4
  jb _while_byte       /* if length  < 4 then */
  
 _while:
  mov edx, dword[ebx]  /* temp   = input*/
  xor dword[eax], edx  /* temp   = temp xor output */
  
  add eax, 4           /* output += 4 */
  add ebx, 4           /* input  += 4 */
  sub ecx, 4           /* length -= 4 */
  
  cmp ecx, 0
  je _exit             /* if length == 0 then  */
  cmp ecx, 4
  jb _while_byte       /* if length  < 4 then */
  jmp _while
  
 _while_byte:
  mov dl, byte[ebx]
  xor byte[eax], dl
  
  add eax, 1
  add ebx, 1
  loop _while_byte
  
  _exit:
}
#else
  const size_t width_register = sizeof(size_t);

        unsigned char * local_output = output;
  const unsigned char * local_input  = input;

  if (!input || !output || (input == output)) {
    return output;
  }

  while (length >= width_register) {
    *((size_t *)local_output) ^= *((size_t *)local_input);

    local_output += width_register;
    local_input  += width_register;

    length       -= width_register;
  }

  while (length) {
    *local_output ^= *local_input;
    
    local_output++;
    local_input++;
	
    length--;
  }
#endif
  
  return output;
}

size_t x_strnlen(const char * s, size_t b) { /* DEBUG = OK */
  size_t r = 0;
#if __ASM_32_X86_CPP_BUILDER__
__asm {
  mov eax, r
  
  mov ecx, b
  cmp ecx, 0
  je _exit
  
  mov edx, s
  cmp edx, 0
  je _exit       /* check NULL pointer */
  
_strnlen:
  cmp ecx, 0
  je _exit
  cmp byte[edx], 0
  je _exit
  inc edx        /* str */
  inc eax        /* result */
  dec ecx
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

void arraytobits(uint8_t * data, size_t len, FILE * stream) {
  size_t j;
  int k;

  if (!data) {
    return;
  }

  if (stream != stdin || stream != stdout || stream != stderr) {
    stream = stderr;
  }

  for (j = 0; j < len; j++) {
    uint8_t c = data[j];

    for(k = 7; k >= 0; --k) {
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

void phex(int tumbler, const uint8_t * data, size_t length, FILE * stream) {
  size_t i;
  int left, right, symbol;
  const char digits[] = "0123456789ABCDEF";

  if (!data) {
    return;
  }

  if (tumbler != HEX_STRING || tumbler != HEX_TABLE) {
    tumbler = HEX_TABLE;
  }

  if (stream != stdin || stream != stdout || stream != stderr) {
    stream = stdout;
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

  if (length > 0)
    putc('\n', stream);
}

size_t printhex(int tumbler, const void * data, size_t length) {
  size_t i = 0;
  const uint8_t * temp = (uint8_t *)data;

  if (!data || 0 == length) {
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

  if (length > 0)
    putc('\n', stdout);

  return i;
}
