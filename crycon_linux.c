/*
 * Plexus Technology Cybernetic Laboratory;
 * Console Encryption Software v5.10;
 *
 * Developer:         ARR0III;
 * Modification date: 31 JUL 2024;
 * Modification:      Release;
 * Language:          English;
 */

/* #define _FILE_OFFSET_BITS 64 */

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <sys/types.h>

#include <unistd.h>
#include <termios.h>

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
/*****************************************************************************/
#ifndef CRYCON_DEBUG
#  define CRYCON_DEBUG 0
#else
#  define CRYCON_DEBUG 1
#endif
/*****************************************************************************/
#define MEMORY_ERROR do { \
    fprintf(stderr, "[!] Cannot allocate memory.\n"); \
  } while(0)
/*****************************************************************************/
#define MINIMAL(a,b) (((a) < (b)) ? (a) : (b))
/*****************************************************************************/
#define KiB                          1
#define MiB                          2
#define GiB                          3
#define TiB                          4
#define PiB                          5
#define EiB                          6
/*****************************************************************************/
#define ERROR_TERMINAL               1
#define ERROR_SET_FLAG               2
#define ERROR_GET_STRING             3
/*****************************************************************************/
#define OK                           0
#define READ_FILE_NOT_OPEN           1
#define WRITE_FILE_NOT_OPEN          2
#define SIZE_FILE_ERROR              3
#define WRITE_FILE_ERROR             4
#define READ_FILE_ERROR              5
#define STREAM_INPUT_CLOSE_ERROR     6
#define STREAM_OUTPUT_CLOSE_ERROR    7
#define SIZE_DECRYPT_FILE_INCORRECT  8
#define REKEYING_PROCESS_ERROR       9
/*****************************************************************************/
#define LENGTH_DATA_FOR_CHECK     1024
/*****************************************************************************/
#define ENCRYPT                   0x00
#define DECRYPT                   0xDE
/*****************************************************************************/
#define PROGRESS_BAR_LENGTH         26
/*****************************************************************************/
#define STRING_MAX_LENGTH         2048
#define DATA_SIZE     ((1024*1024) * 8 /*MiB*/ ) /* READ AND WRITE FROM DRIVE */
/*****************************************************************************/
const char * PARAM_READ_BYTE  = "rb";
const char * PARAM_WRITE_BYTE = "wb";
const char * PROGRAMM_NAME    = "PlexusTCL Console Crypter 5.10 31JUL24 [EN]";

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

static const off_t INT_SIZE_DATA[] = {
  (off_t)1 << 10, /* KiB */
  (off_t)1 << 20, /* MiB */
  (off_t)1 << 30, /* GiB */
  (off_t)1 << 40, /* TiB */
  (off_t)1 << 50, /* PiB */
  (off_t)1 << 60  /* EiB */
};

static const char * CHAR_SIZE_DATA[] = {
  "Bt" ,
  "KiB",
  "MiB",
  "GiB",
  "TiB",
  "PiB",
  "EiB"
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

  uint8_t    * real_key;            /* real_key for temp key data */
  size_t       real_key_length;     /* size buffer for crypt key */

  uint8_t    * new_key;             /* raw data for re-keying */
  size_t       new_key_length;      /* always equal real_key_length */

  char       * password;            /* path keyfile or string key */
  size_t       password_length;

  SHA256_CTX * sha256sum;           /* memory for sha256 hash function */
  size_t       sha256sum_length;    /* size struct to pointer ctx->sha256sum */

  uint8_t    * vector;              /* initialized vector for crypt data */
  size_t       vector_length;       /* block size cipher execution */

  uint8_t      input  [DATA_SIZE];  /* memory for read */
  uint8_t      output [DATA_SIZE];  /* memory for write */

  uint8_t      progress_bar[PROGRESS_BAR_LENGTH];

  int          operation;           /* ENCRYPT == 0x00 or DECRYPT == 0xDE */
  cipher_t     cipher_number;       /* search type name cipher_number_enum */
} GLOBAL_MEMORY;

static void free_global_memory(GLOBAL_MEMORY * ctx, const size_t ctx_length) {
  if (ctx->real_key && ctx->real_key_length > 0) {
    meminit(ctx->real_key, 0x00, ctx->real_key_length);
  }

  if (ctx->new_key && ctx->new_key_length > 0) {
    meminit(ctx->new_key, 0x00, ctx->new_key_length);
  }

  if (ctx->password && ctx->password_length > 0) {
    meminit(ctx->password, 0x00, ctx->password_length);
  }

  if (ctx->sha256sum && ctx->sha256sum_length > 0) {
    meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
  }

  if (ctx->vector && ctx->vector_length > 0) {
    meminit(ctx->vector, 0x00, ctx->vector_length);
  }

  free(ctx->real_key);
  free(ctx->new_key);
  free(ctx->password);
  free(ctx->sha256sum);
  free(ctx->vector);
	
  /* clear all memory and all pointers */
  meminit(ctx, 0x00, ctx_length);
  free(ctx);
}

