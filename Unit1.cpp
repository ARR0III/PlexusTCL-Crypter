#include <io.h>
#include <vcl.h>
#include <windows.h>
#include <wincrypt.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <time.h>

#include "src/arc4.h"      /* only for password generator */
#include "src/serpent.h"
#include "src/twofish.h"
#include "src/rijndael.h"
#include "src/blowfish.h"
#include "src/threefish.h"

#include "src/crc32.h"
#include "src/sha256.h"
#include "src/base64.h"

#include "src/xtalw.h"
#include "src/clomul.h"

/****************************************************************************/
/* DEFINED THIS SYMBOL IF YOU WANT COMPILE SOFTWARE WITCH RUSSIAN LANGUAGE  */
/* #define PTCL_RUSSIAN_LANGUAGE                                            */
#include "LANGUAGE_STRINGS.h"
/****************************************************************************/

#include "Unit1.h"
#include "Unit2.h"

#include "LICENSE.h"

#define SETTINGS_FILENAME  "settings.cry"
#define EMPTY_FILENAME     "output.dat"
#define EXT_CRYCON         ".crycon"
#define CIPHER_MODE        "-CFB"

#define KiB                          1
#define MiB                          2
#define GiB                          3
#define TiB                          4
#define PiB                          5
#define EiB                          6

#define OK                           0
#define READ_FILE_NOT_OPEN           1
#define WRITE_FILE_NOT_OPEN          2
#define READ_FILE_ERROR              3
#define WRITE_FILE_ERROR             4
#define STREAM_INPUT_CLOSE_ERROR     5
#define STREAM_OUTPUT_CLOSE_ERROR    6
#define SIZE_FILE_EMPTY              7
#define SIZE_FILE_VERY_BIG           8
#define SIZE_DECRYPT_FILE_INCORRECT  9
#define OPERATION_BREAK             10

#define STATUS_BUFFER_SIZE         256

#define SIZE_PASSWORD_GENERATE     512
#define BLOCK_SIZE_FOR_ERASED      512

#define LENGTH_DATA_FOR_CHECK     1024

#define ENCRYPT                   0x00
#define DECRYPT                   0xDE

#define STRINGS_COUNT_LIMIT      10000

#define DATA_SIZE         (1024*1024*8) /* 8 MiB */

#define MINIMAL(a,b) (((a) < (b)) ? (a) : (b))

#define SET_START_STREAM                  \
  do {                                    \
    EnterCriticalSection(&Form1->CrSec);  \
    PROCESSING = true;                    \
    LeaveCriticalSection(&Form1->CrSec);  \
  } while(0)

#define SET_STOP_STREAM                   \
  do {                                    \
    EnterCriticalSection(&Form1->CrSec);  \
    PROCESSING = false;                   \
    LeaveCriticalSection(&Form1->CrSec);  \
  } while(0)

#define CHECK_BREAK_STREAM                                  \
  do {                                                      \
    if (TryEnterCriticalSection(&Form1->CrSec)) {           \
      if (false == PROCESSING) {                            \
        LeaveCriticalSection(&Form1->CrSec);                \
        meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));  \
        return close_in_out_files(fi, fo, OPERATION_BREAK); \
      }                                                     \
                                                            \
      LeaveCriticalSection(&Form1->CrSec);                  \
    }                                                       \
  } while(0)

#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

HCRYPTPROV hcrypt      = NULL;
static bool PROCESSING = false;

typedef int64_t fsize_t;

typedef enum cipher_number_enum {
  AES       = 0,
  BLOWFISH  = 1,
  SERPENT   = 2,
  THREEFISH = 3,
  TWOFISH   = 4,
} cipher_t;

typedef enum config_enum {
  CIPHER           = 0,
  ERASED           = 1,
  KEY_SIZE         = 2,
  OPERATION        = 3,
  PASS_GEN_SIZE    = 4,
  TOP_COLOR        = 5,
  TOP_TEXT_B_COLOR = 6,
  TOP_TEXT_COLOR   = 7
} config_key;

typedef struct {
  TColor   top_color;
  TColor   top_text_color;
  TColor   top_text_b_color;
  cipher_t cipher;

  int  key_size;
  int  pass_gen_size;
  int  operation;

  bool erased;
} SETTINGS;

typedef struct {
  uint8_t hash[SHA256_BLOCK_SIZE];
  uint8_t KEY_0[SHA256_BLOCK_SIZE];
  uint8_t KEY_1[SHA256_BLOCK_SIZE];
} HMAC_CTX;

typedef struct {
  uint8_t input  [SIZE_PASSWORD_GENERATE];
  uint8_t output [SIZE_PASSWORD_GENERATE];
} PASSWORD_MEMORY_CTX;

typedef struct {
/* pointers for */
  SHA256_CTX * sha256sum;          /* memory for sha256 hash function */

  uint8_t    * hash_matrix;
  size_t       hash_matrix_length;

  uint8_t    * vector;             /* initialized vector for crypt data */
  size_t       vector_length;      /* block size cipher execution */

  uint8_t    * real_key;           /* real_key for temp key data */
  size_t       real_key_length;    /* size buffer for crypt key */

  uint8_t    * new_key;            /* real_key for temp key data */
  size_t       new_key_length;     /* size buffer for crypt key */

  int          operation;          /* ENCRYPT == 0x00 or DECRYPT == 0xDE */
  cipher_t     cipher_number;      /* search type name cipher_number_enum */
  
  uint8_t      input  [DATA_SIZE]; /* memory for read */
  uint8_t      output [DATA_SIZE]; /* memory for write */
} GLOBAL_MEMORY;

/* Only sorted strings for bin search function */
#define CONFIG_KEYS_COUNT 8
static const char * CONFIG_KEYS[CONFIG_KEYS_COUNT] = {
  "CIPHER",
  "ERASED",
  "KEY_SIZE",
  "OPERATION",
  "PASS_GEN_SIZE",
  "TOP_COLOR",
  "TOP_TEXT_B_COLOR",
  "TOP_TEXT_COLOR"
};

#define ALGORITM_NAME_COUNT 5
static const char * ALGORITM_NAME[ALGORITM_NAME_COUNT] = {
  "AES",
  "BLOWFISH",
  "SERPENT",
  "THREEFISH",
  "TWOFISH"
};

static const char *CHAR_SIZE_DATA[] = {
#ifdef PTCL_RUSSIAN_LANGUAGE
  "бт", "КиБ", "МиБ", "ГиБ", "ТиБ", "ПиБ", "ЭиБ"
#else
  "bt" , "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"	
#endif
};

static const char * OPERATION_NAME[] = {
#ifdef PTCL_RUSSIAN_LANGUAGE
  "Шифрование", "Расшифровка",
#else
  "Encrypting", "Decrypting",
#endif
};

#define CHAR_KEY_LENGTH_AES_COUNT 3
static const char * CHAR_KEY_LENGTH_AES[CHAR_KEY_LENGTH_AES_COUNT] = {
  "128",
  "192",
  "256"
};

#define CHAR_KEY_LENGTH_THREEFISH_COUNT 3
static const char * CHAR_KEY_LENGTH_THREEFISH[CHAR_KEY_LENGTH_THREEFISH_COUNT] = {
  "256",
  "512",
  "1024"
};

static const char * PARAM_APPEND_BYTE  = "ab";
static const char * PARAM_READ_BYTE    = "rb";
static const char * PARAM_WRITE_BYTE   = "wb";
static const char * PARAM_REWRITE_BYTE = "r+b";

static const TColor FORM_HEAD_COLOR        = TColor(0x00000000);
static const TColor FORM_HEAD_TEXT_B_COLOR = TColor(0x00000000);
static const TColor FORM_HEAD_TEXT_COLOR   = TColor(0x00FFFFFF);

static const fsize_t INT_SIZE_DATA[] = {
  (fsize_t)1 << 10, /* KiB */
  (fsize_t)1 << 20, /* MiB */
  (fsize_t)1 << 30, /* GiB */
  (fsize_t)1 << 40, /* TiB */
  (fsize_t)1 << 50, /* PiB */
  (fsize_t)1 << 60  /* EiB */
};

/****************************************************************************/

static uint32_t      * rijndael_ctx  = NULL;
static SERPENT_CTX   * serpent_ctx   = NULL;
static TWOFISH_CTX   * twofish_ctx   = NULL;
static BLOWFISH_CTX  * blowfish_ctx  = NULL;
static THREEFISH_CTX * threefish_ctx = NULL;

