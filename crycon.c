/*
  Plexus Technology Cybernetic Laboratories;
  Console Cryptography Software v4.92;

  Developer:    ARR0III;
  Make date:    02 June 2021;
  Modification: Release (Original);
  Language:     English;
*/
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "src/arc4.h"
#include "src/crc32.h"
#include "src/sha256.h"
#include "src/serpent.h"
#include "src/twofish.h"
#include "src/rijndael.h"
#include "src/blowfish.h"
#include "src/threefish.h"

#include "src/xtalw.h"
#include "src/clomul.h"

/* if DEBUG_INFORMATION not defined */
#ifdef DEBUG_INFORMATION
#define DEBUG_INFORMATION 1
#endif

/* if COMPILE_FOR_WINDOWS defined in command line */
#ifdef COMPILE_FOR_WINDOWS
#include <windows.h>
#define __STRCMP(S_ONE,S_TWO) strcmpi(S_ONE,S_TWO) /* WINDOWS */
#else
#define __STRCMP(S_ONE,S_TWO) strcmp(S_ONE,S_TWO)  /* LINUX */
#endif

#define OK                      0
#define READ_FILE_NOT_OPEN     -1
#define WRITE_FILE_NOT_OPEN    -2
#define SIZE_FILE_ERROR        -3
#define WRITE_FILE_ERROR       -4
#define READ_FILE_ERROR        -5

#define ENCRYPT              0x00
#define DECRYPT              0xDE

#define PROGRESS_BAR_LENGTH (25+1)

#define BOUNDARY             2048
#define DATA_SIZE      (1024*1024) /* 1 MB */

const char * PARAM_READ_BYTE  = "rb";
const char * PARAM_WRITE_BYTE = "wb";
const char * PROGRAMM_NAME    = "PlexusTCL Console Crypter 4.92 02JUN21 [EN]";

ARC4_CTX      * arc4_ctx      = NULL;
uint32_t      * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
TWOFISH_CTX   * twofish_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

enum {
  ARC4      ,
  AES       ,
  SERPENT   ,
  TWOFISH   ,
  BLOWFISH  ,
  THREEFISH
};

static const uint32_t INT_SIZE_DATA[] = {
  1024,      /* KiB */
  1048576,   /* MiB */
  1073741824 /* GiB */
};

static const char * CHAR_SIZE_DATA[] = {
  "Bt" ,
  "KiB",
  "MiB",
  "GiB"
};

static const char * OPERATION_NAME[] = {
  "Encrypt",
  "Decrypt",
  "Stream"
};

static const char * ALGORITM_NAME[]  = {
  "ARC4",
  "AES-CFB",
  "SERPENT-CFB",
  "TWOFISH-CFB",
  "BLOWFISH-CFB",
  "THREEFISH-CFB"
};

/* Global struct for data */
typedef struct {
/* pointers for */
  char    * finput;             /* path crypt file */
  char    * foutput;            /* path write file */
  char    * keyfile;            /* path keyfile */

  uint8_t * vector;             /* initialized vector for crypt data */
  size_t    vector_length;      /* block size cipher execution */

  uint8_t * temp_buffer;        /* temp_buffer for temp key data */
  size_t    temp_buffer_length; /* size buffer for crypt key */

  uint8_t   input  [DATA_SIZE]; /* memory for read */
  uint8_t   output [DATA_SIZE]; /* memory for write */
  
  char      progress_bar[PROGRESS_BAR_LENGTH];
  
  int       operation;
  int       cipher_number;
} GLOBAL_MEMORY;

static void NAME_CIPHER_ERROR(const char * name) {
  printf("[!] Name cipher \"%s\" incorrect!\n", name);
}

static void MEMORY_ERROR(void) {
  printf("[!] Cannot allocate memory!\n");
}

/* Function size_check checked size = Bt, Kb, Mb or Gb */
static int size_check(uint32_t size) {
  int result = 0;

  if (size >= INT_SIZE_DATA[0] && size < INT_SIZE_DATA[1]) {
    result = 1;
  }
  else
  if (size >= INT_SIZE_DATA[1] && size < INT_SIZE_DATA[2]) {
    result = 2;
  }
  else
  if (size >= INT_SIZE_DATA[2]) {
    result = 3;
  }

  return result;
}

static double sizetodoubleprint(const int status, const double size) {
  return (status ? (size / (double)INT_SIZE_DATA[status - 1]) : size);
}