/* Function size_check checked size = Bt, KiB, MiB, GiB or TiB */
static int size_check(off_t size) {
  int result = 0;

  if (size >= INT_SIZE_DATA[0] && size < INT_SIZE_DATA[1]) {
    result = KiB;
  }
  else
  if (size >= INT_SIZE_DATA[1] && size < INT_SIZE_DATA[2]) {
    result = MiB;
  }
  else
  if (size >= INT_SIZE_DATA[2] && size < INT_SIZE_DATA[3]) {
    result = GiB;
  }
  else
  if (size >= INT_SIZE_DATA[3] && size < INT_SIZE_DATA[4]) {
    result = TiB;
  }
  else
  if (size >= INT_SIZE_DATA[4] && size < INT_SIZE_DATA[5]) {
    result = PiB;
  }
  else
  if (size >= INT_SIZE_DATA[5]) {
    result = EiB;
  }

  return result;
}

static double sizetodoubleprint(const int status, const double size) {
  return (status ? (size / (double)INT_SIZE_DATA[status - 1]) : size);
}

/* TESTING NEW KEY DERIVATION FUNCTION */
static void KDFCLOMUL2(GLOBAL_MEMORY * ctx,
                      const uint8_t  * password, const size_t password_len,
                            uint8_t  * key,      const size_t key_len) {

  uint32_t i, j;
  uint32_t count;

/*uint32_t pmem_size = (key_len + password_len + CLOMUL_CONST) * SHA256_BLOCK_SIZE*/
  uint32_t pmem_size = (key_len + password_len + CLOMUL_CONST) << 5;

  uint8_t * pmem = (uint8_t *)malloc(pmem_size);

  if (!pmem) {
    MEMORY_ERROR;
    exit(1);
  }

#if CRYCON_DEBUG
  printf("[DEBUG] pointer of key matrix memory: %p\n", pmem);
  printf("[DEBUG] size of key matrix memory byte: %u\n", pmem_size);

  srand(time(0));
  clock_t min = clock();
#endif

/*****************************************************************************/
/* GENERATION COUNTER */

  count  = CRC32(password, password_len);
  count  = count >> 16;
  count |= ((uint32_t)1 << 14);
  count *= CLOMUL_CONST;

#if CRYCON_DEBUG
  printf("[DEBUG] make crypt key count iteration: %u\n",
         (count * (pmem_size >> 5)) + (key_len >> 5));
#endif

/*****************************************************************************/
/* INITIALIZED MEMORY */
  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);

  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, password, password_len);
  sha256_final(ctx->sha256sum);

  for (i = 0; i < pmem_size; i += SHA256_BLOCK_SIZE) {
    memcpy(pmem + i, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
  }

  for (i = 0; i < pmem_size; i += SHA256_BLOCK_SIZE) {
    for (j = 0; j < count; j++) {
      sha256_update(ctx->sha256sum, password, password_len);
    }

    sha256_final(ctx->sha256sum);

    memxormove(pmem + i, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
  }

  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
/*****************************************************************************/
/* HASHING MEMORY FOR GENERATION CRYPT KEY */

  sha256_init(ctx->sha256sum);

  for (i = 0; i < key_len; i += SHA256_BLOCK_SIZE) {
    sha256_update(ctx->sha256sum, pmem, pmem_size);
    sha256_final(ctx->sha256sum);

    j = key_len - i;

    if (j > SHA256_BLOCK_SIZE) {
      j = SHA256_BLOCK_SIZE;
    }

    memcpy(key + i, ctx->sha256sum->hash, j);
  }
/*****************************************************************************/
#if CRYCON_DEBUG
  printf("[DEBUG] make crypt key time: %4.4f seconds\n", ((double)(clock() - min) / (double)CLOCKS_PER_SEC));
  printf("[DEBUG] hash matrix for key generation:\n");
  printhex(HEX_TABLE, pmem, pmem_size);
#endif

  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
  meminit(pmem, 0x00, pmem_size);
  free(pmem);
  pmem = NULL;
}

static void KDFCLOMUL(GLOBAL_MEMORY * ctx,
                     const uint8_t  * password, const size_t password_len,
                           uint8_t  * key,      const size_t key_len) {

  uint32_t i, j, k;
  uint32_t count = 0;

#if CRYCON_DEBUG
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

#if CRYCON_DEBUG
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

#if CRYCON_DEBUG
  printf("[DEBUG] make crypt key time: %4.4f seconds\n", ((double)(clock() - min) / (double)CLOCKS_PER_SEC));
#endif

  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
}

/* return encrypt, decrypt or stream */
static int operation_variant(const int operation) {
  return (operation ? 1 : 0);
}

static off_t size_of_file(FILE * f) {
  off_t result;

  if (!f) {
    return (-1);
  }
	
  if (fseeko(f, 0, SEEK_END) != 0) {
    return (-1);
  }

  result = ftello(f);

  if (fseeko(f, 0, SEEK_SET) != 0) {
    return (-1);
  }

  return result;
}

static void cipher_free(void * ctx, size_t ctx_length) {
  meminit(ctx, 0x00, ctx_length);
  free(ctx);
}

static void hmac_sha256_uf(GLOBAL_MEMORY * ctx) {
  size_t i;
  size_t size_copy_data;
  size_t hmac_ctx_length;

  HMAC_CTX * hmac_ctx;
	
  hmac_ctx_length = sizeof(HMAC_CTX);	
  hmac_ctx = (HMAC_CTX *)malloc(hmac_ctx_length);

  if (!hmac_ctx) {
    MEMORY_ERROR;
    exit(1);
  }

  size_copy_data = MINIMAL(ctx->real_key_length, SHA256_BLOCK_SIZE);

  /* copy hash sum file in local buffer "hash" */
  memcpy((void *)hmac_ctx->hash, (void *)(ctx->sha256sum->hash), SHA256_BLOCK_SIZE);

  /* generate two secret const for hash update */
  memcpy(hmac_ctx->KEY_0, ctx->real_key, size_copy_data);
  memcpy(hmac_ctx->KEY_1, ctx->real_key, size_copy_data);

  /* if length real_key equal or more SHA256_BLOCK_SIZE then cycle NOT executable */
  for (i = size_copy_data; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] = 0x00;
    hmac_ctx->KEY_1[i] = 0x00;
  }

  for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] ^= 0x55; /* simbol 'U', decimal  85, bits 01010101 */
    hmac_ctx->KEY_1[i] ^= 0x66; /* simbol 'f', decimal 102, bits 10101010 */
  }