void ShowBuffer(void * buffer, const size_t buffer_size) {
  size_t debug_buffer_size = buffer_size * 2 + 1;
  char * debug_buffer = (char *)malloc(debug_buffer_size);

  if (!debug_buffer) return;

  memset(debug_buffer, 0x00, debug_buffer_size);
  strtohex(debug_buffer, debug_buffer_size, (char *)buffer, buffer_size);
  debug_buffer[debug_buffer_size-1] = (char)0;
  
  ShowMessage(debug_buffer);

  free(debug_buffer);
}

uint32_t MessageForUser(const int tumbler,
                        const char * head,
                        const char * message) {

  return Application->MessageBox(message, head, tumbler);
}

int close_in_out_files(FILE * file_input, FILE * file_output, const int return_code) {
  if (fclose(file_input) == -1) {
    fclose(file_output);
    return STREAM_INPUT_CLOSE_ERROR;
  }
  
  if (fclose(file_output) == -1) {
    return STREAM_OUTPUT_CLOSE_ERROR;
  }
  
  return return_code; /* All files close complete! */
}

int operation_variant(const int operation) {
  return (operation ? 1 : 0);
}

int str_list_search(const char * str, const char * list[], int length) {
  int found;
  int pos, left, right, result;

  found = -1;

  left  = 0;
  right = length - 1;
  pos   = (left + right) >> 1; /* (left + right) div 2 */

  while (left <= right) {
    result = strcmp(str, list[pos]);

    if (result == 0) {
      found = pos;
      break;
    }

    if (result < 0) {
      right = pos - 1;
    }
    else
    if (result > 0) {
      left = pos + 1;
    }

    pos = (left + right) >> 1;
  }

  return found;
}

void pars_str(const char * key, const char * data, SETTINGS * settings) {
  size_t result;	
  int key_found;
  int data_found;
	
  key_found = str_list_search(key, CONFIG_KEYS, CONFIG_KEYS_COUNT); /* -1 == not found string in list */

  if (key_found != -1) {
    result = atoi(data);
	  
    switch (key_found) {
      case CIPHER:
        data_found = str_list_search(data, ALGORITM_NAME, ALGORITM_NAME_COUNT);
        if (data_found != -1) {
          settings->cipher = (cipher_t)data_found;
        }
        break;

      case ERASED:
        if (strcmp(data, "TRUE") == 0) {
          settings->erased = true;
        }
        else
        if (strcmp(data, "FALSE") == 0) {
          settings->erased = false;
        }
        break;

      case KEY_SIZE:
        if (settings->cipher == AES || settings->cipher == TWOFISH || settings->cipher == SERPENT) {
          if (result == 128 || result == 192 || result == 256) {
            settings->key_size = result;
          }
        }
        else
        if  (settings->cipher == THREEFISH ) {
          if (result == 256 || result == 512 || result == 1024) {
            settings->key_size = result;
          }
        }
        break;
	   
      case OPERATION:
        if (strcmp(data, "ENCRYPT") == 0) {
          settings->operation = ENCRYPT;
        }
        else
        if (strcmp(data, "DECRYPT") == 0) {
          settings->operation = DECRYPT;
        }
        break;

      case PASS_GEN_SIZE:
        if (result > 7 && result < 257) {
          settings->pass_gen_size = result;
        }
        break;

      case TOP_COLOR:
        settings->top_color = (TColor)HexToInt32(data);
        break;

      case TOP_TEXT_B_COLOR:
        settings->top_text_b_color = (TColor)HexToInt32(data);
        break;

      case TOP_TEXT_COLOR:
        settings->top_text_color = (TColor)HexToInt32(data);
        break;
    }
  }
}

void init_settings(const char * filename, SETTINGS * settings) {
#define SETTINGS_BLOCK_SIZE 128
  FILE *fs;
  char *key, *data;
  int  realread, strcount;
  char buffer[SETTINGS_BLOCK_SIZE];

  if (NULL == filename || NULL == settings) {
    return;
  }

  fs = fopen(filename, PARAM_READ_BYTE);
  
  if (NULL == fs) {
    return;
  }

  strcount = 0;

  while (1) {
    realread = readstr(buffer, SETTINGS_BLOCK_SIZE, fs);
	
    if (realread < 1) {
      break;
    }
    
    strcount++;

    if (strcount >= STRINGS_COUNT_LIMIT) { /* max read strings from settings file */
      break;
    }

    key = buffer;
    data = strchr(buffer, '=');
	
    if (NULL == data) { /* this is not parameter  */
      continue;
    }

    if (key >= data){
      continue;
    }
	
    *data = '\0';
    data = data + 1;

    if (*key == '\0' || *data == '\0') {
      continue;
    }

    pars_str(key, data, settings);
    buffer[0] = '\0';
  }

  fclose(fs);
}

static void free_global_memory(GLOBAL_MEMORY * ctx, const size_t ctx_length) {
  if (NULL == ctx) {
    return;
  }

  if (ctx->hash_matrix && ctx->hash_matrix_length > 0) {
    meminit(ctx->hash_matrix, 0x00, ctx->hash_matrix_length);
  }

  if (ctx->real_key && ctx->real_key_length > 0) {
    meminit(ctx->real_key, 0x00, ctx->real_key_length);
  }

  if (ctx->new_key && ctx->new_key_length > 0) {
    meminit(ctx->new_key, 0x00, ctx->new_key_length);
  }

  if (ctx->sha256sum) {
    meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  }

  if (ctx->vector && ctx->vector_length > 0) {
    meminit(ctx->vector, 0x00, ctx->vector_length);
  }

  free(ctx->hash_matrix);
  free(ctx->real_key);
  free(ctx->new_key);
  free(ctx->sha256sum);
  free(ctx->vector);

  /* clear all memory and all pointers */
  meminit(ctx, 0x00, ctx_length);
  free(ctx);
}

void cursorpos(uint8_t * data) {
  TPoint position; /* position cursor write in initialization vector */

  GetCursorPos(&position);

  data[0] ^= (uint8_t)(position.x);
  data[1] ^= (uint8_t)(position.y);
  data[2] ^= (uint8_t)(position.x + position.y + 1);
/*
  position->x = 0;
  position->y = 0;
  function meminit this is analog standart C library memset function
*/
  meminit(&position, 0x00, sizeof(TPoint));
}