static void KDFCLOMUL(SHA256_CTX * sha256_ctx,
              const uint8_t * password, const size_t password_len,
                    uint8_t * key,      const size_t key_length) {

  uint32_t i, j, k;
  uint32_t count = 0;
  uint8_t  hash[SHA256_BLOCK_SIZE];
  
#if DEBUG_INFORMATION
  printf("[DEBUG] make crypt key information\n");
  srand(time(0));
  clock_t min = clock();
#endif

  for (i = 0; i < password_len; ++i) {  /* dynamic generation count */
    count ^= (uint32_t)(CRC32(password, i) + CLOMUL_CONST);
    count -= (password_len + key_length + CLOMUL_CONST + i);
  }

  count  &= CRC32(password, password_len);
  count >>= 18; /* MAX == 16383 */
  count  |= ((uint32_t)1 << 14);
  count  *= CLOMUL_CONST;
  
#if DEBUG_INFORMATION
  printf("[DEBUG] count iteration = %ld\n", count);
#endif

  sha256_init(sha256_ctx);

  for (i = k = 0; i < key_length; ++i, ++k) {
    for (j = 0; j < count; ++j) {
      sha256_update(sha256_ctx, password, password_len);
    }

    sha256_final(sha256_ctx, hash);

    if (k >= SHA256_BLOCK_SIZE) {
      k = 0;
    }

    key[i] = hash[k];
  }
  
#if DEBUG_INFORMATION
  printf("[DEBUG] make crypt key time: %4.4f seconds\n", ((double)(clock() - min) / (double)CLOCKS_PER_SEC));
#endif

  meminit((void *)hash, 0x00, SHA256_BLOCK_SIZE);
  count = i = j = k = 0;
}

static void cent(short * number) {
  if (*number > 100) {
    *number = 100;
  }
}

/* return encrypt, decrypt or stream */
static int operation_variant(const int cipher, const int operation) {
  return (cipher ? (operation ? 1 : 0) : 2);
}

static long int size_of_file(FILE * f) {

  if (fseek(f, 0, SEEK_END) != 0) {
    return (-1);
  }

  long int result = ftell(f);

  if (fseek(f, 0, SEEK_SET) != 0) {
    return (-1);
  }

  return result;
}

static void cipher_free(void * ctx, size_t ctx_length) {
  meminit(ctx, 0x00, ctx_length);
  free(ctx);
}

static void free_global_memory(GLOBAL_MEMORY * ctx, const size_t ctx_length) {
  if (NULL != ctx->vector) {
    if (ctx->vector_length > 0) {
      meminit((void *)ctx->vector, 0x00, ctx->vector_length);
    }
    free((void *)ctx->vector);
  }

  if (NULL != ctx->temp_buffer) {
    if (ctx->temp_buffer_length > 0) {
      meminit((void *)ctx->temp_buffer, 0x00, ctx->temp_buffer_length);
    }
    free((void *)ctx->temp_buffer);
  }

  /* clear all memory and all pointers */
  meminit((void *)ctx, 0x00, ctx_length);
  free((void *)ctx);
}

