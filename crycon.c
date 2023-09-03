/*
 * Plexus Technology Cybernetic Laboratory;
 * Console Cryptography Software v5.08;
 *
 * Developer:         ARR0III;
 * Modification date: 03 SEP 2023;
 * Modification:      Release;
 * Language:          English;
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

#ifndef DEBUG_INFORMATION
#  define DEBUG_INFORMATION 0
#else
#  define DEBUG_INFORMATION 1
#endif

#ifdef MS_WINDOWS
#  include <windows.h>
#  define STRCMP(S_ONE,S_TWO) strcmpi((S_ONE), (S_TWO)) /* WINDOWS */
#else
#  define STRCMP(S_ONE,S_TWO) strcmp((S_ONE), (S_TWO))  /* LINUX */
#endif

#define MEMORY_ERROR do \
    fprintf(stderr, "[!] Cannot allocate memory!\n"); \
  while(0)

#define MINIMAL(a,b) (((a) < (b)) ? (a) : (b))

#define OK                           0
#define READ_FILE_NOT_OPEN          -1
#define WRITE_FILE_NOT_OPEN         -2
#define SIZE_FILE_ERROR             -3
#define WRITE_FILE_ERROR            -4
#define READ_FILE_ERROR             -5
#define STREAM_INPUT_CLOSE_ERROR    -6
#define STREAM_OUTPUT_CLOSE_ERROR   -7
#define SIZE_DECRYPT_FILE_INCORRECT -8

#define LENGTH_DATA_FOR_CHECK     1024

#define ENCRYPT                   0x00
#define DECRYPT                   0xDE

#define PROGRESS_BAR_LENGTH         26

#define STRING_MAX_LENGTH         2048
#define DATA_SIZE     ((1024*1024) * 8 /*MiB*/ ) /* READ AND WRITE FROM DRIVE */

const char * PARAM_READ_BYTE  = "rb";
const char * PARAM_WRITE_BYTE = "wb";
const char * PROGRAMM_NAME    = "PlexusTCL Console Crypter 5.08 03SEP23 [EN]";

static uint32_t      * rijndael_ctx  = NULL;
static SERPENT_CTX   * serpent_ctx   = NULL;
static TWOFISH_CTX   * twofish_ctx   = NULL;
static BLOWFISH_CTX  * blowfish_ctx  = NULL;
static THREEFISH_CTX * threefish_ctx = NULL;

typedef enum cipher_number_enum {
  AES       = 0,
  SERPENT   = 1,
  TWOFISH   = 2,
  BLOWFISH  = 3,
  THREEFISH = 4
} cipher_t;

static const uint32_t INT_SIZE_DATA[] = {
  (uint32_t)1 << 10, /* KiB */
  (uint32_t)1 << 20, /* MeB */
  (uint32_t)1 << 30  /* GiB */
};

static const char * CHAR_SIZE_DATA[] = {
  "Bt" ,
  "KiB",
  "MiB",
  "GiB"
};

static const char * OPERATION_NAME[] = {
  "Encrypt",
  "Decrypt"
};

static const char * ALGORITM_NAME[]  = {
  "AES",
  "SERPENT",
  "TWOFISH",
  "BLOWFISH",
  "THREEFISH"
};

typedef struct {
  uint8_t hash[SHA256_BLOCK_SIZE];
  uint8_t KEY_0[SHA256_BLOCK_SIZE];
  uint8_t KEY_1[SHA256_BLOCK_SIZE];
} HMAC_CTX;

/* Global struct for data */
typedef struct {
/* pointers for */
  char       * finput;              /* path crypt file */
  char       * foutput;             /* path write file */
  char       * keyfile;             /* path keyfile or string key */

  SHA256_CTX * sha256sum;           /* memory for sha256 hash function */
  size_t       sha256sum_length;    /* size struct to pointer ctx->sha256sum */

  uint8_t    * vector;              /* initialized vector for crypt data */
  size_t       vector_length;       /* block size cipher execution */

  uint8_t    * temp_buffer;         /* temp_buffer for temp key data */
  size_t       temp_buffer_length;  /* size buffer for crypt key */

  uint8_t      input  [DATA_SIZE];  /* memory for read */
  uint8_t      output [DATA_SIZE];  /* memory for write */

  char         progress_bar[PROGRESS_BAR_LENGTH];

  int          operation;           /* ENCRYPT == 0x00 or DECRYPT == 0xDE */
  cipher_t     cipher_number;       /* search type name cipher_number_enum */
} GLOBAL_MEMORY;