/* 
  CLOMUL_CONST - security level. CLOMUL_CONST = 1
*/
static bool KDFCLOMUL(GLOBAL_MEMORY * ctx,
                      const uint8_t * password, const size_t password_len,
                            uint8_t * key,      const size_t key_len) {

  int      real, past = 0;
  uint32_t count;
  size_t   i, j, k, div;

  ctx->hash_matrix_length = (key_len + password_len + CLOMUL_CONST) *
                             SHA256_BLOCK_SIZE * 1024 * CLOMUL_CONST;

  ctx->hash_matrix = (uint8_t *)malloc(ctx->hash_matrix_length);

  if (!(ctx->hash_matrix)) {
    return false;
  }

  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, password, password_len);
  sha256_final(ctx->sha256sum);

  count  = *(uint32_t *)(ctx->sha256sum->hash);
  count &= 0x000FFFFF;
  count |= ((uint32_t)1 << 19); /* set 20 bit */
  count *= CLOMUL_CONST;

  div = (size_t)((double)count * 0.1L); /* 1/10 */

  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  sha256_init(ctx->sha256sum);

  for (i = 0; i < count; i++) {  /* ~500.000 */
    sha256_update(ctx->sha256sum, password, password_len);

    if (!(i & div)) continue; /* if i != 1/10 */

    real = (size_t)((double)i / ((double)div * 0.1L) + 0.1);

    if (real > 100) {
      real = 100;
    }

    if (real > past) {
      Form1->ProgressBar1->Position = real;

      Form1->Label9->Caption =
#ifdef PTCL_RUSSIAN_LANGUAGE
        "Генерация "
        + IntToStr(key_len * 8)  + "-битного ключа из "
        + IntToStr(password_len) + "-символьного пароля: "
        + IntToStr(real) + " %";
#else
        "Generating "
        + IntToStr(key_len * 8) + "-bit key from "
        + IntToStr(password_len) + "-character password: "
        + IntToStr(real) + " %";
#endif
      Application->ProcessMessages();
      past = real;
    }
  }

  for (i = 0; i < ctx->hash_matrix_length; i += SHA256_BLOCK_SIZE) {
    sha256_update(ctx->sha256sum, password, password_len);
    sha256_final(ctx->sha256sum);
    memcpy(ctx->hash_matrix + i, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
  }

  k = 0;
  i = 0;

  while (i < key_len) {
    sha256_update(ctx->sha256sum, ctx->hash_matrix, ctx->hash_matrix_length);
    sha256_final(ctx->sha256sum);

    j = key_len - i;

    if (j > SHA256_BLOCK_SIZE) {
      j = SHA256_BLOCK_SIZE;
    }

    if (k >= ctx->hash_matrix_length) {
      k = 0;
    }

    memcpy(ctx->hash_matrix + k, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
    memcpy(ctx->real_key  + i, ctx->sha256sum->hash, j);

    k += SHA256_BLOCK_SIZE;
    i += j;
  }

  meminit(ctx->sha256sum,   0x00, sizeof(SHA256_CTX));
  meminit(ctx->hash_matrix, 0x00, ctx->hash_matrix_length);

  free(ctx->hash_matrix);
  ctx->hash_matrix = NULL;
  ctx->hash_matrix_length = 0;

  return true;
}

/* Function size_check checked size = Bt, KiB, MiB, GiB or TiB */
static int size_check(const fsize_t size) {
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

/* get size of file bytes */
fsize_t SizeOfFile(const char * filename) {
  HANDLE fh;
  DWORD low_size;
  DWORD high_size = 0;
  fsize_t result  = -1LL;

  if (!filename) {
    return result;
  }

  fh = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                  0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if (INVALID_HANDLE_VALUE != fh) {
    low_size = GetFileSize(fh, &high_size);

    if (!GetLastError()) {
      result  = (fsize_t)high_size << 32;
      result += (fsize_t)low_size;
    }

    CloseHandle(fh);
  }
  
  return result;
}

double sizetodoubleprint(const int status, const double size) {
  return (status ? (size / (double)INT_SIZE_DATA[status - 1]) : size);
}

int erased_head_of_file(const char * filename) {
  size_t counter = 16;
  size_t result  = 0;  
  FILE * f;

  unsigned char * data = (unsigned char *)malloc(BLOCK_SIZE_FOR_ERASED);

  if (!data) {
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return -1;
  }

  meminit(data, 0x00, BLOCK_SIZE_FOR_ERASED);

  while (counter) {
    f = fopen(filename, PARAM_APPEND_BYTE); /* 16 operation openned file */

    if (!f) {
      result = (-1);
      break;
    }

    if (fwrite(data, 1, BLOCK_SIZE_FOR_ERASED, f) != BLOCK_SIZE_FOR_ERASED) {
      fclose(f);
      result = (-1);
      break;
    }

    fflush(f);

    counter--;

    if (0 == counter) { /* if old iteration */
      chsize(fileno(f), 0);
    }

    fclose(f); /* 16 operation close file */
  }

  free(data);

  return result;
}

static int erasedfile(const char * filename) {
  FILE * f;
  fsize_t size_for_erased, fsize, position = 0;

  int     real_percent, past_percent = 0;
  int     check, fsize_check;
  size_t  realread;
  
  double  fsize_double, div;

  int status_buffer_pos;
  char status_buffer[STATUS_BUFFER_SIZE] = {0};

  fsize = SizeOfFile(filename);

  if (fsize <= 0LL) {
    return -1;
  }

  f = fopen(filename, PARAM_REWRITE_BYTE);

  if (!f) {
    return -1;
  }

  uint8_t * data = (uint8_t *)malloc(DATA_SIZE);

  if (!data) {
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    fclose(f);
    return -1;
  }

  div = (double)fsize * 0.01;

  fsize_check  = size_check(fsize);
  fsize_double = sizetodoubleprint(fsize_check, (double)fsize);

  status_buffer_pos = snprintf(status_buffer, STATUS_BUFFER_SIZE,
                               STR_ERASED_FILE_PROC);

  while (position < fsize) {	
    size_for_erased = (fsize - position);

    if (size_for_erased > (fsize_t)DATA_SIZE) {
      size_for_erased = DATA_SIZE;
    }

/*****************************************************************************/
    if (TryEnterCriticalSection(&Form1->CrSec)) {
      if (false == PROCESSING) {
        LeaveCriticalSection(&Form1->CrSec);
        meminit(data, 0x00, DATA_SIZE);
        free(data);
        fclose(f);
        return OPERATION_BREAK;
      }

      LeaveCriticalSection(&Form1->CrSec);
    }
/*****************************************************************************/

    realread = fread(data, 1, size_for_erased, f);
    meminit(data, 0x00, realread);

    fseek(f, position, SEEK_SET);
    
/*****************************************************************************/
    if (TryEnterCriticalSection(&Form1->CrSec)) {
      if (false == PROCESSING) {
        LeaveCriticalSection(&Form1->CrSec);
        meminit(data, 0x00, DATA_SIZE);
        free(data);
        fclose(f);
        return OPERATION_BREAK;
      }

      LeaveCriticalSection(&Form1->CrSec);
    }
/*****************************************************************************/

    if (fwrite(data, 1, realread, f) != realread) {
      fclose(f);
      free(data);

      return -1;
    }
    
    fflush(f);

    position += (fsize_t)realread;

    real_percent = (int)((double)position / div + 0.1);

    if (real_percent > 100) {
      real_percent = 100;
    }

    if (position >= 0x80000000) {  /* Else overflow! Sorry bro... */
      real_percent = 100;
      position = fsize;
    }

    if (real_percent > past_percent) {
      check = size_check(position);

      snprintf(status_buffer + status_buffer_pos,
               STATUS_BUFFER_SIZE - status_buffer_pos,
               STR_PROGRESS_BAR_MKS,
               check ? (double)position / (double)INT_SIZE_DATA[check-1] : (double)position,
               CHAR_SIZE_DATA[check],
               fsize_double,
               CHAR_SIZE_DATA[fsize_check],
               real_percent);

      Form1->ProgressBar1->Position = real_percent;
      Form1->Label9->Caption = AnsiString(&status_buffer[0]);
      Application->ProcessMessages();

      past_percent = real_percent;
    }
  }

  free(data);

  chsize(fileno(f), 0);
  fclose(f);

  return erased_head_of_file(filename);
}

void cipher_free(void * ctx, const size_t ctx_length) {
  /* clear crypt key and struct cipher */
  meminit(ctx, 0x00, ctx_length);
  free(ctx);
}

void hmac_sha256_uf(GLOBAL_MEMORY * ctx) {
  HMAC_CTX * hmac_ctx;
  int i;
  size_t size_copy_data;
  
  hmac_ctx = (HMAC_CTX *)malloc(sizeof(HMAC_CTX));

  if (!hmac_ctx) {
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return;
  }

  size_copy_data = MINIMAL(ctx->real_key_length, SHA256_BLOCK_SIZE);

  /* copy hash sum file in local buffer "hash" */
  memcpy(hmac_ctx->hash, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);

  /* generate two secret const for hash update */
  memcpy(hmac_ctx->KEY_0, ctx->real_key, size_copy_data);
  memcpy(hmac_ctx->KEY_1, ctx->real_key, size_copy_data);

  /* if length real_key equal or more SHA256_BLOCK_SIZE then cycle NOT executable */
  for (i = ctx->real_key_length; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] = 0x00;
    hmac_ctx->KEY_1[i] = 0x00;
  }

  /* simbol 'U', decimal  85, bits 01010101 */
  for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] ^= 0x55;
  }

  /* simbol 'f', decimal 102, bits 10101010 */
  for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_1[i] ^= 0x66;
  }

  /* clear sha256sum struct */
  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));

  /* calculate hash for (key xor 0x55) and hash file */
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, hmac_ctx->KEY_0, SHA256_BLOCK_SIZE);
  sha256_update(ctx->sha256sum, hmac_ctx->hash, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  memcpy(hmac_ctx->hash, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);

  /* clear sha256sum struct */
  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));

  /* calculate hash for (key xor 0x66) and hash for ((key xor 0x55) and hash file) */
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, hmac_ctx->KEY_1, SHA256_BLOCK_SIZE);
  sha256_update(ctx->sha256sum, hmac_ctx->hash, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  /* clear  buffers for security */
  meminit(hmac_ctx, 0x00, sizeof(HMAC_CTX));
  free(hmac_ctx);
  /* now control sum crypt key and file in buffer ctx->sha256sum->hash */
}