static int filecrypt(GLOBAL_MEMORY * ctx) {

  FILE * fi = fopen(ctx->finput, PARAM_READ_BYTE);
  
  if (NULL == fi) {
    return READ_FILE_NOT_OPEN;
  }

  FILE * fo = fopen(ctx->foutput, PARAM_WRITE_BYTE);
  
  if (NULL == fo) {
    fclose(fi);
    return WRITE_FILE_NOT_OPEN;
  }
  
  register int32_t fsize    = size_of_file(fi);
  register int32_t position = 0;

  if (((-1) == fsize) || (0 == fsize)) {
    fclose(fi);
    fclose(fo);
    return SIZE_FILE_ERROR;
  }

  double div          = (double)((double)fsize / 100.0);
  int    real_check   = 0;
  int    fsize_check  = size_check(fsize);
  double fsize_double = sizetodoubleprint(fsize_check, (double)fsize);

  size_t nblock;
  size_t realread;
  
  short real_percent = 0;
  short past_percent = 0;

  meminit((void *)ctx->progress_bar, '.', PROGRESS_BAR_LENGTH - 1);

  while (position < fsize) {
    if ((ARC4 != ctx->cipher_number) && (0 == position)) {
      if (ENCRYPT == ctx->operation) {
        switch (ctx->cipher_number) {
          case AES:
            rijndael_encrypt(rijndael_ctx, ctx->vector, ctx->output);
            break; 
          case SERPENT:
            serpent_encrypt(serpent_ctx, (uint32_t *)ctx->vector, (uint32_t *)ctx->output);
            break;
          case TWOFISH:
            twofish_encrypt(twofish_ctx, ctx->vector, ctx->output);
            break;
          case BLOWFISH:
            memmove(ctx->output, ctx->vector, ctx->vector_length);
            blowfish_encrypt(blowfish_ctx, (uint32_t *)ctx->output, (uint32_t *)(ctx->output + 4));
            break;
          case THREEFISH:
            threefish_encrypt(threefish_ctx, (uint64_t*)ctx->vector, (uint64_t*)ctx->output);
            break;
        }

        memmove(ctx->vector, ctx->output, ctx->vector_length);

        if (fwrite((void *)ctx->vector, 1, ctx->vector_length, fo) != ctx->vector_length) {
          fclose(fi);
          fclose(fo);

          return WRITE_FILE_ERROR;
        }
        else {
          fflush(fo);
        }
      }
      else
      if (DECRYPT == ctx->operation) {
        if (fread((void *)ctx->vector, 1, ctx->vector_length, fi) != ctx->vector_length) {
          fclose(fi);
          fclose(fo);

          return READ_FILE_ERROR;
        }
        position += (int32_t)ctx->vector_length;
      }
    }

    realread = fread((void *)ctx->input, 1, DATA_SIZE, fi);

    if (ARC4 == ctx->cipher_number) {
      arc4(arc4_ctx, ctx->input, ctx->output, realread);
    }
    else { /* if block ciphers */
      for (nblock = 0; nblock < realread; nblock += ctx->vector_length) {
        switch (ctx->cipher_number) {
          case AES:
            rijndael_encrypt(rijndael_ctx, ctx->vector, ctx->output + nblock);
            break;
          case SERPENT:
            serpent_encrypt(serpent_ctx, (uint32_t *)ctx->vector, (uint32_t *)(ctx->output + nblock));
            break;
          case TWOFISH:
            twofish_encrypt(twofish_ctx, ctx->vector, ctx->output + nblock);
            break;
          case BLOWFISH:
            blowfish_encrypt(blowfish_ctx, (uint32_t *)ctx->vector, (uint32_t *)(ctx->vector + 4));
            memmove(ctx->output + nblock, ctx->vector, ctx->vector_length);
            break;
          case THREEFISH:
            threefish_encrypt(threefish_ctx, (uint64_t*)ctx->vector, (uint64_t*)(ctx->output + nblock));
            break;
        }

        strxor(ctx->output + nblock, ctx->input + nblock, ctx->vector_length);
        memmove(ctx->vector, (ctx->operation ? ctx->input : ctx->output) + nblock, ctx->vector_length);
      }
    }

    if (fwrite((void *)ctx->output, 1, realread, fo) != realread) {
      fclose(fi);
      fclose(fo);

      return WRITE_FILE_ERROR;
    }
    else {
      fflush(fo);
    }

    position += (int32_t)realread;
    real_percent = (short)((double)position / div + 0.1);

    cent(&real_percent);
    
    if (real_percent > past_percent) {
      if ((real_percent % 4) == 0) {
        meminit((void *)ctx->progress_bar, '#', (real_percent / 4));

        real_check = size_check(position);

        printf(" >  %s [%s] (%4.2f %s/%4.2f %s) %3d %%",
          OPERATION_NAME[operation_variant(ctx->cipher_number, ctx->operation)],
          ctx->progress_bar,
          
          sizetodoubleprint(real_check, (double)position + 0.1),
          CHAR_SIZE_DATA[real_check],
          
          fsize_double,
          CHAR_SIZE_DATA[fsize_check],
          
          real_percent);
          
          /* '\r' not show printf function */
          putc(' ',  stdout);
          putc('\r', stdout);

        fflush(stdout);
      }
      past_percent = real_percent;
    }
  }

  putc('\n', stdout);

  fclose(fi);
  fclose(fo);

  return OK;
}