static void free_global_memory(GLOBAL_MEMORY * ctx, const size_t ctx_length) {
  if (!ctx) {
    return;
  }	
	
  if (ctx->sha256sum) {
    if (ctx->sha256sum_length > 0) {
      meminit((void *)ctx->sha256sum, 0x00, ctx->sha256sum_length);
    }
    free((void *)ctx->sha256sum);
  }

  if (ctx->vector) {
    if (ctx->vector_length > 0) {
      meminit((void *)ctx->vector, 0x00, ctx->vector_length);
    }
    free((void *)ctx->vector);
  }

  if (ctx->temp_buffer) {
    if (ctx->temp_buffer_length > 0) {
      meminit((void *)ctx->temp_buffer, 0x00, ctx->temp_buffer_length);
    }
    free((void *)ctx->temp_buffer);
  }

  /* clear all memory and all pointers */
  meminit((void *)ctx, 0x00, ctx_length);
  free((void *)ctx);
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

static void KDFCLOMUL(GLOBAL_MEMORY * ctx,
              const uint8_t  * password, const size_t password_len,
                    uint8_t  * key,      const size_t key_len) {

  uint32_t i, j, k;
  uint32_t count = 0;

  if (!ctx || !password || !key) {
    return;
  }

#if DEBUG_INFORMATION
  srand(time(0));
  clock_t min = clock();
#endif

  for (i = 1; i <= password_len; ++i) {  /* dynamic generation count */
    count ^= (uint32_t)(CRC32(password, i) + CLOMUL_CONST);
    count -= (password_len + key_len + CLOMUL_CONST + i);
  }

  count  &= CRC32(password, password_len);
  count >>= 18; /* MAX == 16383 */
  count  |= ((uint32_t)1 << 14);
  count  *= CLOMUL_CONST;

#if DEBUG_INFORMATION
  printf("[DEBUG] make crypt key count iteration: %u\n", count);
#endif

  sha256_init(ctx->sha256sum);

  for (i = k = 0; i < key_len; ++i, ++k) {
    for (j = 0; j < count; ++j) {
      sha256_update(ctx->sha256sum, password, password_len);
    }

    sha256_final(ctx->sha256sum);

    if (k >= SHA256_BLOCK_SIZE) {
      k = 0;
    }

    key[i] = ctx->sha256sum->hash[k];
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] make crypt key time: %4.4f seconds\n", ((double)(clock() - min) / (double)CLOCKS_PER_SEC));
#endif

  meminit((void *)ctx->sha256sum, 0x00, ctx->sha256sum_length);
}

/* return encrypt, decrypt or stream */
static int operation_variant(const int operation) {
  return (operation ? 1 : 0);
}

static int32_t size_of_file(FILE * f) {
  int32_t result;

  if (!f) {
    return (-1);
  }
	
  if (fseek(f, 0, SEEK_END) != 0) {
    return (-1);
  }

  result = ftell(f);

  if (fseek(f, 0, SEEK_SET) != 0) {
    return (-1);
  }

  return result;
}

static void cipher_free(void * ctx, size_t ctx_length) {
  if (!ctx) {
    return;
  }
	
  meminit(ctx, 0x00, ctx_length);
  free(ctx);
}

static void hmac_sha256_uf(GLOBAL_MEMORY * ctx) {
  size_t i;
  size_t size_copy_data;
  size_t hmac_ctx_length;

  HMAC_CTX * hmac_ctx;

  if (!ctx) {
    return;
  }
	
  hmac_ctx_length = sizeof(HMAC_CTX);	
  hmac_ctx = (HMAC_CTX *)malloc(hmac_ctx_length);

  if (!hmac_ctx) {
    return;
  }

  size_copy_data = MINIMAL(ctx->temp_buffer_length, SHA256_BLOCK_SIZE);

  /* copy hash sum file in local buffer "hash" */
  memcpy((void *)hmac_ctx->hash, (void *)(ctx->sha256sum->hash), SHA256_BLOCK_SIZE);

  /* generate two secret const for hash update */
  memcpy((void *)hmac_ctx->KEY_0, (void *)ctx->temp_buffer, size_copy_data);
  memcpy((void *)hmac_ctx->KEY_1, (void *)ctx->temp_buffer, size_copy_data);

    /* if length temp_buffer equal or more SHA256_BLOCK_SIZE then cycle NOT executable */
  for (i = size_copy_data; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] = 0x00;
    hmac_ctx->KEY_1[i] = 0x00;
  }

  for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] ^= 0x55; /* simbol 'U', decimal  85, bits 01010101 */
    hmac_ctx->KEY_1[i] ^= 0x66; /* simbol 'f', decimal 102, bits 10101010 */
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] authentification key \'U\':\n");
  printhex(HEX_TABLE, hmac_ctx->KEY_0, SHA256_BLOCK_SIZE);

  printf("[DEBUG] authentification key \'f\':\n");
  printhex(HEX_TABLE, hmac_ctx->KEY_1, SHA256_BLOCK_SIZE);