void control_sum_buffer(GLOBAL_MEMORY * ctx, const size_t count) {
  size_t       i = 0;
  size_t remnant = count;

  while (i < count) {
    if (remnant < LENGTH_DATA_FOR_CHECK) {
      sha256_update(ctx->sha256sum,
                    (ctx->operation ? ctx->output : ctx->input) + i,
                    remnant);
    }
    else { /* if remnant >= LENGTH_DATA_FOR_CHECK */
      sha256_update(ctx->sha256sum,
                    (ctx->operation ? ctx->output : ctx->input) + i,
                    LENGTH_DATA_FOR_CHECK);
    }

          i += LENGTH_DATA_FOR_CHECK;
    remnant -= LENGTH_DATA_FOR_CHECK;
  }
}

/* generating new crypt key for re-keying */
static void internal_re_keying(GLOBAL_MEMORY * ctx) {
  SHA256_CTX sha;
  size_t i, size;

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

  switch(ctx->cipher_number) {
    case AES:
      rijndael_key_encrypt_init(rijndael_ctx, ctx->real_key, ctx->real_key_length * 8);
      break;
    case BLOWFISH:
      blowfish_init(blowfish_ctx, ctx->real_key, ctx->real_key_length);
      break;
    case SERPENT:
      serpent_init(serpent_ctx, ctx->real_key_length * 8, ctx->real_key);
      break;
    case THREEFISH:
      threefish_init(threefish_ctx, (threefishkeysize_t)(ctx->real_key_length * 8),
                     (uint64_t*)ctx->real_key, (uint64_t*)ctx->real_key);
      break;
    case TWOFISH:
      twofish_init(twofish_ctx, ctx->real_key, ctx->real_key_length);
      break;
  }
}

/* fsize += (size initialized vector + size sha256 hash sum) */
/* break operation if (fsize > 4 EiB) or (fsize == 0) or (fsize == -1) */
static int size_correct(const GLOBAL_MEMORY * ctx, fsize_t fsize) {
  if (-1LL == fsize) {
    return READ_FILE_NOT_OPEN;
  }

  if (0LL == fsize) {
    return SIZE_FILE_EMPTY;
  }

  if (ENCRYPT == ctx->operation) {
/* if post encrypt size of file >= 4 EiB then this operation BAD ->> don't for decrypting */
    if ((fsize_t)(fsize + SHA256_BLOCK_SIZE + ctx->vector_length) & ((fsize_t)1 << 62)) {
      return SIZE_FILE_VERY_BIG;
    }
  }
  else {
/* if fsize < minimal size, this file don't for decrypt */
    if (fsize < (fsize_t)(SHA256_BLOCK_SIZE + ctx->vector_length + 1)) {
      return SIZE_DECRYPT_FILE_INCORRECT;
    }
  }

  return OK;
}

int filecrypt(GLOBAL_MEMORY * ctx) {
  fsize_t fsize, position = 0LL;
	
  double div, fsize_double;

  int check, fsize_check, status_buffer_pos;

  size_t nblock;
  size_t realread;
  size_t re_keying = 0;

  int real_percent, past_percent = 0;

  FILE * fi, *fo;
  
  char status_buffer[STATUS_BUFFER_SIZE] = {0};

  fsize        = SizeOfFile(Form1->Edit1->Text.c_str());
  fsize_check  = size_correct(ctx, fsize);

  if (fsize_check) {      /* IF NOT OK */
    if (fclose(fi) == -1)
      return STREAM_INPUT_CLOSE_ERROR;
    else
      return fsize_check;
  }

  fi = fopen(Form1->Edit1->Text.c_str(), PARAM_READ_BYTE);

  if (!fi) {
    return READ_FILE_NOT_OPEN;
  }

  fo = fopen(Form1->Edit2->Text.c_str(), PARAM_WRITE_BYTE);

  if (!fo) {
    if (fclose(fi) == -1)
      return STREAM_INPUT_CLOSE_ERROR;
    else
      return WRITE_FILE_NOT_OPEN;
  }

  div = (double)fsize * 0.01L;
  
  fsize_check  = size_check(fsize);
  fsize_double = sizetodoubleprint(fsize_check, (double)fsize);

  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  sha256_init(ctx->sha256sum);

/*****************************************************************************/

  status_buffer_pos = snprintf(status_buffer, STATUS_BUFFER_SIZE,
    STR_ENCRYPT_FILE_PROC,
    OPERATION_NAME[ctx->operation ? 1 : 0],
    ALGORITM_NAME[ctx->cipher_number]);

  while (position < fsize) {
    if (0LL == position) {
      if (ENCRYPT == ctx->operation) {
        switch (ctx->cipher_number) {
          case AES:
            rijndael_encrypt(rijndael_ctx, ctx->vector, ctx->output);
            break;
          case BLOWFISH:
            memcpy(ctx->output, ctx->vector, ctx->vector_length);
            blowfish_encrypt(blowfish_ctx, (uint32_t *)ctx->output, (uint32_t *)(ctx->output + 4));
            break;
          case SERPENT:
            serpent_encrypt(serpent_ctx, (uint32_t *)ctx->vector, (uint32_t *)ctx->output);
            break;
          case THREEFISH:
            threefish_encrypt(threefish_ctx, (uint64_t*)ctx->vector, (uint64_t*)ctx->output);
            break;
          case TWOFISH:
            twofish_encrypt(twofish_ctx, ctx->vector, ctx->output);
            break;
        }

        memcpy(ctx->vector, ctx->output, ctx->vector_length);

        if (fwrite(ctx->vector, 1, ctx->vector_length, fo) != ctx->vector_length) {
          return close_in_out_files(fi, fo, WRITE_FILE_ERROR);
        }

        fflush(fo);
      }
      else
      if (DECRYPT == ctx->operation) {
        if (fread(ctx->vector, 1, ctx->vector_length, fi) != ctx->vector_length) {
          return close_in_out_files(fi, fo, READ_FILE_ERROR);
        }
        position += (fsize_t)ctx->vector_length;
      }
    }

    CHECK_BREAK_STREAM;
    
    realread = fread(ctx->input, 1, DATA_SIZE, fi);

    for (nblock = 0; nblock < realread; nblock += ctx->vector_length) {
      switch (ctx->cipher_number) {
        case AES:
          rijndael_encrypt(rijndael_ctx, ctx->vector, ctx->output + nblock);
          break;
        case BLOWFISH:
          blowfish_encrypt(blowfish_ctx, (uint32_t *)ctx->vector, (uint32_t *)(ctx->vector + 4));
          memcpy(ctx->output + nblock, ctx->vector, ctx->vector_length);
          break;
        case SERPENT:
          serpent_encrypt(serpent_ctx, (uint32_t *)ctx->vector, (uint32_t *)(ctx->output + nblock));
          break;
        case THREEFISH:
          threefish_encrypt(threefish_ctx, (uint64_t*)ctx->vector, (uint64_t*)(ctx->output + nblock));
          break;
        case TWOFISH:
          twofish_encrypt(twofish_ctx, ctx->vector, ctx->output + nblock);
          break;
        }

      memxormove(ctx->output + nblock, ctx->input + nblock, ctx->vector_length);
      memcpy(ctx->vector, (ctx->operation ? ctx->input : ctx->output) + nblock, ctx->vector_length);
    }

       position  += (fsize_t)realread;
    real_percent  = (int)((double)position / div + 0.1);

    if (real_percent > 100) {
      real_percent = 100;
    }

    if ((position == fsize) && (DECRYPT == ctx->operation)) {
      realread -= SHA256_BLOCK_SIZE;
    }

    /* control sum all read 1 MB data for [en/de]crypt + crypt key */
    control_sum_buffer(ctx, realread);

    CHECK_BREAK_STREAM;

    if (fwrite(ctx->output, 1, realread, fo) != realread) {
      return close_in_out_files(fi, fo, WRITE_FILE_ERROR);
    }

    fflush(fo);

    if (real_percent > past_percent) {
      check = size_check(position);

      snprintf(status_buffer + status_buffer_pos,
               STATUS_BUFFER_SIZE - status_buffer_pos,
               STR_PROGRESS_BAR_MKS,
               check ? (double)position / (double)INT_SIZE_DATA[check-1] : (double)position,
               CHAR_SIZE_DATA[check],
               fsize_double,
               CHAR_SIZE_DATA[fsize_check],
               real_percent);

      Form1->ProgressBar1->Position = real_percent;              
      Form1->Label9->Caption = AnsiString(&status_buffer[0]);
      Application->ProcessMessages();
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

  sha256_final(ctx->sha256sum);

  hmac_sha256_uf(ctx);

  if (ENCRYPT == ctx->operation) {
    if (fwrite(ctx->sha256sum->hash, 1, SHA256_BLOCK_SIZE, fo) != SHA256_BLOCK_SIZE) {
      return close_in_out_files(fi, fo, WRITE_FILE_ERROR);
    }

    fflush(fo);
  }
  else {
    if (memcmp(ctx->input + realread, ctx->sha256sum->hash, SHA256_BLOCK_SIZE) != 0) {
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG, STR_CONTROL_SUM_FILE_ERROR);
    }
  }

  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));

  return close_in_out_files(fi, fo, OK);
}