static size_t vector_init(uint8_t * data, size_t size) {
  size_t i;
  size_t stack_trash; /* NOT initialized */

  for (i = 0; i < size; i++) {
    data[i] = (uint8_t)i ^ (uint8_t)genrand(0x00, 0xFF);
  }

  data[0] ^= (uint8_t)stack_trash ^ (uint8_t)genrand(0x00, 0xFF);

  size = size - 2;

  for (i = 0; i < size; i++) {
    if ((data[i] == data[i + 1]) && (data[i + 1] == data[i + 2])) {
      break;
    }
  }

  return i;
}

int main (int argc, char * argv[]) {

  if (argc > 1 && argc < 8) {
    for (int i = 1; i < (argc - 1); i++) {
      if (__strnlen(argv[i], BOUNDARY) == BOUNDARY) { /* if length argument >= 2048 */
        printf("[!] Warning: argument \"%d\" length more \"%d\"!\n", i, BOUNDARY);
        return (-1);
      }
    }
  }
  else
  if (1 == argc || argc >= 8) {
    printf("[!] Error: count arguments %d; necessary to 2 do 7 strings!\n", argc);
    return 0;
  }

  if (2 == argc) {
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
      printf("%s\n", PROGRAMM_NAME);
      printf("This is software for encrypt/decrypt files.\n\n");
      printf("Algoritms:\n\t"
               "-a/--arc4\n\t"
               "-r/--aes\n\t"
               "-s/--serpent\n\t"
               "-w/--twofish\n\t"
               "-b/--blowfish\n\t"
               "-t/--threefish\n");
      printf("Operations:\n\t"
               "-e/--encrypt\n\t"
               "-d/--decrypt\n");
      printf("Lengths key:\n\t"
               "-a/--128/--256\n\t"
               "-b/--192/--512\n\t"
               "-c/--256/--1024\n\n");
      printf("Enter: %s [algoritm] [operation]"
      	     " [key length] [input filename] [output filename] [key filename or string key]\n", argv[0]);
      return 0;
    }
    else {
      printf("[!] Incorrect parameter \"%s\"! Only \"-h\" or \"--help\".\n", argv[1]);
      return (-1);
    }
  }

  if (argc < 5) {
    printf("[!] Error: count arguments %d; necessary to 2 do 7 strings!\n", argc);
    return 0;
  }

  size_t ctx_length = sizeof(GLOBAL_MEMORY);
  GLOBAL_MEMORY * ctx = (GLOBAL_MEMORY *)calloc(ctx_length, 1);

  if (NULL == ctx) {
    MEMORY_ERROR();
    return (-1);
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] compile date and time:   %s, %s\n", __DATE__, __TIME__);
  printf("[DEBUG] global memory allocated: %ld byte\n", ctx_length);
  printf("[DEBUG] global memory pointer:   %p\n", ctx);
#endif

  ctx->keyfile = argv[argc - 1];
  ctx->foutput = argv[argc - 2];
  ctx->finput  = argv[argc - 3];

#if DEBUG_INFORMATION
  printf("[DEBUG] input filename: %s\n", ctx->finput);
  printf("[DEBUG] output filename: %s\n", ctx->foutput);
  printf("[DEBUG] keyfile filename: %s\n", ctx->keyfile);