#if CRYCON_DEBUG
  printf("[DEBUG] authentification key \'U\':\n");
  printhex(HEX_TABLE, hmac_ctx->KEY_0, SHA256_BLOCK_SIZE);

  printf("[DEBUG] authentification key \'f\':\n");
  printhex(HEX_TABLE, hmac_ctx->KEY_1, SHA256_BLOCK_SIZE);
#endif

  /* clear sha256sum struct */
  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);

  /* calculate hash for (key xor 0x55) and hash file */
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, hmac_ctx->KEY_0, SHA256_BLOCK_SIZE);
  sha256_update(ctx->sha256sum, hmac_ctx->hash, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  memcpy(hmac_ctx->hash, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);

  /* clear sha256sum struct */
  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);

  /* calculate hash for (key xor 0x66) and hash for ((key xor 0x55) and hash file) */
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, hmac_ctx->KEY_1, SHA256_BLOCK_SIZE);
  sha256_update(ctx->sha256sum, hmac_ctx->hash, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  /* clear memory for security */
  meminit(hmac_ctx, 0x00, hmac_ctx_length);
  free(hmac_ctx);
  /* now control sum crypt key and file in buffer ctx->sha256sum->hash */
}

static void control_sum_buffer(GLOBAL_MEMORY * ctx, const size_t count) {
  size_t i = 0;
  size_t remnant = count;

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
  
  return return_code; /* All files close complete. */
}

/* fsize += (size initialized vector + size sha256 hash sum) */
/* break operation if (fsize > 2 EiB) or (fsize == 0) or (fsize == -1) */
static int size_correct(const GLOBAL_MEMORY * ctx, off_t fsize) {
  if (0LL == fsize) {
    return SIZE_FILE_ERROR;
  }

  if (ENCRYPT == ctx->operation) {
/* if post encrypt size of file >= 4 EiB then this operation BAD ->> don't for decrypting */
    if ((off_t)(fsize + SHA256_BLOCK_SIZE + ctx->vector_length) & ((off_t)1 << 63)) {
      return SIZE_FILE_ERROR;
    }
  }
  else {
/* if fsize < minimal size, this file don't for decrypt */
    if (fsize < (off_t)(SHA256_BLOCK_SIZE + ctx->vector_length + 1)) {
      return SIZE_DECRYPT_FILE_INCORRECT;
    }
  }

  return OK;
}