bool random_vector_init(uint8_t * data, size_t size) {
  size_t i;
  size_t vector_size = size;

  ARC4_CTX * arc4_memory   = (ARC4_CTX *)malloc(sizeof(ARC4_CTX));
  uint8_t *  vector_memory = (uint8_t *)malloc(vector_size);

  if (!arc4_memory || !vector_memory) {
    free(vector_memory);
    free(arc4_memory);		
    return false;
  }

  CryptGenRandom(hcrypt, vector_size, vector_memory);

  /* generate trash for security and xor with random data from memory */
  for (i = 0; i < vector_size; i++) {
    vector_memory[i] ^= (uint8_t)genrand(0x00, 0xFF);
  }
  
  /* encrypt data initialized vector for security */
  arc4_init(arc4_memory, data, size);
  arc4(arc4_memory, vector_memory, data, size);
  
  /* clear all data for security */
  meminit(vector_memory, 0x00, vector_size);
  meminit(arc4_memory, 0x00, sizeof(ARC4_CTX));
  
  free(vector_memory);
  free(arc4_memory);

  return true;
}

bool vector_init(uint8_t * data, size_t size) {
  size_t i;
  size_t stack_trash[2]; /* NOT initialized == ALL OK */

  CryptGenRandom(hcrypt, size, data);

  for (i = 0; i < size; i++) {
    data[i] ^= (uint8_t)genrand(0x00, 0xFF);
  }

  /* random data from stack xor initialized vector */
  (*(uint32_t *)data) ^= (uint32_t)stack_trash[0] +
                         (uint32_t)genrand(0x00000000, 0xFFFFFFFF) ^
                         (uint32_t)stack_trash[1];

  cursorpos(data); // X and Y cursor position xor operation for data[0] and data[1];

  stack_trash[0] = stack_trash[1] = 0;

  /* generate real vector with cryptography */
  return random_vector_init(data, size);
}

char * CharA_Or_CharOV(size_t length) {
  return (24 == length || 128 == length) ? " бита" : " бит";
}

int GeneratingCryptKey(const char * message) {
  if (MessageForUser(MB_ICONINFORMATION + MB_YESNO, STR_PROGRAMM_NAME, message) == IDNO) {
    return IDNO;
  }

  return IDYES;
}

void * CipherInitMemory(GLOBAL_MEMORY * ctx, size_t cipher_length) {
  void * cipher_ptr = (void *)malloc(cipher_length);

  if (NULL == cipher_ptr) {
    return NULL;
  }

  meminit(cipher_ptr, 0x00, cipher_length);

  switch(ctx->cipher_number) {
    case AES:
      rijndael_ctx = (uint32_t *)cipher_ptr;
      rijndael_key_encrypt_init(rijndael_ctx, ctx->real_key, ctx->real_key_length * 8);
      break;
    case BLOWFISH:
      blowfish_ctx = (BLOWFISH_CTX *)cipher_ptr;
      blowfish_init(blowfish_ctx, ctx->real_key, ctx->real_key_length);
      break;
    case SERPENT:
      serpent_ctx = (SERPENT_CTX *)cipher_ptr;
      serpent_init(serpent_ctx, ctx->real_key_length * 8, ctx->real_key);
      break;
    case THREEFISH:
      threefish_ctx = (THREEFISH_CTX *)cipher_ptr;
      threefish_init(threefish_ctx, (threefishkeysize_t)(ctx->real_key_length * 8),
                     (uint64_t *)ctx->real_key, (uint64_t *)ctx->real_key);
      break;
    case TWOFISH:
      twofish_ctx = (TWOFISH_CTX *)cipher_ptr;
      twofish_init(twofish_ctx, ctx->real_key, ctx->real_key_length);
      break;
  }

  return cipher_ptr;
}

bool GLOBAL_MEMORY_ALLOCATOR(GLOBAL_MEMORY ** memory) {
  (*memory) = (GLOBAL_MEMORY *)malloc(sizeof(GLOBAL_MEMORY));

  if (NULL == (*memory)) {
    return false;
  }

  meminit((*memory), 0x00, sizeof(GLOBAL_MEMORY));

  (*memory)->sha256sum = (SHA256_CTX *)malloc(sizeof(SHA256_CTX));

  if (NULL == (*memory)->sha256sum) {
    free(*memory);
    return false;
  }

  meminit((*memory)->sha256sum, 0x00, sizeof(SHA256_CTX));

  return true;
}

void FormActivate(const bool active) {
  Form1->Label5->Enabled = active;
  Application->ProcessMessages();
}

bool CIPHER_SET(GLOBAL_MEMORY * ctx, const char * key_size, int * aes) {
  int result;

  if (ctx->cipher_number == THREEFISH) {
    if (strcmp(key_size, CHAR_KEY_LENGTH_THREEFISH[0]) == 0) {
      ctx->real_key_length =  256;
    }
    else
    if (strcmp(key_size, CHAR_KEY_LENGTH_THREEFISH[1]) == 0) {
      ctx->real_key_length =  512;
    }
    else
    if (strcmp(key_size, CHAR_KEY_LENGTH_THREEFISH[2]) == 0) {
      ctx->real_key_length = 1024;
    }
  }
  else
  if (ctx->cipher_number == BLOWFISH) {
    ctx->real_key_length = 448;
  }
  else { /*  AES || SERPENT || TWOFISH  */
    result = str_list_search(key_size, CHAR_KEY_LENGTH_AES, CHAR_KEY_LENGTH_AES_COUNT);
		  
    if (result == -1) {
      return false;
    }
	
    if (ctx->cipher_number == AES) {
      switch (result) {
        case 0: *aes = 10;
                 break;
        case 1: *aes = 12;
                 break;
        case 2: *aes = 14;
                 break;
      }
    }
	
    switch (result) {
      case 0: ctx->real_key_length = 128;
              break;
      case 1: ctx->real_key_length = 192;
              break;
      case 2: ctx->real_key_length = 256;
              break;
    }
  }
  
  return true;
}

__fastcall TForm1::TForm1(TComponent* Owner): TForm(Owner) {
}

void ShowOperationStatus(const int status) {
  switch (status) {
    case OK:
      MessageForUser(MB_ICONINFORMATION + MB_OK, STR_PROGRAMM_NAME,
                     STR_OK);
      break;
    case READ_FILE_NOT_OPEN:
      MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG,
                     STR_READ_FILE_NOT_OPEN);
      break;
    case WRITE_FILE_NOT_OPEN:
      MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG,
                     STR_WRITE_FILE_NOT_OPEN);
      break;
    case READ_FILE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG,
                     STR_READ_FILE_ERROR);
      break;
    case WRITE_FILE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG,
                     STR_WRITE_FILE_ERROR);
      break;
    case STREAM_INPUT_CLOSE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG,
                     STR_STREAM_IN_CLOSE_ERROR);
      break;
    case STREAM_OUTPUT_CLOSE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG,
                     STR_STREAM_OUT_CLOSE_ERROR);
      break;
    case SIZE_FILE_EMPTY:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                    STR_SIZE_FILE_EMPTY);
      break;
    case SIZE_FILE_VERY_BIG:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_SIZE_FILE_VERY_BIG);
      break;
    case SIZE_DECRYPT_FILE_INCORRECT:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_SIZE_DECRYPT_FILE_INCORRECT);
      break;
    case OPERATION_BREAK:
      MessageForUser(MB_ICONINFORMATION + MB_OK, STR_PROGRAMM_NAME,
                     STR_OPERATION_BREAK);
      break;
  }
}