#endif

  /* clear sha256sum struct */
  meminit((void *)(ctx->sha256sum), 0x00, ctx->sha256sum_length);

  /* calculate hash for (key xor 0x55) and hash file */
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, hmac_ctx->KEY_0, SHA256_BLOCK_SIZE);
  sha256_update(ctx->sha256sum, hmac_ctx->hash, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  memcpy((void *)hmac_ctx->hash, (void *)(ctx->sha256sum->hash), SHA256_BLOCK_SIZE);

  /* clear sha256sum struct */
  meminit((void *)(ctx->sha256sum), 0x00, ctx->sha256sum_length);

  /* calculate hash for (key xor 0x66) and hash for ((key xor 0x55) and hash file) */
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, hmac_ctx->KEY_1, SHA256_BLOCK_SIZE);
  sha256_update(ctx->sha256sum, hmac_ctx->hash, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  /* clear memory for security */
  meminit((void *)hmac_ctx, 0x00, hmac_ctx_length);
  free(hmac_ctx);
  /* now control sum crypt key and file in buffer ctx->sha256sum->hash */
}

static void control_sum_buffer(GLOBAL_MEMORY * ctx, const size_t count) {
  size_t i = 0;
  size_t remnant = count;

  if (!ctx) {
    return;
  }

  while (i < count) {
    if (remnant < LENGTH_DATA_FOR_CHECK) {
      sha256_update(ctx->sha256sum,
                    (ctx->operation ? ctx->output : ctx->input) + i,
                    remnant);
    }
    else {
      sha256_update(ctx->sha256sum,
                    (ctx->operation ? ctx->output : ctx->input) + i,
                    LENGTH_DATA_FOR_CHECK);
    }

          i += LENGTH_DATA_FOR_CHECK;
    remnant -= LENGTH_DATA_FOR_CHECK;
  }
}

static int close_in_out_files(FILE * file_input, FILE * file_output, const int return_code) {
  if (fclose(file_input) == -1) {
    fclose(file_output);
    return STREAM_INPUT_CLOSE_ERROR;
  }
  
  if (fclose(file_output) == -1) {
    return STREAM_OUTPUT_CLOSE_ERROR;
  }
  
  return return_code; /* All files close complete! */
}