/* generating new crypt key for re-keying */
static void internal_re_keying(GLOBAL_MEMORY * ctx) {
  SHA256_CTX sha;
  int i, size;

  meminit(&sha, 0x00, sizeof(SHA256_CTX));
  meminit(ctx->new_key, 0x00, ctx->new_key_length);

  sha256_init(&sha);

  for (i = 0; i < ctx->real_key_length; i += SHA256_BLOCK_SIZE) {
    sha256_update(&sha, ctx->real_key, ctx->real_key_length);
    sha256_final(&sha);

    size = ctx->real_key_length - i;

    if (size > SHA256_BLOCK_SIZE) {
      size = SHA256_BLOCK_SIZE;
    }

    memcpy(ctx->new_key + i, sha.hash, size);
  }

/* ctx->real_key_length and ctx->new_key_length always equal */
  memcpy(ctx->real_key, ctx->new_key, ctx->new_key_length);

  meminit(&sha, 0x00, sizeof(SHA256_CTX));
  meminit(ctx->new_key, 0x00, ctx->new_key_length);

/* now new crypt key in memory pointer ctx->real_key */

#if CRYCON_DEBUG
  printf("\n[DEBUG] by generation new crypt key:\n");
  printhex(HEX_TABLE, ctx->real_key, ctx->real_key_length);
#endif

/* re-keying for security */

  switch(ctx->cipher_number) {
    case AES:       rijndael_key_encrypt_init(rijndael_ctx,
                                              ctx->real_key,
                                              ctx->real_key_length * 8);

                    break;

    case TWOFISH:   twofish_init(twofish_ctx, ctx->real_key, ctx->real_key_length);
                    break;

    case SERPENT:   serpent_init(serpent_ctx, ctx->real_key_length * 8, ctx->real_key);
                    break;

    case BLOWFISH:  blowfish_init(blowfish_ctx, ctx->real_key, ctx->real_key_length);
                    break;

    case THREEFISH: threefish_init(threefish_ctx,
                                  (threefishkeysize_t)(ctx->real_key_length * 8),
                                  (uint64_t*)ctx->real_key,
                                  (uint64_t*)ctx->real_key);
                    break;
  }
}

static int filecrypt(GLOBAL_MEMORY * ctx) {
  FILE * fi = NULL;
  FILE * fo = NULL;

  double div, fsize_double;
  int    fsize_check, real_check = 0;
  int    real_percent, past_percent = 0;

  size_t re_keying = 0;
  size_t nblock, realread = 0;

  off_t fsize, position = 0;

  fi = fopen(ctx->finput, PARAM_READ_BYTE);

  if (!fi) {
    return READ_FILE_NOT_OPEN;
  }

  fsize       = size_of_file(fi);
  fsize_check = size_correct(ctx, fsize);

  if (fsize_check) { /* IF NOT OK */
    if (fclose(fi) == -1)
      return STREAM_INPUT_CLOSE_ERROR;
    else
      return fsize_check;
  }

  fsize_check = 0;

  fo = fopen(ctx->foutput, PARAM_WRITE_BYTE);

  if (!fo) {
    if (fclose(fi) == -1)
      return STREAM_INPUT_CLOSE_ERROR;
    else
      return WRITE_FILE_NOT_OPEN;
  }

  div          = (double)((double)fsize / 100.0);
  fsize_check  = size_check(fsize);
  fsize_double = sizetodoubleprint(fsize_check, (double)fsize);

#if CRYCON_DEBUG
  printf("[DEBUG] size of file: %4.2f %s\n", fsize_double, CHAR_SIZE_DATA[fsize_check]);
#endif

  meminit(ctx->progress_bar, '.', PROGRESS_BAR_LENGTH - 1);

  sha256_init(ctx->sha256sum);

  while (position < fsize) {
    if (0ULL == position) { /* if first block */

#if CRYCON_DEBUG
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

#if CRYCON_DEBUG
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
#if CRYCON_DEBUG
  printf("[DEBUG] vector data read from file in pointer: %p\n", ctx->vector);
#endif
      }

#if CRYCON_DEBUG
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

       position  += (off_t)realread;
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
          OPERATION_NAME[operation_variant(ctx->operation)], ctx->progress_bar,
          sizetodoubleprint(real_check, (double)position),
          CHAR_SIZE_DATA[real_check],  fsize_double,
          CHAR_SIZE_DATA[fsize_check], real_percent);

        fflush(stdout);
      /* } */
      past_percent = real_percent;
    }

    re_keying += realread;

/*  if crypt key using for en/decrypt very long time --> regenerating crypt key
    limit en/decrypt bytes = 2 GB
    new_key = sha256sum(old_key);
*/
    if (re_keying >= 0x80000000) {
      re_keying = 0;
      internal_re_keying(ctx);
    }
  }

  putc('\n', stdout);

  sha256_final(ctx->sha256sum);