#endif
  
  if (__STRCMP(ctx->finput, ctx->foutput) == 0) {
    free_global_memory(ctx, ctx_length);
    printf("[!] Names input and output files equal!\n");
    return (-1);
  }
  else
  if (__STRCMP(ctx->foutput, ctx->keyfile) == 0) {
    free_global_memory(ctx, ctx_length);
    printf("[!] Names keyfile and output files equal!\n");
    return (-1);
  }
  else
  if (__STRCMP(ctx->finput, ctx->keyfile) == 0) {
    free_global_memory(ctx, ctx_length);
    printf("[!] Names keyfile and input files equal!\n");
    return (-1);
  }
  
  if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--arc4") == 0)
    ctx->cipher_number = ARC4;
  else
  if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--blowfish") == 0)
    ctx->cipher_number = BLOWFISH;
  else
  if (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--threefish") == 0)
    ctx->cipher_number = THREEFISH;
  else
  if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--aes") == 0)
    ctx->cipher_number = AES;
  else
  if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--serpent") == 0)
    ctx->cipher_number = SERPENT;
  else
  if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--twofish") == 0)
    ctx->cipher_number = TWOFISH;
  else {
    free_global_memory(ctx, ctx_length);
	
    NAME_CIPHER_ERROR(argv[1]);
    return (-1);
  }

  if (ARC4 != ctx->cipher_number) {
    if (strcmp(argv[2], "-e") == 0 || strcmp(argv[2], "--encrypt") == 0) {
      ctx->operation = ENCRYPT;
    }
    else
    if (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "--decrypt") == 0) {
      ctx->operation = DECRYPT;
    }
    else {
      free_global_memory(ctx, ctx_length);
      
      printf("[!] Operation \"%s\" incorrect!\n", argv[2]);
      return (-1);
    }
  }
  
  if (ARC4 == ctx->cipher_number)
    ctx->temp_buffer_length = 2048;
  else
  if (AES     == ctx->cipher_number || 
      SERPENT == ctx->cipher_number ||
      TWOFISH == ctx->cipher_number) { // AES, SERPENT, TWOFISH
    
    if (strcmp(argv[3], "-a") == 0 || strcmp(argv[3], "--128") == 0) {
      if (AES == ctx->cipher_number) {
        AES_Rounds = 10;
      }
      ctx->temp_buffer_length = 128;
    }
    else
    if (strcmp(argv[3], "-b") == 0 || strcmp(argv[3], "--192") == 0) {
      if (AES == ctx->cipher_number) {
        AES_Rounds = 12;
      }
      ctx->temp_buffer_length = 192;
    }
    else
    if (strcmp(argv[3], "-c") == 0 || strcmp(argv[3], "--256") == 0) {
      if (AES == ctx->cipher_number) {
        AES_Rounds = 14;
      }
      ctx->temp_buffer_length = 256;
    }
    else {
      free_global_memory(ctx, ctx_length);
	  
      printf("[!] Key length \"%s\" incorrect!\n", argv[3]);
      return (-1);
    }
  }
  else
  if (BLOWFISH  == ctx->cipher_number) {
    ctx->temp_buffer_length = 448;
  }
  else
  if (THREEFISH == ctx->cipher_number) {
    if (strcmp(argv[3], "-a") == 0 || strcmp(argv[3], "--256") == 0) {
      ctx->temp_buffer_length = 256;
    }
    else
    if (strcmp(argv[3], "-b") == 0 || strcmp(argv[3], "--512") == 0) {
      ctx->temp_buffer_length = 512;
    }
    else
    if (strcmp(argv[3], "-c") == 0 || strcmp(argv[3], "--1024") == 0) {
      ctx->temp_buffer_length = 1024;
    }
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] cipher: %s\n", ALGORITM_NAME[ctx->cipher_number]);
  printf("[DEBUG] key length: %ld bist\n", ctx->temp_buffer_length);
  printf("[DEBUG] operation: %s\n", OPERATION_NAME[ctx->operation ? 1 : 0]);
#endif
  
  ctx->temp_buffer_length /= 8; /* for allocate memory */
  
  /*
    ARC4      = (temp_buffer_length = 256);
    AES       = (temp_buffer_length = 16 or 24 or 32;
    SERPENT   = (temp_buffer_length = 16 or 24 or 32);
    BLOWFISH  = (temp_buffer_length = 56);
    THREEFISH = (temp_buffer_length = 32 or 64 or 128);
  */
  
  ctx->temp_buffer = (uint8_t*)calloc(ctx->temp_buffer_length, 1);

  if (NULL == ctx->temp_buffer) {
    free_global_memory(ctx, ctx_length);
    
    MEMORY_ERROR();
    return (-1);
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] temp memory allocated: %ld byte\n", ctx->temp_buffer_length);
  printf("[DEBUG] temp memory pointer: %p\n", ctx->temp_buffer);