static int filecrypt(GLOBAL_MEMORY * ctx) {

  FILE * fi = NULL;
  FILE * fo = NULL;

  double div, fsize_double;
  int    fsize_check, real_check = 0;
  int    real_percent, past_percent = 0;

  size_t nblock, realread;

  int32_t fsize, position = 0;

  fi = fopen(ctx->finput, PARAM_READ_BYTE);

  if (!fi) {
    return READ_FILE_NOT_OPEN;
  }

  fo = fopen(ctx->foutput, PARAM_WRITE_BYTE);

  if (!fo) {
    if (fclose(fi) == -1)
      return STREAM_INPUT_CLOSE_ERROR;
    else
      return WRITE_FILE_NOT_OPEN;
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] size of file: %d byte\n", fsize);
#endif

  fsize = size_of_file(fi);

  if (ENCRYPT == ctx->operation) { /* only for check fsize */
    fsize += (SHA256_BLOCK_SIZE + ctx->vector_length);
  }

  /* fsize += (size initialized vector + size sha256 hash sum) */
  /* break operation if (fsize > 2 GB) or (fsize == 0) or (fsize == -1) */

  if (fsize <= 0L) {
    return close_in_out_files(fi, fo, SIZE_FILE_ERROR);
  }

  if (ENCRYPT == ctx->operation) { /* only for check fsize */
    fsize -= (SHA256_BLOCK_SIZE + ctx->vector_length);
  }

  if (DECRYPT == ctx->operation) {
    /* if fsize < minimal size file for decrypt */
    if (fsize < (long int)(SHA256_BLOCK_SIZE + ctx->vector_length + 1)) {
      return close_in_out_files(fi, fo, SIZE_DECRYPT_FILE_INCORRECT);
    }
  }

  div          = (double)((double)fsize / 100.0);
  fsize_check  = size_check(fsize);
  fsize_double = sizetodoubleprint(fsize_check, (double)fsize);

  meminit(ctx->progress_bar, '.', PROGRESS_BAR_LENGTH - 1);

  sha256_init(ctx->sha256sum);

  while (position < fsize) {
    if (0 == position) {

#if DEBUG_INFORMATION
  printf("[DEBUG] vector memory allocated: %u byte\n", ctx->vector_length);
  printf("[DEBUG] vector memory pointer: %p\n", ctx->vector);
#endif

      if (ENCRYPT == ctx->operation) {
        switch (ctx->cipher_number) { /* delete this code block or not */
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
            memcpy(ctx->output, ctx->vector, ctx->vector_length);
            blowfish_encrypt(blowfish_ctx, (uint32_t *)ctx->output, (uint32_t *)(ctx->output + 4));
            break;
          case THREEFISH:
            threefish_encrypt(threefish_ctx, (uint64_t*)ctx->vector, (uint64_t*)ctx->output);
            break;
        }

        memcpy(ctx->vector, ctx->output, ctx->vector_length); /* delete this string or not */

#if DEBUG_INFORMATION
  printf("[DEBUG] vector generator write data in pointer: %p\n", ctx->vector);
#endif

        if (fwrite((void *)ctx->vector, 1, ctx->vector_length, fo) != ctx->vector_length) {
          return close_in_out_files(fi, fo, WRITE_FILE_ERROR);
        }

        fflush(fo);

      }
      else
      if (DECRYPT == ctx->operation) {
        if (fread((void *)ctx->vector, 1, ctx->vector_length, fi) != ctx->vector_length) {
          return close_in_out_files(fi, fo, READ_FILE_ERROR);
        }
		
      position += (int32_t)ctx->vector_length;
#if DEBUG_INFORMATION
  printf("[DEBUG] vector data read from file in pointer: %p\n", ctx->vector);
#endif
      }

#if DEBUG_INFORMATION
  printf("[DEBUG] vector real data:\n");
  printhex(HEX_TABLE, ctx->vector, ctx->vector_length);
#endif

    }

    realread = fread((void *)ctx->input, 1, DATA_SIZE, fi);

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
          memcpy(ctx->output + nblock, ctx->vector, ctx->vector_length);
          break;
        case THREEFISH:
          threefish_encrypt(threefish_ctx, (uint64_t*)ctx->vector, (uint64_t*)(ctx->output + nblock));
          break;
        }

      memxormove(ctx->output + nblock, ctx->input + nblock, ctx->vector_length);
      memcpy(ctx->vector, (ctx->operation ? ctx->input : ctx->output) + nblock, ctx->vector_length);
    }

       position  += (int32_t)realread;
    real_percent  = (int)((double)position / div + 0.1);

    if (real_percent > 100) {
      real_percent = 100;
    }

    if ((position == fsize) && (DECRYPT == ctx->operation)) {
      realread -= SHA256_BLOCK_SIZE;
    }

    /* control sum all read 1 MB data for [en/de]crypt + crypt key */
    control_sum_buffer(ctx, realread);

    if (fwrite((void *)ctx->output, 1, realread, fo) != realread) {
      return close_in_out_files(fi, fo, WRITE_FILE_ERROR);
    }

    fflush(fo);

    if (real_percent > past_percent) {
      /* if ((real_percent % 4) == 0) { */
        meminit(ctx->progress_bar, '#', (real_percent / 4));

        real_check = size_check(position);

        printf("\r >  %s [%s] (%4.2f %s/%4.2f %s) %3d %%  ",
          OPERATION_NAME[operation_variant(ctx->operation)],
          ctx->progress_bar,

          sizetodoubleprint(real_check, (double)position),
          CHAR_SIZE_DATA[real_check],

          fsize_double,
          CHAR_SIZE_DATA[fsize_check],

          real_percent);

        fflush(stdout);
      /* } */
      past_percent = real_percent;
    }
  }

  putc('\n', stdout);

  sha256_final(ctx->sha256sum);