#if CRYCON_DEBUG
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
    if (memcmp(ctx->input + realread, ctx->sha256sum->hash, SHA256_BLOCK_SIZE) != 0) {
      printf("[!] WARNING: Control sum file \"%s\" not correct!\n", ctx->finput);
    }
    else {
      printf("[#] Control sum file \"%s\" check. :)\n", ctx->finput);
    }
  }

#if CRYCON_DEBUG
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
  ARC4_CTX * arc4_memory  = NULL;

  vector_memory = (uint8_t *)malloc(vector_size);
  arc4_memory   = (ARC4_CTX *)malloc(arc4_size);
  
  if (!arc4_memory || !vector_memory) {

    free(vector_memory);
    free(arc4_memory);

    MEMORY_ERROR;
    exit(1);
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

void vector_init(uint8_t * data, size_t size) {
  size_t i;
  
  FILE * furand = fopen("/dev/urandom", "rb");

  if (furand) {
    fread(data, 1, size, furand);
    fclose(furand);

#if CRYCON_DEBUG
  printf("[DEBUG] data from device \"/dev/urandom\":\n");
  printhex(HEX_TABLE, data, size);
#endif
  }
  else {
    fprintf(stderr, "[!] Device \"/dev/urandom\" not openned!\n");
  }

  for (i = 0; i < size; i++) {
    data[i] ^= (uint8_t)i ^ (uint8_t)genrand(0x00, 0xFF);
  }

  random_vector_init(data, size);
}

static void * cipher_init_memory(GLOBAL_MEMORY * ctx, size_t cipher_len) {

  void * cipher_ptr = (void *)calloc(cipher_len, 1);

  if (!cipher_ptr) {
    return NULL;
  }

  switch(ctx->cipher_number) {
    case AES:       { rijndael_ctx = (uint32_t *)cipher_ptr;
                      rijndael_key_encrypt_init(rijndael_ctx,
                                                ctx->real_key,
                                                ctx->real_key_length * 8);
                    }
                    break;

    case TWOFISH:   { twofish_ctx = (TWOFISH_CTX *)cipher_ptr;
                      twofish_init(twofish_ctx, ctx->real_key, ctx->real_key_length);
                    }
                    break;

    case SERPENT:   { serpent_ctx = (SERPENT_CTX *)cipher_ptr;
                      serpent_init(serpent_ctx, ctx->real_key_length * 8, ctx->real_key);
                    }
                    break;

    case BLOWFISH:  { blowfish_ctx = (BLOWFISH_CTX *)cipher_ptr;
                      blowfish_init(blowfish_ctx, ctx->real_key, ctx->real_key_length);
                    }
                    break;

    case THREEFISH: { threefish_ctx = (THREEFISH_CTX *)cipher_ptr;
                      threefish_init(threefish_ctx,
                                  (threefishkeysize_t)(ctx->real_key_length * 8),
                                  (uint64_t*)ctx->real_key,
                                  (uint64_t*)ctx->real_key);
                    }
                    break;
  }

  return cipher_ptr;
}

void PRINT_OPERATION_STATUS(GLOBAL_MEMORY * ctx, int result) {
  switch (result) {
    case OK:
      printf("[#] %s file \"%s\" complete.\n",
        OPERATION_NAME[operation_variant(ctx->operation)], ctx->finput);
      break;

    case READ_FILE_NOT_OPEN:
      fprintf(stderr, "[!] File for %s \"%s\" not opened.\n",
        OPERATION_NAME[operation_variant(ctx->operation)], ctx->finput);
      break;
    case WRITE_FILE_NOT_OPEN:
      fprintf(stderr, "[!] Output file \"%s\" not opened.\n", ctx->foutput);
      break;
    case SIZE_FILE_ERROR:
      fprintf(stderr, "[!] Size of input file \"%s\" 0 or more 4 EiB.\n", ctx->finput);
      break;
    case WRITE_FILE_ERROR:
      fprintf(stderr, "[!] Error write in file \"%s\" .\n", ctx->foutput);
      break;
    case READ_FILE_ERROR:
      fprintf(stderr, "[!] Error read form file \"%s\" .\n", ctx->finput);
      break;
    case STREAM_INPUT_CLOSE_ERROR:
      fprintf(stderr, "[!] Error close input stream.\n");
      break;
    case STREAM_OUTPUT_CLOSE_ERROR:
      fprintf(stderr, "[!] Error close output stream.\n");
      break;
    case SIZE_DECRYPT_FILE_INCORRECT:
      fprintf(stderr, "[!] Size of file for decrypt \"%s\" incorrect.\n", ctx->finput);
      break;
    case REKEYING_PROCESS_ERROR:
      fprintf(stderr, "[!] Cannot allocate memory for re-keying.\n");
      break;
  }
}

int password_read(GLOBAL_MEMORY * ctx) {
  int i;
  struct termios trms, trms_old;

  tcgetattr(0, &trms);                    /* get settings termios system */
  memcpy(&trms_old, &trms, sizeof(trms)); /* copy normal settings */
  trms.c_lflag &= ~ECHO;                  /* flush flag ECHO */
  tcsetattr(0, TCSANOW, &trms);           /* set new settings */
/*
  tcgetattr(0, &trms);

  if (trms.c_lflag & ECHO) {
    fprintf(stderr, "[X] Not are set ECHO flag for Termios!\n");
    tcsetattr(0, TCSANOW, &trms_old);
    return ERROR_SET_FLAG;
  }
*/
  fflush(stdin);
  fflush(stdout);
  printf("[$] Enter password or name keyfile:");

  if (!fgets(ctx->password, ctx->password_length, stdin)) {
    fprintf(stderr, "[X] Password not read from command line.\n");
    return ERROR_GET_STRING;
  }

  for (i = 0; i < (ctx->password_length); i++) {
    if ('\n' == ctx->password[i]) {
      ctx->password[i] = '\0';
      break;
    }
  }

  tcsetattr(0, TCSANOW, &trms_old); /* reability settings */
  putc('\n', stdout);

  return OK;
}

int INITIALIZED_GLOBAL_MEMORY(GLOBAL_MEMORY ** ctx, size_t ctx_size) {
  *ctx = (GLOBAL_MEMORY *)malloc(ctx_size);

  if (NULL == *ctx) {
    return 1;
  }

  meminit(*ctx, 0x00, ctx_size);

  (*ctx)->finput             = NULL;
  (*ctx)->foutput            = NULL;
  (*ctx)->vector             = NULL;
  (*ctx)->real_key           = NULL;

  (*ctx)->operation          = ENCRYPT;
  (*ctx)->cipher_number      = AES;

  (*ctx)->new_key_length     = 0;
  (*ctx)->vector_length      = 0;
  (*ctx)->real_key_length    = 0;
  (*ctx)->password_length    = STRING_MAX_LENGTH;
  (*ctx)->sha256sum_length   = sizeof(SHA256_CTX);

  (*ctx)->password = (char *)malloc((*ctx)->password_length);

  if (NULL == (*ctx)->password) {
    return 2;
  }

  (*ctx)->sha256sum = (SHA256_CTX *)malloc((*ctx)->sha256sum_length);

  if (NULL == (*ctx)->sha256sum) {
    return 3;
  }

  meminit((*ctx)->password,  0x00, (*ctx)->password_length);
  meminit((*ctx)->sha256sum, 0x00, (*ctx)->sha256sum_length);

  return OK;
}

int main(int argc, char * argv[]) {
/*****************************************************************************/
  unsigned int trash;         /* not initialized == all control */
  extern int AES_Rounds;      /* in rijndael.c source code file */
  int i, real_read, result;
  size_t ctx_length, cipher_ctx_len = 0;

  GLOBAL_MEMORY * ctx;
  void * cipher_pointer;
/*****************************************************************************/
  srand(trash + time(NULL));
/*****************************************************************************/
  if (!isatty(0)) {
    fprintf(stderr, "[X] Not terminal!\n");
    return ERROR_TERMINAL;
  }
/*****************************************************************************/
  ctx_length = sizeof(GLOBAL_MEMORY);
  result = INITIALIZED_GLOBAL_MEMORY(&ctx, ctx_length);

  if (result != OK) {
    free_global_memory(ctx, ctx_length);
    MEMORY_ERROR;
    return result;
  }
/*****************************************************************************/
  if (argc > 1 && argc < 7) {
    for (i = 1; i < (argc - 1); i++) {
      if (x_strnlen(argv[i], STRING_MAX_LENGTH) == STRING_MAX_LENGTH) { /* if length argument >= 2048 */
        fprintf(stderr, "[!] Warning: argument \"%d\" length more \"%d\"!\n", i, STRING_MAX_LENGTH);
        return 1;
      }
    }
  }
  else
  if (1 == argc || argc >= 7) {
    fprintf(stderr, "[!] Error: count arguments %d; necessary to 2 do 7 strings.\n", argc);
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
      	     " [key length] [input filename] [output filename]\n", argv[0]);
      return 0;
    }
    else {
      fprintf(stderr, "[!] Incorrect parameter \"%s\"! Only \"-h\" or \"--help\".\n", argv[1]);
      return 1;
    }
  }

  if (argc < 4) {
    fprintf(stderr, "[!] Error: count arguments %d; necessary to 2 do 7 strings.\n", argc);
    return 0;
  }

  ctx->foutput = argv[argc - 1];
  ctx->finput  = argv[argc - 2];