#endif
  
  int real_read = readfromfile(ctx->keyfile, ctx->temp_buffer, ctx->temp_buffer_length);

  if (real_read == ctx->temp_buffer_length)
    printf("[#] Crypt key read from file \"%s\"!\n", ctx->keyfile);
  else
  if ((real_read > 0) && (real_read < (ctx->temp_buffer_length))) {
  
    printf("[!] Data in key file %d byte; necessary %d byte!\n",
            real_read, (int32_t)ctx->temp_buffer_length);
    
    free_global_memory(ctx, ctx_length);
    return (-1);
  }
  else
  if ((0 == real_read) || ((-1) == real_read)) {
    real_read = (int)__strnlen(ctx->keyfile, 256);

    if ((real_read > 7) && (real_read < 257)) { /* Max password length = 256 byte; min = 8  */
      size_t sha_ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = (SHA256_CTX*) calloc(1, sha_ctx_len);

      if (sha256_ctx != NULL) {
        /* Password -> crypt key; Pseudo PBKDF2 */
        KDFCLOMUL(sha256_ctx, (uint8_t *)(ctx->keyfile), 
                    real_read, ctx->temp_buffer,
                    ctx->temp_buffer_length);
                  
        meminit((void *)sha256_ctx, 0x00, sha_ctx_len);

        free((void *)sha256_ctx);
        sha256_ctx = NULL;

        printf("[#] Crypt key read from command line!\n");
      }
      else {
        free_global_memory(ctx, ctx_length);
        
        MEMORY_ERROR();
        return (-1);
      }
    }
    else {
      free_global_memory(ctx, ctx_length);
      
      printf("[!] Data in string key %d byte; necessary 8..256 byte!\n", real_read);
      return (-1);
    }
  }

  printf("[#] Key length %d-bits initialized!\n", (int32_t)ctx->temp_buffer_length * 8);

#if DEBUG_INFORMATION
  printf("[DEBUG] key or password length: %d byte\n", real_read);
  printf("[DEBUG] key generator write data in pointer: %p\n", ctx->temp_buffer);
  printf("[DEBUG] REAL CRYPT KEY DATA:\n");
  
  printhex(HEX_TABLE, ctx->temp_buffer, ctx->temp_buffer_length);
#endif
  
  size_t cipher_ctx_len = 0;

  switch (ctx->cipher_number) {
    case ARC4:
      cipher_ctx_len = sizeof(ARC4_CTX);
      break;
    case AES:
      ctx->vector_length = 16;
      cipher_ctx_len = 4 * (AES_Rounds + 1) * 4;
      break;
    case SERPENT:
      ctx->vector_length = 16;
      cipher_ctx_len = sizeof(SERPENT_CTX);
      break;
    case TWOFISH:
      ctx->vector_length = 16;
      cipher_ctx_len = sizeof(TWOFISH_CTX);
      break;
    case BLOWFISH:
      ctx->vector_length =  8;
      cipher_ctx_len = sizeof(BLOWFISH_CTX);
      break;
    case THREEFISH:
      ctx->vector_length = ctx->temp_buffer_length;
      cipher_ctx_len = sizeof(THREEFISH_CTX);
      break;
  }

  ctx->vector = NULL;

  if (ARC4 != ctx->cipher_number) {
    ctx->vector = (uint8_t*)calloc(ctx->vector_length, 1);

    if (NULL == ctx->vector) {
      free_global_memory(ctx, ctx_length);
      
      MEMORY_ERROR();
      return (-1);
    }
    
#if DEBUG_INFORMATION
  printf("[DEBUG] vector memory allocated: %ld byte\n", ctx->vector_length);
  printf("[DEBUG] vector memory pointer: %p\n", ctx->vector);
#endif
    
  }

  if ((ARC4 != ctx->cipher_number) && (ENCRYPT == ctx->operation)) {
    srand((unsigned int)time(NULL));

    if (vector_init(ctx->vector, ctx->vector_length) < (ctx->vector_length - 2)) {
      free_global_memory(ctx, ctx_length);
      
      printf("[X] Critical error! System time stopped?\n");
      return (-1);
    }
    
#if DEBUG_INFORMATION
  printf("[DEBUG] vector generator write data in pointer: %p\n", ctx->vector);
  printf("[DEBUG] VECTOR REAL DATA:\n");
  printhex(HEX_TABLE, ctx->vector, ctx->vector_length);
#endif
    
  }

  void * cipher_pointer = NULL;

  if (ARC4 == ctx->cipher_number) {
    arc4_ctx = (ARC4_CTX *) calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)arc4_ctx;

    if (NULL == arc4_ctx) {
      free_global_memory(ctx, ctx_length);
      
      MEMORY_ERROR();
      return (-1);
    }

    arc4_init(arc4_ctx, ctx->temp_buffer, ctx->temp_buffer_length);
  }
  else
  if (AES == ctx->cipher_number) {
    rijndael_ctx = (uint32_t *) calloc(cipher_ctx_len, 1);
    cipher_pointer = (void *)rijndael_ctx;

    if (NULL == rijndael_ctx) {
      free_global_memory(ctx, ctx_length);
      
      MEMORY_ERROR();
      return (-1);
    }
    
    AES_Rounds =
      rijndael_key_encrypt_init(rijndael_ctx, ctx->temp_buffer, ctx->temp_buffer_length * 8);
  }
  if (TWOFISH == ctx->cipher_number) {
    twofish_ctx = (TWOFISH_CTX *) calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)twofish_ctx;

    if (NULL == twofish_ctx) {
      free_global_memory(ctx, ctx_length);
      
      MEMORY_ERROR();
      return (-1);
    }

    twofish_init(twofish_ctx, ctx->temp_buffer, ctx->temp_buffer_length);
  }
  else
  if (SERPENT == ctx->cipher_number) {
    serpent_ctx = (SERPENT_CTX *) calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)serpent_ctx;

    if (NULL == serpent_ctx) {
      free_global_memory(ctx, ctx_length);
      
      MEMORY_ERROR();
      return (-1);
    }

    serpent_init(serpent_ctx, ctx->temp_buffer_length * 8, ctx->temp_buffer);
  }
  else
  if (BLOWFISH == ctx->cipher_number) {
    blowfish_ctx = (BLOWFISH_CTX *)calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)blowfish_ctx;

    if (NULL == blowfish_ctx) {
      free_global_memory(ctx, ctx_length);
      
      MEMORY_ERROR();
      return (-1);
    }

    blowfish_init(blowfish_ctx, ctx->temp_buffer, ctx->temp_buffer_length);
  }
  else
  if (THREEFISH == ctx->cipher_number) {
    threefish_ctx = (THREEFISH_CTX *)calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)threefish_ctx;

    if (NULL == threefish_ctx) {
      free_global_memory(ctx, ctx_length);
      
      MEMORY_ERROR();
      return (-1);
    }

    threefish_init(threefish_ctx, (threefishkeysize_t)(ctx->temp_buffer_length * 8),
                   (uint64_t*)ctx->temp_buffer, (uint64_t*)ctx->temp_buffer);
  }

  printf("[#] Algoritm %s initialized!\n",
    ALGORITM_NAME[(ctx->cipher_number)]);