void __fastcall TForm1::Button4Click(TObject *Sender) {
  extern int AES_Rounds; /* in rijndael.c source code file */

  String UnicodeMsg = STR_STOP_PROCESS;

  /* if stream create and execute then blocking stream and set PROCESSING in false
     stream to check PROCESSING and if PROCESSING == false, stream killself */
  EnterCriticalSection(&Form1->CrSec);
  if (PROCESSING) {
    if (MessageForUser(MB_ICONQUESTION + MB_YESNO, STR_PROGRAMM_NAME, UnicodeMsg.c_str()) == IDYES) {
      PROCESSING = false;

      FormActivate(true);
      Button4->Caption = STR_START;
      Form1->ProgressBar1->Position = 0;
      Form1->ProgressBar1->Update();
    }

    LeaveCriticalSection(&Form1->CrSec);
    return;
  }
  LeaveCriticalSection(&Form1->CrSec);
/*****************************************************************************/

  GLOBAL_MEMORY * memory;
  void * cipher_pointer;

  int real_read, result;
  size_t cipher_length;

/*****************************************************************************/
  if (x_strnlen(Edit1->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_INPUT_FILENAME_EMPTY);
    return;
  }

  if (x_strnlen(Edit2->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_OUTPUT_FILENAME_EMPTY);
    return;
  }

  if (x_strnlen(Memo1->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_PASS_OR_KEYFILE_EMPTY);
    return;
  }
/*****************************************************************************/
  if (Edit1->Text == Edit2->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_IN_OUT_FILENAME_EQUAL);
    return;
  }

  if (Edit1->Text == Memo1->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_OUT_KEY_FILENAME_EQUAL);
    return;
  }

  if (Edit2->Text == Memo1->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_IN_KEY_FILENAME_EQUAL);
    return;
  }

/*****************************************************************************/
  if (FileExists(Edit1->Text) == False) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_IN_FILENAME_NFOUND);
    return;
  }

  UnicodeMsg = "";

  if (FileExists(Edit2->Text) == True) {
    UnicodeMsg = STR_OUT_FILE_FOUND;

    if (MessageForUser(MB_ICONWARNING + MB_YESNO, STR_WARNING_MSG, UnicodeMsg.c_str()) == IDNO) {
      MessageForUser(MB_ICONINFORMATION + MB_OK, STR_PROGRAMM_NAME,
                     STR_CHANGE_OUT_FILENAME);
      UnicodeMsg = "";
      return;
    }

    UnicodeMsg = "";
  }

/*****************************************************************************/
  if (GLOBAL_MEMORY_ALLOCATOR(&memory) == false) {
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return;
  }
/*****************************************************************************/
  result = str_list_search(ComboBox1->Text.c_str(), ALGORITM_NAME, ALGORITM_NAME_COUNT);
  
  if (-1 == result) {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_CIPHER_NOT_ENTER);
    return;
  }

  memory->cipher_number = (cipher_t)result; /* search and set cipher number */

/*****************************************************************************/
  memory->real_key_length = 0;

  if (!CIPHER_SET(memory, ComboBox2->Text.c_str(), &AES_Rounds)) {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_KEY_SIZE_NOT_ENTER);
    return;
  }

/*****************************************************************************/
  memory->real_key_length = (memory->real_key_length / 8);

  /*
    AES       = (real_key_length = 16 or 24 or 32;
    TWOFISH   = (real_key_length = 16 or 24 or 32);
    SERPENT   = (real_key_length = 16 or 24 or 32);
    BLOWFISH  = (real_key_length = 56);
    THREEFISH = (real_key_length = 32 or 64 or 128);
  */

  memory->operation = ENCRYPT; /* fucking warnings c++ builder !*/

  if (RadioButton1->Checked) {
    memory->operation = ENCRYPT;
  }
  else
  if (RadioButton2->Checked) {
    memory->operation = DECRYPT;
  }
  else {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_OPERATION_NOT_ENTER);
    return;
  }

  memory->real_key = (uint8_t*)malloc(memory->real_key_length);

  if (!memory->real_key) {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return;
  }

  memory->new_key_length = memory->real_key_length;
  memory->new_key = (uint8_t*)malloc(memory->new_key_length);

  if (!memory->new_key) {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return;
  }
/*****************************************************************************/
  meminit(memory->new_key, 0x00, memory->new_key_length);
  
  real_read = readfromfile(Memo1->Text.c_str(), memory->real_key, memory->real_key_length);

  if (real_read == (int)(memory->real_key_length)) {
      UnicodeMsg =
#ifdef PTCL_RUSSIAN_LANGUAGE
      "Использовать " + IntToStr(memory->real_key_length * 8) + "-битный ключ шифрования из файла?\n";
#else
      "Use " + IntToStr(memory->real_key_length * 8) + "-bit encryption key from file?\n";
#endif
	
    if (GeneratingCryptKey(UnicodeMsg.c_str()) == IDNO) {
      free_global_memory(memory, sizeof(GLOBAL_MEMORY));
      UnicodeMsg = "";
      return;
    }
    /*
    if (DoubleSecurityWitchPassword() == true) {
      GeneratingKeyWitchPassword(memory->real_key, memory->real_key_length, password);
    }
    */
  }
  else
  if ((real_read > 0) && (real_read < (int)(memory->real_key_length))) {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));

    UnicodeMsg = 
#ifdef PTCL_RUSSIAN_LANGUAGE
      "Недостаточно данных в ключевом файле!\n\n"
      "Прочитано:\t" + IntToStr(real_read) + " байт\n"
      "Требуется:\t" + IntToStr(memory->real_key_length) + " байт";
#else
      "Not enough data in the key file!\n\n"
      "Read:\t" + IntToStr(real_read) + " bt\n"
      "Required:\t" + IntToStr(memory->real_key_length) + " bt";
#endif

    Application->MessageBox(UnicodeMsg.c_str(), STR_WARNING_MSG, MB_ICONWARNING + MB_OK);
    UnicodeMsg = "";
    return;
  }
  else
  if ((real_read == 0) || (real_read == -1)) {
    real_read = (int)x_strnlen(Memo1->Text.c_str(), 256);

    if ((real_read > 7) && (real_read < 257)) {
      UnicodeMsg = 
#ifdef PTCL_RUSSIAN_LANGUAGE
        "Сгенерировать " + IntToStr(memory->real_key_length * 8) +
        "-битный ключ шифрования из пароля?\n";
#else
        "Generate " + IntToStr(memory->real_key_length * 8) +
        "-bit encryption key from password?\n";
#endif
	
      if (GeneratingCryptKey(UnicodeMsg.c_str()) == IDNO) {
        free_global_memory(memory, sizeof(GLOBAL_MEMORY));
        UnicodeMsg = "";
        return;
      }

      Button4->Enabled = False;

      Form1->ProgressBar1->Position = 0;
      Form1->ProgressBar1->Update();

      /* Crypt key generator; generate crypt key from password */
      if (KDFCLOMUL(memory, (uint8_t *)Memo1->Text.c_str(), real_read,
                    memory->real_key, memory->real_key_length) == false) {
                    
        free_global_memory(memory, sizeof(GLOBAL_MEMORY));
        MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
        Form1->Close();
      }

      Button4->Enabled = True;
    }
    else {
      free_global_memory(memory, sizeof(GLOBAL_MEMORY));

      UnicodeMsg = 
#ifdef PTCL_RUSSIAN_LANGUAGE
        "Длина символьного ключа некорректна!\n\n"
        "Прочитано:\t" + IntToStr(real_read) + " байт\n"
        "Требуется:\tот 8 до 256 байт";
#else
        "The string key length is incorrect!\n\n"
        "Read:\t" + IntToStr(real_read) + " bt\n"
        "Required:\tfrom 8 to 256 bt";
#endif

      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG, UnicodeMsg.c_str());
      UnicodeMsg = "";
      return;
    }
  }
/*****************************************************************************/
  UnicodeMsg = "";
  
  switch(memory->cipher_number) {
    case AES:
      memory->vector_length = 16;
      cipher_length = 4 * (AES_Rounds + 1) * 4;
      break;
			
    case BLOWFISH:
      memory->vector_length =  8;
      cipher_length = sizeof(BLOWFISH_CTX);
      break;
			
    case SERPENT:
      memory->vector_length = 16;
      cipher_length = sizeof(SERPENT_CTX);
      break;
			
    case THREEFISH:
      memory->vector_length = memory->real_key_length;
      cipher_length = sizeof(THREEFISH_CTX);
      break;
			
    case TWOFISH:
      memory->vector_length = 16;
      cipher_length = sizeof(TWOFISH_CTX);
      break;
  }