#if DEBUG_INFORMATION
  printf("[DEBUG] real sha-2-256 hash sum file: %s\n", ctx->finput);
  printhex(HEX_TABLE, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
#endif

  /* generate HMAC for hash file and key */
  hmac_sha256_uf(ctx);

  if (ENCRYPT == ctx->operation) {
    if (fwrite((void *)ctx->sha256sum->hash, 1, SHA256_BLOCK_SIZE, fo) != SHA256_BLOCK_SIZE) {
      return close_in_out_files(fi, fo, WRITE_FILE_ERROR);
    }

    fflush(fo);

  }
  else {
    if (memcmp((void *)(ctx->input + realread),
               (void *)(ctx->sha256sum->hash), SHA256_BLOCK_SIZE) != 0) {

      printf("[!] WARNING: Control sum file \"%s\" not correct!\n", ctx->finput);
    }
    else {
      printf("[#] Control sum file \"%s\" check! :)\n", ctx->finput);
    }
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] real hmac input filename: %s\n", ctx->finput);
  printhex(HEX_TABLE, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
#endif

  return close_in_out_files(fi, fo, OK);
}

static void random_vector_init(uint8_t * data, size_t size) {
  size_t i;
  size_t arc4_size   = sizeof(ARC4_CTX);
  size_t vector_size = size;
  uint8_t * vector_memory = NULL;

  if ((!data) || (0 == size)) {
    return;
  }
  
  ARC4_CTX * arc4_memory = (ARC4_CTX *)malloc(arc4_size);
  vector_memory = (uint8_t *)malloc(vector_size);
  
  if ((!arc4_memory) || (!vector_memory)) {
    if (NULL != arc4_memory) {
      free(arc4_memory);
    }
	
    if (NULL != vector_memory) {
      free(vector_memory);
    }
	
    return;
  }
  
  /* generate trash for security and xor with random data from memory */
  for (i = 0; i < vector_size; i++) {
    vector_memory[i] ^= (uint8_t)genrand(0x00, 0xFF);
  }
  
  /* encrypt data initialized vector for security */
  arc4_init(arc4_memory, data, size);
  arc4(arc4_memory, vector_memory, data, size);
  
  /* clear all data for security */
  meminit(vector_memory, 0x00, vector_size);
  meminit(arc4_memory, 0x00, arc4_size);
  
  free(vector_memory);
  free(arc4_memory);
  
  vector_memory = NULL;
  arc4_memory = NULL;
  
  vector_size = 0;
  arc4_size   = 0;
}

static size_t vector_init(uint8_t * data, size_t size) {
  size_t i;
  size_t stack_trash; /* NOT initialized == ALL OK */

  if (!data) {
    return 0;
  }	

#if DEBUG_INFORMATION
  printf("[DEBUG] stack_trash: %u\n", stack_trash);
#endif
  
  for (i = 0; i < size; i++) {
    data[i] = (uint8_t)i ^ (uint8_t)genrand(0x00, 0xFF);
  }

  /* random data from stack xor initialized vector */
  (*(uint32_t *)data) ^= (uint32_t)stack_trash ^ (uint32_t)genrand(0x0000, 0x7FFF);

  /* generate real vector with cryptography */
  random_vector_init(data, size);

  size = size - 2; /* for check cycle bottom */

  /* What the fuck is this ??? */
  for (i = 0; i < size; i++) {
    if ((data[i] == data[i + 1]) && (data[i + 1] == data[i + 2])) {
      break;
    }
  }

  return i;
}

int main(int argc, char * argv[]) {

/*****************************************************************************/
  extern int AES_Rounds; /* in rijndael.c source code file */

  int i, real_read, result;

  size_t ctx_length, cipher_ctx_len = 0;

  GLOBAL_MEMORY * ctx;
  void * cipher_pointer = NULL;
/*****************************************************************************/

  if (argc > 1 && argc < 8) {
    for (i = 1; i < (argc - 1); i++) {
      if (x_strnlen(argv[i], STRING_MAX_LENGTH) == STRING_MAX_LENGTH) { /* if length argument >= 2048 */
        fprintf(stderr, "[!] Warning: argument \"%d\" length more \"%d\"!\n", i, STRING_MAX_LENGTH);
        return (-1);
      }
    }
  }
  else
  if (1 == argc || argc >= 8) {
    fprintf(stderr, "[!] Error: count arguments %d; necessary to 2 do 7 strings!\n", argc);
    return 0;
  }

  if (2 == argc) {
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
      printf("%s\n", PROGRAMM_NAME);
      printf("This is software for encrypt/decrypt files.\n\n");
      printf("Algoritms:\n\t"
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
      fprintf(stderr, "[!] Incorrect parameter \"%s\"! Only \"-h\" or \"--help\".\n", argv[1]);
      return (-1);
    }
  }

  if (argc < 5) {
    fprintf(stderr, "[!] Error: count arguments %d; necessary to 2 do 7 strings!\n", argc);
    return 0;
  }

  ctx_length = sizeof(GLOBAL_MEMORY);
  ctx = (GLOBAL_MEMORY *)calloc(ctx_length, 1);

  if (!ctx) {
    MEMORY_ERROR;
    return (-1);
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] compile date and time:   %s, %s\n", __DATE__, __TIME__);
  printf("[DEBUG] global memory allocated: %u byte\n", ctx_length);
  printf("[DEBUG] global memory pointer:   %p\n", ctx);
#endif

  ctx->keyfile = argv[argc - 1];
  ctx->foutput = argv[argc - 2];
  ctx->finput  = argv[argc - 3];

#if DEBUG_INFORMATION
  printf("[DEBUG] input filename:      %s\n", ctx->finput);
  printf("[DEBUG] output filename:     %s\n", ctx->foutput);
  printf("[DEBUG] keyfile or password: %s\n", ctx->keyfile);
#endif

  if (STRCMP(ctx->finput, ctx->foutput) == 0) {
    free_global_memory(ctx, ctx_length);

    fprintf(stderr, "[!] Names input and output files equal!\n");
    return (-1);
  }
  else
  if (STRCMP(ctx->foutput, ctx->keyfile) == 0) {
    free_global_memory(ctx, ctx_length);

    fprintf(stderr, "[!] Names keyfile and output files equal!\n");
    return (-1);
  }
  else
  if (STRCMP(ctx->finput, ctx->keyfile) == 0) {
    free_global_memory(ctx, ctx_length);

    fprintf(stderr, "[!] Names keyfile and input files equal!\n");
    return (-1);
  }

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

    fprintf(stderr, "[!] Name cipher \"%s\" incorrect!\n", argv[1]);
    return (-1);
  }

  if (strcmp(argv[2], "-e") == 0 || strcmp(argv[2], "--encrypt") == 0) {
    ctx->operation = ENCRYPT;
  }
  else
  if (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "--decrypt") == 0) {
    ctx->operation = DECRYPT;
  }
  else {
    free_global_memory(ctx, ctx_length);

    fprintf(stderr, "[!] Operation \"%s\" incorrect!\n", argv[2]);
    return (-1);
  }

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

      fprintf(stderr, "[!] Key length \"%s\" incorrect!\n", argv[3]);
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
  printf("[DEBUG] block cipher mode of operation: CFB\n");
  printf("[DEBUG] key length: %u bist\n", ctx->temp_buffer_length);
  printf("[DEBUG] operation: %s\n", OPERATION_NAME[ctx->operation ? 1 : 0]);