#if CRYCON_DEBUG
  printf("[DEBUG] compile date and time:   %s, %s\n", __DATE__, __TIME__);
  printf("[DEBUG] global memory allocated: %u byte\n", ctx_length);
  printf("[DEBUG] global memory pointer:   %p\n", ctx);
  printf("[DEBUG] size struct for sha256sum function: %u byte\n", ctx->sha256sum_length);
  printf("[DEBUG] sha256sum struct create in pointer: %p\n", ctx->sha256sum);
  printf("[DEBUG] maximal length password or name keyfile: %u byte\n", ctx->password_length);
  printf("[DEBUG] pointer of memory for password or name keyfile: %p\n", ctx->password);
#endif

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

    fprintf(stderr, "[!] Name cipher \"%s\" incorrect.\n", argv[1]);
    return 1;
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

    fprintf(stderr, "[!] Operation \"%s\" incorrect.\n", argv[2]);
    return 1;
  }

  if (AES     == ctx->cipher_number ||
      SERPENT == ctx->cipher_number ||
      TWOFISH == ctx->cipher_number) { // AES, SERPENT, TWOFISH

    if (strcmp(argv[3], "-a") == 0 || strcmp(argv[3], "--128") == 0) {
      if (AES == ctx->cipher_number) {
        AES_Rounds = 10;
      }
      ctx->real_key_length = 128;
    }
    else
    if (strcmp(argv[3], "-b") == 0 || strcmp(argv[3], "--192") == 0) {
      if (AES == ctx->cipher_number) {
        AES_Rounds = 12;
      }
      ctx->real_key_length = 192;
    }
    else
    if (strcmp(argv[3], "-c") == 0 || strcmp(argv[3], "--256") == 0) {
      if (AES == ctx->cipher_number) {
        AES_Rounds = 14;
      }
      ctx->real_key_length = 256;
    }
    else {
      free_global_memory(ctx, ctx_length);

      fprintf(stderr, "[!] Key length \"%s\" incorrect.\n", argv[3]);
      return 1;
    }
  }
  else
  if (BLOWFISH  == ctx->cipher_number) {
    ctx->real_key_length = 448;
  }
  else
  if (THREEFISH == ctx->cipher_number) {
    if (strcmp(argv[3], "-a") == 0 || strcmp(argv[3], "--256") == 0) {
      ctx->real_key_length = 256;
    }
    else
    if (strcmp(argv[3], "-b") == 0 || strcmp(argv[3], "--512") == 0) {
      ctx->real_key_length = 512;
    }
    else
    if (strcmp(argv[3], "-c") == 0 || strcmp(argv[3], "--1024") == 0) {
      ctx->real_key_length = 1024;
    }
    else {
      free_global_memory(ctx, ctx_length);

      fprintf(stderr, "[!] Key length \"%s\" incorrect.\n", argv[3]);
      return 1;
    }
  }

  if (password_read(ctx) != OK) {
    free_global_memory(ctx, ctx_length);
    return 1;
  }

  if (strcmp(ctx->finput, ctx->foutput) == 0) {
    free_global_memory(ctx, ctx_length);

    fprintf(stderr, "[!] Names input and output files equal.\n");
    return 1;
  }
  else
  if (strcmp(ctx->foutput, ctx->password) == 0) {
    free_global_memory(ctx, ctx_length);

    fprintf(stderr, "[!] Names keyfile and output files equal.\n");
    return 1;
  }
  else
  if (strcmp(ctx->finput, ctx->password) == 0) {
    free_global_memory(ctx, ctx_length);

    fprintf(stderr, "[!] Names keyfile and input files equal.\n");
    return 1;
  }