/*****************************************************************************/
  memory->vector = (uint8_t*)malloc(memory->vector_length);

  if (!memory->vector) {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return;
  }

  meminit(memory->vector, 0x00, memory->vector_length);

  if (ENCRYPT == memory->operation) {
    if (!vector_init(memory->vector, memory->vector_length)) {
      free_global_memory(memory, sizeof(GLOBAL_MEMORY));
      MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
      Form1->Close();
    }
  }

  /* allocate memory + lock memory + protect memory */
  cipher_pointer = CipherInitMemory(memory, cipher_length);

  if (NULL == cipher_pointer) {
    free_global_memory(memory, sizeof(GLOBAL_MEMORY));
    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return;
  }

  UnicodeMsg =
#ifdef PTCL_RUSSIAN_LANGUAGE
    "Приступить к выполнению выбранной операции?\n\n"
    "Операция:\t" + String(OPERATION_NAME[memory->operation ? 1 : 0]) + "\n"
    "Алгоритм:\t" + String(ALGORITM_NAME[memory->cipher_number]) + CIPHER_MODE + "\n"
    "Длина ключа:\t" + IntToStr(memory->real_key_length * 8).c_str() +
                      CharA_Or_CharOV(memory->real_key_length);
#else
    "Proceed with the operation you selected?\n\n"
    "Operation:\t" + String(OPERATION_NAME[memory->operation ? 1 : 0]) + "\n"
    "Algorithm:\t" + String(ALGORITM_NAME[memory->cipher_number]) + CIPHER_MODE + "\n"
    "Key length:\t" + IntToStr(memory->real_key_length * 8).c_str() + " bit";
#endif

  result = OPERATION_BREAK;

  if (MessageForUser(MB_ICONQUESTION + MB_YESNO, STR_PROGRAMM_NAME, UnicodeMsg.c_str()) == IDYES) {
    Button4->Caption = STR_STOP;
    Form1->ProgressBar1->Position = 0;
    Form1->ProgressBar1->Update();
    FormActivate(false);

/*****************************************************************************/
    SET_START_STREAM;
    result = filecrypt(memory);
    SET_STOP_STREAM;
/*****************************************************************************/

    Button4->Caption = STR_START;
    FormActivate(true);
  }

  ShowOperationStatus(result);
/*****************************************************************************/
  if (result == OK && CheckBox1->Checked == True) {
    UnicodeMsg = STR_ERASED_FILE_QUES;

    if (MessageForUser(MB_ICONWARNING + MB_YESNO, STR_WARNING_MSG,
                       UnicodeMsg.c_str()) == IDYES) {

      Button4->Caption = STR_STOP;               

      Form1->ProgressBar1->Position = 0;
      Form1->ProgressBar1->Update();

      UnicodeMsg = "";

/*****************************************************************************/
      SET_START_STREAM;
        result = erasedfile(Edit1->Text.c_str());
      SET_STOP_STREAM;
/*****************************************************************************/

      if (result == OK) {
        if (DeleteFile(Edit1->Text) != False) {
          MessageForUser(MB_ICONINFORMATION + MB_OK, STR_PROGRAMM_NAME,
                        STR_ERASED_FILE_OK);
        }
        else {
          /* if file rewriting and not delete -> show error code for user */
          unsigned long error_delete = GetLastError();

          UnicodeMsg = 
#ifdef PTCL_RUSSIAN_LANGUAGE
            "Ошибка удаления файла!\n\n"
            "Файл: " + Form1->Edit1->Text + "\n\n"
            "был перезаписан, но не был удалён с диска!\n\n"
            "Код ошибки: " + IntToStr(error_delete);
#else
            "Error delete file!\n\n"
            "Filename: " + Form1->Edit1->Text + "\n\n"
            "was overwritten but not deleted from the disk!\n\n"
            "Error code: " + IntToStr(error_delete);
#endif
          MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, UnicodeMsg.c_str());
        }
      }
      else
      if (result == OPERATION_BREAK) {
        MessageForUser(MB_ICONINFORMATION + MB_OK, STR_PROGRAMM_NAME,
                       STR_OPERATION_BREAK);
      }
      else {
        MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG,
                       STR_ERROR_ERASED_FILE);
      }
    }
  }


  UnicodeMsg = "";

  cipher_free(cipher_pointer, cipher_length);
  free_global_memory(memory, sizeof(GLOBAL_MEMORY));

  Button4->Caption = STR_START; 

  Form1->ProgressBar1->Position = 0;
  Form1->ProgressBar1->Update();

  Application->ProcessMessages();
}

void PathToStrings(const AnsiString &FileName, AnsiString &path, AnsiString &name, AnsiString &ext) {
  path = ExtractFilePath(FileName);
  name = ExtractFileName(FileName);
  ext  = ExtractFileExt(FileName);
}

/* INPUT FILE */
void __fastcall TForm1::Button1Click(TObject *Sender) {
  OpenDialog1->Title = STR_INPUT_FILENAME;

  AnsiString path;
  AnsiString name;
  AnsiString ext;

  if (OpenDialog1->Execute()) {
    PathToStrings(OpenDialog1->FileName, path, name, ext);

    Form1->Edit1->Clear();
    Form1->Edit1->Text = OpenDialog1->FileName;

    Form1->Edit2->Clear();

    if (ext.LowerCase() == EXT_CRYCON) {
      if (name.Length() < 8) {
        Form1->Edit2->Text = path + EMPTY_FILENAME;
      }
      else {
        Form1->Edit2->Text = path + name.SetLength(name.Length() - 7);
      }
    }
    else {
      Form1->Edit2->Text = OpenDialog1->FileName + EXT_CRYCON;
    }
  }

  OpenDialog1->FileName = "";
}

/* OUTPUT FILE */
void __fastcall TForm1::Button2Click(TObject *Sender) {
  SaveDialog1->Title = STR_OUTPUT_FILENAME;

  AnsiString path;
  AnsiString name;
  AnsiString ext;

  if (SaveDialog1->Execute()) {
    PathToStrings(SaveDialog1->FileName, path, name, ext);

    Form1->Edit2->Clear();

    if (Form1->Edit1->Text == SaveDialog1->FileName) {
      if (ext.LowerCase() == EXT_CRYCON) {
        if (name.Length() < 8) {
          Form1->Edit2->Text = path + EMPTY_FILENAME;
        }
        else {
          Form1->Edit2->Text = path + name.SetLength(name.Length() - 7);
        }
      }
      else {
        Form1->Edit2->Text = SaveDialog1->FileName + EXT_CRYCON;
      }
    }
    else {
      Form1->Edit2->Text = SaveDialog1->FileName;
    }
  }
  
  SaveDialog1->FileName = "";
}

void __fastcall TForm1::Button3Click(TObject *Sender) {
  OpenDialog1->Title = STR_KEY_FILENAME;

  if (OpenDialog1->Execute()) {
    Form1->Memo1->Clear();
    Form1->Memo1->Lines->Text = OpenDialog1->FileName;
  }
}

void AddListKeys(const cipher_t cipher) {
  int i;

  Form1->ComboBox2->Items->Clear();

  if (cipher == THREEFISH) {
	for (i = 0; i < 3; i++) {
	  Form1->ComboBox2->Items->Add(CHAR_KEY_LENGTH_THREEFISH[i]);
	}
	Form1->ComboBox2->Text = AnsiString(CHAR_KEY_LENGTH_THREEFISH[0]);
  }
  else {
	for (i = 0; i < 3; i++) {
	  Form1->ComboBox2->Items->Add(CHAR_KEY_LENGTH_AES[i]);
	}
	Form1->ComboBox2->Text = AnsiString(CHAR_KEY_LENGTH_AES[0]);
  }
}

void __fastcall TForm1::FormCreate(TObject *Sender) {
  int i;
  SETTINGS settings;

  if (!CryptAcquireContext(&hcrypt, NULL, NULL, PROV_RSA_FULL, 0)) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
#ifdef PTCL_RUSSIAN_LANGUAGE
    "Криптопровайдер Microsoft Windows недоступен!");
#else
    "Microsoft Windows cryptographic provider is unavailable!");