#endif

  ctx->temp_buffer_length /= 8; /* for allocate memory */

  /*
    AES       = (temp_buffer_length = 16 or 24 or 32);
    SERPENT   = (temp_buffer_length = 16 or 24 or 32);
    TWOFISH   = (temp_buffer_length = 16 or 24 or 32);
    BLOWFISH  = (temp_buffer_length = 56);
    THREEFISH = (temp_buffer_length = 32 or 64 or 128);
  */

  ctx->temp_buffer = (uint8_t*)calloc(ctx->temp_buffer_length, 1);

  if (!ctx->temp_buffer) {
    free_global_memory(ctx, ctx_length);

    MEMORY_ERROR;
    return (-1);
  }

#if DEBUG_INFORMATION
  printf("[DEBUG] temp memory allocated: %u byte\n", ctx->temp_buffer_length);
  printf("[DEBUG] temp memory pointer: %p\n", ctx->temp_buffer);
#endif

  real_read = readfromfile(ctx->keyfile, ctx->temp_buffer, ctx->temp_buffer_length);

  if (real_read == (int)ctx->temp_buffer_length)
    printf("[#] Crypt key read from file \"%s\"!\n", ctx->keyfile);
  else
  if ((real_read > 0) && (real_read < (int)ctx->temp_buffer_length)) {

    printf("[!] Data in key file %d byte; necessary %d byte!\n",
            real_read, (int32_t)ctx->temp_buffer_length);

    free_global_memory(ctx, ctx_length);
    return (-1);
  }
  else
  if ((0 == real_read) || ((-1) == real_read)) {
    real_read = (int)x_strnlen(ctx->keyfile, 256);

    if ((real_read > 7) && (real_read < 257)) { /* Max password length = 256 byte; min = 8  */
      ctx->sha256sum_length = sizeof(SHA256_CTX);
      ctx->sha256sum        = (SHA256_CTX *)calloc(1, ctx->sha256sum_length);

#if DEBUG_INFORMATION
  printf("[DEBUG] size struct for sha256sum function: %u byte\n", ctx->sha256sum_length);
  printf("[DEBUG] sha256sum struct create in pointer: %p\n", ctx->sha256sum);
#endif

      if (ctx->sha256sum) {
        /* password -> crypt key; Pseudo PBKDF2 */
        KDFCLOMUL(ctx, (uint8_t *)(ctx->keyfile), real_read,
                  ctx->temp_buffer,
                  ctx->temp_buffer_length);

        printf("[#] Crypt key read from command line!\n");
      }
      else {
        free_global_memory(ctx, ctx_length);

        MEMORY_ERROR;
        return (-1);
      }
    }
    else {
      free_global_memory(ctx, ctx_length);

      fprintf(stderr, "[!] Data in string key %d byte; necessary 8..256 byte!\n", real_read);
      return (-1);
    }
  }

  printf("[#] Key length %d-bits initialized!\n", (int32_t)ctx->temp_buffer_length * 8);