#if CRYCON_DEBUG
  printf("[DEBUG] input filename:      %s\n", ctx->finput);
  printf("[DEBUG] output filename:     %s\n", ctx->foutput);
  printf("[DEBUG] keyfile or password: %s\n", ctx->password);
  printf("[DEBUG] cipher: %s\n", ALGORITM_NAME[ctx->cipher_number]);
  printf("[DEBUG] block cipher mode of operation: CFB\n");
  printf("[DEBUG] key length: %u bist\n", ctx->real_key_length);
  printf("[DEBUG] operation: %s\n", OPERATION_NAME[ctx->operation ? 1 : 0]);
#endif

  ctx->real_key_length >>= 3; /* for allocate memory; (rkl div 8) */

/*
  AES       = (real_key_length = 16 or 24 or 32);
  SERPENT   = (real_key_length = 16 or 24 or 32);
  TWOFISH   = (real_key_length = 16 or 24 or 32);
  BLOWFISH  = (real_key_length = 56);
  THREEFISH = (real_key_length = 32 or 64 or 128);
*/

  ctx->real_key = (uint8_t *)calloc(ctx->real_key_length, 1);

  if (!ctx->real_key) {
    free_global_memory(ctx, ctx_length);

    MEMORY_ERROR;
    return 1;
  }

  ctx->new_key_length = ctx->real_key_length;
  ctx->new_key = (uint8_t *)malloc(ctx->new_key_length);

  if (!ctx->new_key) {
    free_global_memory(ctx, ctx_length);

    MEMORY_ERROR;
    return 1;
  }

  meminit(ctx->new_key, 0x00, ctx->new_key_length);