#endif
  }

  InitializeCriticalSection(&CrSec);
  SendMessage(ProgressBar1->Handle, PBM_SETBARCOLOR, 0, clGreen);

  settings.top_color        = FORM_HEAD_COLOR;
  settings.top_text_color   = FORM_HEAD_TEXT_COLOR;
  settings.top_text_b_color = FORM_HEAD_TEXT_B_COLOR;
  settings.cipher           = AES;
  settings.key_size         = 128;
  settings.pass_gen_size    =  64;
  settings.operation        = ENCRYPT;
  settings.erased           = false;

  init_settings(SETTINGS_FILENAME, &settings); /* in settings.h */

  for (int i = 0; i < 5; i++) {
    ComboBox1->Items->Add(ALGORITM_NAME[i]);
  }

  Form1->Caption         = STR_PROGRAMM_NAME;
  Form1->Label6->Caption = STR_PROGRAMM_NAME;

  Form1->Label1->Caption = STR_CRYPT_ALG;
  Form1->Label2->Caption = STR_INPUT_FILENAME;
  Form1->Label3->Caption = STR_OUTPUT_FILENAME;
  Form1->Label4->Caption = STR_KEY_LENGTH;
  Form1->Label8->Caption = STR_KEY_OR_KEY_FILENAME;
  Form1->Label9->Caption = STR_STATUS;

  Form1->Button1->Caption = STR_OPEN;
  Form1->Button2->Caption = STR_OPEN;
  Form1->Button3->Caption = STR_OPEN;
  Form1->Button4->Caption = STR_START;
  Form1->Button5->Caption = STR_KEYGEN;

  Form1->RadioButton1->Caption = STR_EN;
  Form1->RadioButton2->Caption = STR_DE;

  Form1->CheckBox1->Caption    = STR_ERASED;

/* this is code work with data from configurate file */

  Form1->Shape1->Pen->Color    = settings.top_color;
  Form1->Shape2->Brush->Color  = settings.top_color;
  Form1->Shape2->Pen->Color    = settings.top_color;

  Form1->Label5->Color         = settings.top_text_b_color;
  Form1->Label6->Color         = settings.top_text_b_color;
  Form1->Label7->Color         = settings.top_text_b_color;

  Form1->Label5->Font->Color   = settings.top_text_color;
  Form1->Label6->Font->Color   = settings.top_text_color;
  Form1->Label7->Font->Color   = settings.top_text_color;

  Form1->Edit3->Text           = IntToStr(settings.pass_gen_size);
  Form1->ComboBox1->Text       = AnsiString(ALGORITM_NAME[settings.cipher]);

  Form1->CheckBox1->Checked    = settings.erased;

  if (settings.cipher == BLOWFISH) {
    Label4->Visible = False;
    Form1->ComboBox2->Visible = False;
  }
  else { /* other ciphers */
    Label4->Visible = True;
    Form1->ComboBox2->Visible = True;

    AddListKeys(settings.cipher);

    Form1->ComboBox2->Text = IntToStr(settings.key_size);
  }

  switch(settings.operation) {
    case ENCRYPT: Form1->RadioButton1->Checked = true;
                  break;
    case DECRYPT: Form1->RadioButton2->Checked = true;
                  break;

  }

  Form1->ComboBox1->Style = csDropDown;
  Form1->ComboBox2->Style = csDropDown;

/* this is code work with data from configurate file */

  Form1->ProgressBar1->Min = 0;
  Form1->ProgressBar1->Max = 100;
}

void __fastcall TForm1::ComboBox1Change(TObject *Sender) {
  int i, result;

  result = str_list_search(ComboBox1->Text.c_str(), ALGORITM_NAME, 5);

  if (result == -1) {
    return;
  }

  ComboBox2->Items->Clear();

  if (result != BLOWFISH) {
    if (result != THREEFISH) {
      for (i = 0; i < 3; i++) {
        ComboBox2->Items->Add(CHAR_KEY_LENGTH_AES[i]);
      }
      ComboBox2->Text = AnsiString(CHAR_KEY_LENGTH_AES[0]);
    }
    else {
      for (i = 0; i < 3; i++) {
        ComboBox2->Items->Add(CHAR_KEY_LENGTH_THREEFISH[i]);
      }
      ComboBox2->Text = AnsiString(CHAR_KEY_LENGTH_THREEFISH[0]);
    }

    Label4->Visible = True;
    ComboBox2->Visible = True;
  }
  else {
    Label4->Visible = False;
    ComboBox2->Visible = False;
  }
}

void CryptProvDelete(HCRYPTPROV * hcp) {
  if (*hcp) {
    CryptReleaseContext(*hcp, 0);
    *hcp = NULL;
  }
}

void __fastcall TForm1::Label5Click(TObject *Sender) {
  DeleteCriticalSection(&CrSec);
  CryptProvDelete(&hcrypt);
  
  ComboBox1->Clear();
  ComboBox2->Clear();

  Edit1->Clear();
  Edit2->Clear();
  Edit3->Clear();

  Memo1->Clear();

  Form1->Label9->Caption = "";

  Form1->Close();
}

void __fastcall TForm1::Label7Click(TObject *Sender) {
  Form2->Label4->Caption = STR_PROGRAMM_NAME;

  Form2->Shape1->Brush->Color = Form1->Shape2->Brush->Color;
  Form2->Shape1->Pen->Color   = Form1->Shape2->Pen->Color;

  Form2->Shape2->Pen->Color   = Form1->Shape1->Pen->Color;

  Form2->Label4->Color        = Form1->Label6->Color;
  Form2->Label4->Font->Color  = Form1->Label6->Font->Color;
  
  Form2->Label5->Color        = Form1->Label5->Color;
  Form2->Label5->Font->Color  = Form1->Label5->Font->Color;

  Form2->Label2->Caption      = PTCL_LICENSE_INFORMATION;
  Form2->Label3->Caption      = PTCL_AUTHORS_INFORMATION;

  Form2->Show();
}

void __fastcall TForm1::Button5Click(TObject *Sender) {
  int len;

  len = atoi(Edit3->Text.c_str());

  if ((len < 8) || (len > 256)) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_ENTER_8_TO_256);
    return;
  }

  PASSWORD_MEMORY_CTX * memory = (PASSWORD_MEMORY_CTX *)malloc(sizeof(PASSWORD_MEMORY_CTX));
  ARC4_CTX * arc4_ctx = (ARC4_CTX *)malloc(sizeof(ARC4_CTX));

  if (!memory || !arc4_ctx) {
    free(memory);
    free(arc4_ctx);

    MessageForUser(MB_ICONERROR + MB_OK, STR_ERROR_MSG, STR_MEMORY_BLOCKED);
    return;
  }

  CryptGenRandom(hcrypt, len, memory->input);

  /* random byte to 0x00 do 0xFF xor random memory block */
  for (int i = 0; i < len; i++) {
    memory->input[i] ^= (uint8_t)genrand(0x00, 0xFF);
  }

  arc4_init(arc4_ctx, memory->input, len);
  arc4(arc4_ctx, memory->input, memory->output, len);

  meminit(memory->input, 0x00, len);

  base64encode(memory->output, memory->input, len);
  memory->input[len] = 0x00;

  Memo1->Clear();
  Memo1->Lines->Text = AnsiString((char*)memory->input);
  Application->ProcessMessages();  

  meminit(arc4_ctx, 0x00, sizeof(ARC4_CTX));
  meminit(memory, 0x00, sizeof(PASSWORD_MEMORY_CTX));

  free(memory);
  free(arc4_ctx);
}

void __fastcall TForm1::Shape2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y) {
  ReleaseCapture();
  Form1->Perform(WM_SYSCOMMAND, 0xF012, 0);
}

TColor InverseColor(const TColor color) {
  return TColor((~(uint32_t)color) & 0x00FFFFFF); 
}

void __fastcall TForm1::Label5MouseEnter(TObject *Sender) {
  Label5->Font->Color = InverseColor(Label5->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label5MouseLeave(TObject *Sender) {
  Label5->Font->Color = InverseColor(Label5->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label7MouseEnter(TObject *Sender) {
  Label7->Font->Color = InverseColor(Label7->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label7MouseLeave(TObject *Sender) {
  Label7->Font->Color = InverseColor(Label7->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action) {
  DeleteCriticalSection(&CrSec);
  CryptProvDelete(&hcrypt);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender) {
  DeleteCriticalSection(&CrSec);
  CryptProvDelete(&hcrypt);
}
//---------------------------------------------------------------------------