#if DEBUG_INFORMATION
  printf("[DEBUG] key or password length: %d byte\n", real_read);
  printf("[DEBUG] key generator write data in pointer: %p\n", ctx->temp_buffer);
  printf("[DEBUG] real crypt key data:\n");

  printhex(HEX_TABLE, ctx->temp_buffer, ctx->temp_buffer_length);
#endif

  switch (ctx->cipher_number) {
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

  ctx->vector = (uint8_t*)calloc(ctx->vector_length, 1);

  if (!ctx->vector) {
    free_global_memory(ctx, ctx_length);

    MEMORY_ERROR;
    return (-1);
  }

  if (ENCRYPT == ctx->operation) {
    srand((unsigned int)time(NULL));

    if (vector_init(ctx->vector, ctx->vector_length) < (ctx->vector_length - 2)) {
      free_global_memory(ctx, ctx_length);

      fprintf(stderr, "[X] Critical error! System time stopped?\n");
      return (-1);
    }
  }

  if (AES == ctx->cipher_number) {
    rijndael_ctx = (uint32_t *) calloc(cipher_ctx_len, 1);
    cipher_pointer = (void *)rijndael_ctx;

    if (!rijndael_ctx) {
      free_global_memory(ctx, ctx_length);

      MEMORY_ERROR;
      return (-1);
    }
    /* This function return real iteration count encrypt operation */
    AES_Rounds = rijndael_key_encrypt_init(rijndael_ctx,
                                           ctx->temp_buffer,
                                           ctx->temp_buffer_length * 8);
  }
  else
  if (TWOFISH == ctx->cipher_number) {
    twofish_ctx = (TWOFISH_CTX *) calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)twofish_ctx;

    if (!twofish_ctx) {
      free_global_memory(ctx, ctx_length);

      MEMORY_ERROR;
      return (-1);
    }

    twofish_init(twofish_ctx, ctx->temp_buffer, ctx->temp_buffer_length);
  }
  else
  if (SERPENT == ctx->cipher_number) {
    serpent_ctx = (SERPENT_CTX *) calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)serpent_ctx;

    if (!serpent_ctx) {
      free_global_memory(ctx, ctx_length);

      MEMORY_ERROR;
      return (-1);
    }

    serpent_init(serpent_ctx, ctx->temp_buffer_length * 8, ctx->temp_buffer);
  }
  else
  if (BLOWFISH == ctx->cipher_number) {
    blowfish_ctx = (BLOWFISH_CTX *)calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)blowfish_ctx;

    if (!blowfish_ctx) {
      free_global_memory(ctx, ctx_length);

      MEMORY_ERROR;
      return (-1);
    }

    blowfish_init(blowfish_ctx, ctx->temp_buffer, ctx->temp_buffer_length);
  }
  else
  if (THREEFISH == ctx->cipher_number) {
    threefish_ctx = (THREEFISH_CTX *)calloc(1, cipher_ctx_len);
    cipher_pointer = (void *)threefish_ctx;

    if (!threefish_ctx) {
      free_global_memory(ctx, ctx_length);

      MEMORY_ERROR;
      return (-1);
    }

    threefish_init(threefish_ctx, (threefishkeysize_t)(ctx->temp_buffer_length * 8),
                   (uint64_t*)ctx->temp_buffer, (uint64_t*)ctx->temp_buffer);
  }

  printf("[#] Algoritm %s initialized!\n", ALGORITM_NAME[(ctx->cipher_number)]);