#if CRYCON_DEBUG
  printf("[DEBUG] temp memory allocated: %u byte\n", ctx->real_key_length);
  printf("[DEBUG] temp memory pointer: %p\n", ctx->real_key);
#endif
/*****************************************************************************/
  real_read = readfromfile(ctx->password, ctx->real_key, ctx->real_key_length);

  if (real_read == (int)ctx->real_key_length)
    printf("[#] Crypt key read from file \"%s\".\n", ctx->password);
  else
  if ((real_read > 0) && (real_read < (int)ctx->real_key_length)) {

    printf("[!] Data in key file %d byte; necessary %d byte.\n",
            real_read, (int32_t)ctx->real_key_length);

    free_global_memory(ctx, ctx_length);
    return 1;
  }
  else
  if ((0 == real_read) || ((-1) == real_read)) {
    real_read = (int)x_strnlen(ctx->password, 256);

    if ((real_read > 7) && (real_read < 257)) { /* Max password length = 256 byte; min = 8  */
      /* password --> crypt key; Pseudo PBKDF2 */
      KDFCLOMUL(ctx, (uint8_t *)(ctx->password), real_read,
                ctx->real_key,
                ctx->real_key_length);

      meminit(ctx->password, 0x00, ctx->password_length);
      ctx->password_length = 0;

      printf("[#] Crypt key read from command line.\n");
    }
    else {
      free_global_memory(ctx, ctx_length);

      fprintf(stderr, "[!] Data in string key %d byte; necessary 8..256 byte.\n", real_read);
      return 1;
    }
  }
/*****************************************************************************/
  printf("[#] Key length %d-bits initialized.\n",
    (int32_t)ctx->real_key_length << 3); /* (rkl * 8) */

#if CRYCON_DEBUG
  printf("[DEBUG] key or password length: %d byte\n", real_read);
  printf("[DEBUG] key generator write data in pointer: %p\n", ctx->real_key);
  printf("[DEBUG] real crypt key data:\n");

  printhex(HEX_TABLE, ctx->real_key, ctx->real_key_length);
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
      ctx->vector_length = ctx->real_key_length;
      cipher_ctx_len = sizeof(THREEFISH_CTX);
      break;
  }

  ctx->vector = (uint8_t*)calloc(ctx->vector_length, 1);

  if (!ctx->vector) {
    free_global_memory(ctx, ctx_length);

    MEMORY_ERROR;
    return 1;
  }

  if (ENCRYPT == ctx->operation) {
#if CRYCON_DEBUG
  printf("[DEBUG] stack_trash: %02x\n", trash);
#endif
    /* random data from stack xor initialized vector */
    (*(uint32_t *)ctx->vector) ^= trash + (uint32_t)genrand(0x00000000, 0xFFFFFFFF);

    vector_init(ctx->vector, ctx->vector_length);
  }

  cipher_pointer = cipher_init_memory(ctx, cipher_ctx_len);

  if (!cipher_pointer) {
    free_global_memory(ctx, ctx_length);

    MEMORY_ERROR;
    return 1;
  }

  printf("[#] Algoritm %s initialized.\n", ALGORITM_NAME[(ctx->cipher_number)]);

#if CRYCON_DEBUG
  printf("[DEBUG] allocate byte for cipher struct: %u\n", cipher_ctx_len);
  printf("[DEBUG] real data cipher struct:\n");
  printhex(HEX_TABLE, cipher_pointer, cipher_ctx_len);
#endif

  printf("[#] Operation %s file \"%s\" started.\n",
    OPERATION_NAME[operation_variant(ctx->operation)], ctx->finput);

/*****************************************************************************/
/* STARTING ENCRYPT/DECRYPT OPERATION AND RETURN STATUS */

  result = filecrypt(ctx);
  PRINT_OPERATION_STATUS(ctx, result);

/*****************************************************************************/
/* FULL CLEAR MEMORY THIS PROGRAMM */

  cipher_free(cipher_pointer, cipher_ctx_len);
  free_global_memory(ctx, ctx_length);
	
  cipher_pointer = NULL;
  ctx = NULL;

  cipher_ctx_len = 0;
  ctx_length = 0;
	
  return 0;
}