#if DEBUG_INFORMATION
  printf("[DEBUG] allocate byte for cipher struct: %ld\n", cipher_ctx_len);
  printf("[DEBUG] REAL DATA CIPHER STRUCT:\n");
  printhex(HEX_TABLE, cipher_pointer, cipher_ctx_len);
#endif

  printf("[#] Operation %s file \"%s\" started!\n",
    OPERATION_NAME[operation_variant(ctx->cipher_number, ctx->operation)],
    ctx->finput);

  int result = filecrypt(ctx);

  switch (result) {
    case OK:
      printf("[#] %s file \"%s\" complete!\n",
        OPERATION_NAME[operation_variant(ctx->cipher_number, ctx->operation)], ctx->finput);
      break;
    case READ_FILE_NOT_OPEN:
      printf("[!] File for %s \"%s\" not opened!\n",
        OPERATION_NAME[operation_variant(ctx->cipher_number, ctx->operation)], ctx->finput);
      break;
    case WRITE_FILE_NOT_OPEN:
      printf("[!] Output file \"%s\" not opened!\n", ctx->output);
      break;
    case SIZE_FILE_ERROR:
      printf("[!] Size of input file \"%s\" 0 or more 2 GiB!\n", ctx->finput);
      break;
    case WRITE_FILE_ERROR:
      printf("[!] Error write in file \"%s\" !\n", ctx->foutput);
      break;
    case READ_FILE_ERROR:
      printf("[!] Error read form file \"%s\" !\n", ctx->finput);
      break;
  }

/* Clear all allocated memory for programm */

  cipher_free((void *)cipher_pointer, cipher_ctx_len);

  cipher_pointer = NULL;

  switch (ctx->cipher_number) {
    case ARC4:
      arc4_ctx = NULL;
      break;
    case AES:
      rijndael_ctx = NULL;
      break;
    case SERPENT:
      serpent_ctx = NULL;
      break;
    case TWOFISH:
      twofish_ctx = NULL;
      break;
    case BLOWFISH:
      blowfish_ctx = NULL;
      break;
    case THREEFISH:
      threefish_ctx = NULL;
      break;
  }

  free_global_memory(ctx, ctx_length);

  return 0;
}