#if DEBUG_INFORMATION
  printf("[DEBUG] allocate byte for cipher struct: %u\n", cipher_ctx_len);
  printf("[DEBUG] real data cipher struct:\n");
  printhex(HEX_TABLE, cipher_pointer, cipher_ctx_len);
#endif

  printf("[#] Operation %s file \"%s\" started!\n",
    OPERATION_NAME[operation_variant(ctx->operation)],
    ctx->finput);

/*****************************************************************************/
  result = filecrypt(ctx);

  switch (result) {
    case OK:
      printf("[#] %s file \"%s\" complete!\n",
        OPERATION_NAME[operation_variant(ctx->operation)], ctx->finput);
      break;

    case READ_FILE_NOT_OPEN:
      fprintf(stderr, "[!] File for %s \"%s\" not opened!\n",
        OPERATION_NAME[operation_variant(ctx->operation)], ctx->finput);
      break;
    case WRITE_FILE_NOT_OPEN:
      fprintf(stderr, "[!] Output file \"%s\" not opened!\n", ctx->foutput);
      break;
    case SIZE_FILE_ERROR:
      fprintf(stderr, "[!] Size of input file \"%s\" 0 or more 2 GiB!\n", ctx->finput);
      break;
    case WRITE_FILE_ERROR:
      fprintf(stderr, "[!] Error write in file \"%s\" !\n", ctx->foutput);
      break;
    case READ_FILE_ERROR:
      fprintf(stderr, "[!] Error read form file \"%s\" !\n", ctx->finput);
      break;
    case STREAM_INPUT_CLOSE_ERROR:
      fprintf(stderr, "[!] Error close input stream!\n");
      break;
    case STREAM_OUTPUT_CLOSE_ERROR:
      fprintf(stderr, "[!] Error close output stream!\n");
      break;
    case SIZE_DECRYPT_FILE_INCORRECT:
      fprintf(stderr, "[!] Size of file for decrypt \"%s\" incorrect!\n", ctx->finput);
      break;
  }

/*****************************************************************************/

/* Clear all allocated memory for programm */

  cipher_free((void *)cipher_pointer, cipher_ctx_len);

  free_global_memory(ctx, ctx_length);

  return 0;
}
