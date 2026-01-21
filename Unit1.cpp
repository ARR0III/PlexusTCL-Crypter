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
#define PTCL_RUSSIAN_LANGUAGE
#include "LANGUAGE_STRINGS.h"
/****************************************************************************/

#include "LICENSE.h"

#define SETTINGS_FILENAME    "settings.cry"
#define EMPTY_FILENAME       "output.dat"
#define EXT_CRYCON           ".crycon"
#define CIPHER_MODE          "-CFB"

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
#define CANNOT_ALLOCATE_MEMORY      11
#define CONTROL_SUM_FILE_ERROR      12
#define DATA_FROM_FILE_SMALL        13
#define KEY_LEN_INCORRECT           14
#define KEY_FILE_NOT_OPENNED        15
#define CIPHER_NOT_ENTER            16
#define KEY_SIZE_NOT_ENTER          17

#define PASS_SALT_SIZE              16
#define HMAC_DATA_BUFFER            64

#define STATUS_BUFFER_SIZE         256

#define SIZE_PASSWORD_GENERATE     512

#define LENGTH_DATA_FOR_CHECK     1024
#define MAX_SIZE_EDIT_STR         2048

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
        break;                                              \
      }                                                     \
                                                            \
      LeaveCriticalSection(&Form1->CrSec);                  \
    }                                                       \
  } while(0)

#define READ_PARAM_BYTE     "rb"
#define WRITE_PARAM_BYTE    "wb"
#define REWRITE_PARAM_BYTE  "r+b"

#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 * Form1;

/* Thread pointer for core works */
static Crycon     *thread    = NULL;

/* Provider crypt mode random numbers generator */
static HCRYPTPROV  hcrypt    = NULL;

/* Global section for syncronize thread and GUI */
static bool       PROCESSING = false;

/* For entropy */
uint8_t entrp[SHA256_BLOCK_SIZE]; /* NOT initialized == ALL OK */

/* Only sorted strings for bin search function */
#define CONFIG_KEYS_COUNT 7
static const char * CONFIG_KEYS[CONFIG_KEYS_COUNT] = {
  "CIPHER",
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
  "бт" , "КиБ", "МиБ", "ГиБ", "ТиБ", "ПиБ", "ЕиБ"
#else
  "bt" , "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"
#endif
};

static const char * OPERATION_NAME[] = {
#ifdef PTCL_RUSSIAN_LANGUAGE
  "Шифрование", "Расшифровка"
#else
  "Encrypting", "Decrypting"
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

static const fsize_t INT_SIZE_DATA[] = {
  (fsize_t)1 << 10, /* KiB */
  (fsize_t)1 << 20, /* MiB */
  (fsize_t)1 << 30, /* GiB */
  (fsize_t)1 << 40, /* TiB */
  (fsize_t)1 << 50, /* PiB */
  (fsize_t)1 << 60  /* EiB */
};

static const char * PARAM_APPEND_BYTE  = "ab";
static const char * PARAM_READ_BYTE    = "rb";
static const char * PARAM_WRITE_BYTE   = "wb";
static const char * PARAM_REWRITE_BYTE = "r+b";

static const TColor FORM_HEAD_COLOR        = TColor(0x00000000);
static const TColor FORM_HEAD_TEXT_B_COLOR = TColor(0x00000000);
static const TColor FORM_HEAD_TEXT_COLOR   = TColor(0x00FFFFFF);

/****************************************************************************/

__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
}

/*
void ShowMemoryDump(void * data, size_t size) {
  size_t hex_dump_size = size * 2 + 1;
  char * hex_dump = (char *)malloc(hex_dump_size);

  if (NULL == hex_dump) {
    ShowMessage("NO DATA FOR SHOW!\nERROR: cannot allocate memory!\n");
    return;
  }

  meminit(hex_dump, 0x00, hex_dump_size);

  strtohex(hex_dump, hex_dump_size, (char*)data, size);
  hex_dump[hex_dump_size-1] = 0x00;

  ShowMessage(hex_dump);

  meminit(hex_dump, 0x00, hex_dump_size);
  free(hex_dump);
}
*/

uint32_t MessageForUser(const int tumbler,
                        const char * head,
                        const char * message) {

  return Application->MessageBox(message, head, tumbler);
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
    if ((fsize_t)(fsize + SHA256_BLOCK_SIZE + PASS_SALT_SIZE + ctx->vector_length) >= ((fsize_t)1 << 62)) {
      return SIZE_FILE_VERY_BIG;
    }
  }
  else {
/* if fsize < minimal size, this file don't for decrypt */
    if (fsize < (fsize_t)(SHA256_BLOCK_SIZE + PASS_SALT_SIZE + ctx->vector_length + 1)) {
      return SIZE_DECRYPT_FILE_INCORRECT;
    }
  }

  return OK;
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

double sizetodoubleprint(const int status, const double size) {
  return (status ? (size / (double)INT_SIZE_DATA[status - 1]) : size);
}

int str_list_search(const char * str, const char * list[], const int length) {
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

void cursorpos(uint8_t * data) {
  TPoint position; /* position cursor write in initialization vector */

  GetCursorPos(&position);

  data[0] ^= (uint8_t)(position.x);
  data[1] ^= (uint8_t)(position.y);
  data[2] ^= (uint8_t)(position.x + position.y + 1);

  position.x = position.y = 0;
}

void LastHopeEntropyGen(uint8_t * trash, size_t size) {
  size_t i, j;
  SHA256_CTX ctx;

  sha256_init(&ctx);

  i = 0;
  j = size;

  *(uint32_t*)(&entrp[0]) ^= GetTickCount();

  while (i < size) {
    sha256_update(&ctx, entrp, SHA256_BLOCK_SIZE);
    sha256_final(&ctx);

    if (j > SHA256_BLOCK_SIZE)
      j = SHA256_BLOCK_SIZE;

    memxormove(entrp, ctx.hash, SHA256_BLOCK_SIZE);
    memcpy(trash + i, ctx.hash, j);

    i += SHA256_BLOCK_SIZE;
    j -= SHA256_BLOCK_SIZE;
  }
}

void LastHopeEntropyInit(void) {
  size_t i, j;
  SHA256_CTX ctx;

/* i'am know what not good, but me ok */
  memxormove(entrp, ctx.hash, SHA256_BLOCK_SIZE);

  /* entrp - global array for entropy */
  *(uint32_t*)(&entrp[0])  ^= GetCurrentProcessId();
  *(uint32_t*)(&entrp[4])  ^= GetCurrentThreadId();
  *(uint32_t*)(&entrp[8])  ^= GetTickCount();
  *(uint32_t*)(&entrp[12]) ^= GetACP();
  *(uint32_t*)(&entrp[16]) ^= (uint32_t)(&ctx);
  *(uint32_t*)(&entrp[20]) ^= (uint32_t)(&LastHopeEntropyInit);
  *(uint32_t*)(&entrp[24]) ^= (uint32_t)(&entrp) ^ (uint32_t)(&LastHopeEntropyGen);

  for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
    entrp[i] ^= (uint8_t)genrand(0x00, 0xFF);
  }

  cursorpos(entrp); // X and Y cursor position xor operation for data[0] and data[1];

  sha256_init(&ctx);
  sha256_update(&ctx, entrp, SHA256_BLOCK_SIZE);
  sha256_final(&ctx);

  memxormove(entrp, ctx.hash, SHA256_BLOCK_SIZE);
}

void StrongRandomGen(uint8_t * data, size_t size) {
  size_t i;

  CryptGenRandom(hcrypt, size, data);

  for (i = 0; i < size; i++) {
    data[i] ^= (uint8_t)genrand(0x00, 0xFF);
  }

  cursorpos(data); // X and Y cursor position xor operation for data[0] and data[1];

/* If the crypto provider doesn't work */
/* I did everything I could and I'm praying for you */
  LastHopeEntropyGen(data, size);
}

int operation_variant(const int operation) {
  return (operation ? 1 : 0);
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

/* CLOMUL_CONST - security level. CLOMUL_CONST = 1 */
static bool KDFCLOMUL(GLOBAL_MEMORY * ctx,
                      const uint8_t * password, const size_t password_len,
                            uint8_t * key,      const size_t key_len) {

  int      real, past = 0;
  uint32_t count;
  size_t   i, j, k, div;

  ctx->hash_matrix_length = (key_len + password_len + CLOMUL_CONST) *
                             SHA256_BLOCK_SIZE * 1024 * CLOMUL_CONST;

  ctx->hash_matrix = (uint8_t *)malloc(ctx->hash_matrix_length);

  if (NULL == ctx->hash_matrix) {
    return false;
  }

  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, password, password_len);
  sha256_update(ctx->sha256sum, (uint8_t *)ctx->pass_salt, PASS_SALT_SIZE);
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
    sha256_update(ctx->sha256sum, (uint8_t *)ctx->pass_salt, PASS_SALT_SIZE);

    if (!(i & div)) continue; /* if i != 1/10 */

    real = (size_t)((double)i / ((double)div * 0.1L) + 0.1);

    if (real > 100) {
      real = 100;
    }

    if (real > past) {
      Form1->ProgressBar1->Position = real;

      Form1->Label7->Caption =
#ifdef PTCL_RUSSIAN_LANGUAGE
        "Генерация "
         + IntToStr(key_len * 8)  + "-битного ключа: " + IntToStr(real) + " %";
#else
        "Generating "
          + IntToStr(key_len * 8) + "-bit key: "  + IntToStr(real) + " %";
#endif
      Application->ProcessMessages();
      past = real;
    }
  }

  for (i = 0; i < ctx->hash_matrix_length; i += SHA256_BLOCK_SIZE) {
    sha256_update(ctx->sha256sum, password, password_len);
	sha256_update(ctx->sha256sum, (uint8_t *)ctx->pass_salt, PASS_SALT_SIZE);
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

//-----------------------------------------------------------------------------

__fastcall Crycon::Crycon(bool CreateSuspended):TThread(CreateSuspended) {
  Init();
}

__fastcall Crycon::~Crycon() {
  ButtonUpdate();
  ShowStatus();
  Clear();
}

FILE * __fastcall Crycon::GetFile() {
  return ctx->file_input;
}

fsize_t __fastcall Crycon::GetFileSize() {
  return fsize;
}

int __fastcall Crycon::GetStatus() {
  return status;
}

size_t __fastcall Crycon::GetKeySize() {
  return ctx->real_key_length;
}

int __fastcall Crycon::GetOperation() {
  return ctx->operation;
}

cipher_t __fastcall Crycon::GetCipher() {
  return ctx->cipher;
}

void __fastcall Crycon::Init() {
  ctx           = NULL;
  cipher_ptr    = NULL;
  cipher_length = 0;
  fsize         = 0;
  position      = 0;
  re_keying     = 0;
  status        = OK;
  
  ctx = (GLOBAL_MEMORY *)malloc(sizeof(GLOBAL_MEMORY));

  if (NULL == ctx) {
    status = CANNOT_ALLOCATE_MEMORY;
    return;
  }

  meminit(ctx, 0x00, sizeof(GLOBAL_MEMORY));
}

void __fastcall Crycon::Clear() {
  if (NULL == ctx) {
    return;
  }
  
  if (fclose(ctx->file_input) == -1) {
    fclose(ctx->file_output);
    status = STREAM_INPUT_CLOSE_ERROR;
  }

  if (fclose(ctx->file_output) == -1) {
    status = STREAM_OUTPUT_CLOSE_ERROR;
  }

  if (cipher_ptr) {
    meminit(cipher_ptr, 0x00, cipher_length);
  }

  if (ctx->hash_matrix) {
    meminit(ctx->hash_matrix, 0x00, ctx->hash_matrix_length);
  }

  if (ctx->vector) {
    meminit(ctx->vector, 0x00, ctx->vector_length);
  }

  if (ctx->real_key) {
    meminit(ctx->real_key, 0x00, ctx->real_key_length);
  }

  if (ctx->new_key) {
    meminit(ctx->new_key, 0x00, ctx->new_key_length);
  }

  meminit(entrp, 0x00, SHA256_BLOCK_SIZE);

  free(ctx->hash_matrix);
  free(ctx->vector);
  free(ctx->real_key);
  free(ctx->new_key);
  free(cipher_ptr);

  meminit(ctx, 0x00, sizeof(GLOBAL_MEMORY));
  free(ctx);

  ctx           = NULL;
  cipher_ptr    = NULL;
  cipher_length = 0;
  fsize         = 0;
  position      = 0;
  re_keying     = 0;
  status        = OK;
}

void __fastcall Crycon::ProgressBar() {
  Form1->ProgressBar1->Position = real_percent;
  Form1->Label7->Caption = AnsiString(&ctx->status_buffer[0]);
  Application->ProcessMessages();
}

void __fastcall Crycon::ButtonUpdate() {
  Form1->ButtonStart->Caption = STR_START;
  Form1->ButtonStart->Update();

  Form1->ProgressBar1->Position = 0;
  Form1->ProgressBar1->Update();
}


bool __fastcall Crycon::GeneratingCryptKey(const char * message) {
  if (MessageForUser(MB_ICONINFORMATION + MB_YESNO, STR_PROGRAMM_NAME, message) == IDNO) {
    status = OPERATION_BREAK;
    return false;
  }

  return true;
}

/* RFC-4868
  hmac = h(opad xor 0x5C || h(ipad xor 0x36 || m)); */
void __fastcall Crycon::HMAC_Calculate() {
  int i;

  /* hash sum of file now in filehash buffer */
  memcpy(ctx->hmac_data.temp, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
  /* generate two secret const for hash update */

  meminit(ctx->hmac_data.ipad, 0x00, HMAC_DATA_BUFFER);
  meminit(ctx->hmac_data.opad, 0x00, HMAC_DATA_BUFFER);

  if (ctx->real_key_length > HMAC_DATA_BUFFER) {
    meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));

    sha256_init(ctx->sha256sum);
    sha256_update(ctx->sha256sum, ctx->real_key, ctx->real_key_length);
    sha256_final(ctx->sha256sum);

    /* rewrite keys hash sum of key */
    memcpy(ctx->hmac_data.ipad, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
    memcpy(ctx->hmac_data.opad, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
  }
  else {
    memcpy(ctx->hmac_data.ipad, ctx->real_key, ctx->real_key_length);
    memcpy(ctx->hmac_data.opad, ctx->real_key, ctx->real_key_length);
  }

  for (i = 0; i < HMAC_DATA_BUFFER; i++) {
    ctx->hmac_data.ipad[i] ^= 0x36;
    ctx->hmac_data.opad[i] ^= 0x5C;
  }

  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, ctx->hmac_data.ipad, HMAC_DATA_BUFFER);
  sha256_update(ctx->sha256sum, ctx->hmac_data.temp, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  memcpy(ctx->hmac_data.temp, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);

  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  sha256_init(ctx->sha256sum);
  sha256_update(ctx->sha256sum, ctx->hmac_data.opad, HMAC_DATA_BUFFER);
  sha256_update(ctx->sha256sum, ctx->hmac_data.temp, SHA256_BLOCK_SIZE);
  sha256_final(ctx->sha256sum);

  /* clear  buffers for security */
  meminit(&(ctx->hmac_data), 0x00, sizeof(HMAC_CTX));
  /* now control sum crypt key and file in buffer ctx->sha256sum->hash */
}

void __fastcall Crycon::InternalReKeying() {
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
  InitCipherKey();
}

void __fastcall Crycon::Encrypt() {
      switch (ctx->cipher) {
        case AES:
          rijndael_encrypt((uint32_t *)cipher_ptr, ctx->vector, ctx->cipher_text + nblock);
          break;
        case BLOWFISH:
          blowfish_encrypt((BLOWFISH_CTX *)cipher_ptr, (uint32_t *)ctx->vector, (uint32_t *)(ctx->vector + 4));
          memcpy(ctx->cipher_text + nblock, ctx->vector, ctx->vector_length);
          break;
        case SERPENT:
          serpent_encrypt((SERPENT_CTX *)cipher_ptr, (uint32_t *)ctx->vector, (uint32_t *)(ctx->cipher_text + nblock));
          break;
        case THREEFISH:
          threefish_encrypt((THREEFISH_CTX *)cipher_ptr, (uint64_t*)ctx->vector, (uint64_t*)(ctx->cipher_text + nblock));
          break;
        case TWOFISH:
          twofish_encrypt((TWOFISH_CTX *)cipher_ptr, ctx->vector, ctx->cipher_text + nblock);
          break;
        }
}

void __fastcall Crycon::Execute() {
  Priority        = tpNormal;
  FreeOnTerminate = true;	
	
  SET_START_STREAM;
  
  meminit(ctx->sha256sum, 0x00, sizeof(SHA256_CTX));
  sha256_init(ctx->sha256sum);

  while (position < fsize) {
    if (0LL == position) {
      if (ENCRYPT == ctx->operation) {
        Encrypt();
        memcpy(ctx->vector, ctx->cipher_text, ctx->vector_length);

        if (fwrite(ctx->vector, 1, ctx->vector_length, ctx->file_output) != ctx->vector_length) {
          status = WRITE_FILE_ERROR;
          break;
        }

        if (ferror(ctx->file_output)) {
          status = WRITE_FILE_ERROR;
          break;
        }

        fflush(ctx->file_output);
      }
      else
      if (DECRYPT == ctx->operation) {
        if (fread(ctx->vector, 1, ctx->vector_length, ctx->file_input) != ctx->vector_length) {
          status = READ_FILE_ERROR;
          break;
        }

        if (ferror(ctx->file_input)) {
          status = READ_FILE_ERROR;
          break;
        }
        position += (fsize_t)ctx->vector_length;
      }
    }

    realread = fread(ctx->plain_text, 1, DATA_SIZE, ctx->file_input);

    if (ferror(ctx->file_input)) {
      status = READ_FILE_ERROR;
      break;
    }

    if (Terminated) {
      status = OPERATION_BREAK;
      break;
    }

    CHECK_BREAK_STREAM;

    for (nblock = 0; nblock < realread; nblock += ctx->vector_length) {
      Encrypt();
      memxormove(ctx->cipher_text + nblock, ctx->plain_text + nblock, ctx->vector_length);
      memcpy(ctx->vector, (ctx->operation ? ctx->plain_text : ctx->cipher_text) + nblock, ctx->vector_length);

      if (Terminated) {
        status = OPERATION_BREAK;
        break;
      }
    }

       position  += (fsize_t)realread;
    real_percent  = (int)((double)position / div + 0.1);

    if (real_percent > 100) {
      real_percent = 100;
    }

    if ((position == fsize) && (DECRYPT == ctx->operation)) {
      realread -= SHA256_BLOCK_SIZE;
    }

    HashCalculate(realread);

    if (Terminated) {
      status = OPERATION_BREAK;
      break;
    }

    if (fwrite(ctx->cipher_text, 1, realread, ctx->file_output) != realread) {
      status = WRITE_FILE_ERROR;
      break;
    }

    if (ferror(ctx->file_output)) {
      status = WRITE_FILE_ERROR;
      break;
    }

    fflush(ctx->file_output);

    if (Terminated) {
      status = OPERATION_BREAK;
      break;
    }

    CHECK_BREAK_STREAM;

    if (real_percent > past_percent) {
      check = size_check(position);

      snprintf(ctx->status_buffer + status_buffer_pos,
               STATUS_BUFFER_SIZE - status_buffer_pos,
               STR_PROGRESS_BAR_MKS,
               check ? (double)position / (double)INT_SIZE_DATA[check-1] : (double)position,
               CHAR_SIZE_DATA[check],
               fsize_double,
               CHAR_SIZE_DATA[fsize_check],
               real_percent);

      Synchronize(ProgressBar);
      past_percent = real_percent;
    }

    re_keying += realread;

//  if crypt key using for en/decrypt very long time --> regenerating crypt key
//  limit en/decrypt bytes = 2 GB
//  new_key = sha256sum(old_key);
    if (re_keying >= 0x80000000) {
      re_keying = 0;
      InternalReKeying();
    }
  }

  if (status != OK) {
    SET_STOP_STREAM;
    return;
  }

  sha256_final(ctx->sha256sum);
  HMAC_Calculate();

  if (ENCRYPT == ctx->operation) {
    if (fwrite(ctx->sha256sum->hash, 1, SHA256_BLOCK_SIZE, ctx->file_output) != SHA256_BLOCK_SIZE) {
      status = WRITE_FILE_ERROR;
      SET_STOP_STREAM;
      return;
    }

    if (ferror(ctx->file_output)) {
      status = WRITE_FILE_ERROR;
      SET_STOP_STREAM;
      return;
    }

    fflush(ctx->file_output);
  }
  else {
    if (HMAC_Check(realread) == false) {
      status = CONTROL_SUM_FILE_ERROR;
      SET_STOP_STREAM;
      return;
    }
  }

  SET_STOP_STREAM;
}

bool __fastcall Crycon::SetInputFile(const char * name) {
  ctx->file_input = fopen(name, READ_PARAM_BYTE);

  if (NULL == ctx->file_input) {
    status = READ_FILE_NOT_OPEN;
    return false;
  }

  return true;
}

bool __fastcall Crycon::SetOutputFile(const char * name) {
  ctx->file_output = fopen(name, WRITE_PARAM_BYTE);

  if (NULL == ctx->file_output) {
    status = WRITE_FILE_NOT_OPEN;
    return false;
  }

  return true;
}

int __fastcall Crycon::WriteSalt() {
  if (fwrite(ctx->pass_salt, 1, PASS_SALT_SIZE, ctx->file_output) != PASS_SALT_SIZE) {
    return WRITE_FILE_ERROR;
  }
  fflush(ctx->file_output);
  return OK;
}

int __fastcall Crycon::ReadSalt() {
  if (fread(ctx->pass_salt, 1, PASS_SALT_SIZE, ctx->file_input) != PASS_SALT_SIZE) {
    return READ_FILE_ERROR;
  }

  return OK;
}

/* if not OK then BAD */
int __fastcall Crycon::PassSaltInit() {
  int result = OK;

  switch(ctx->operation) {
    case ENCRYPT:
      StrongRandomGen((uint8_t *)ctx->pass_salt, PASS_SALT_SIZE);
      result = WriteSalt();
      break;
    case DECRYPT:
      result = ReadSalt();
      break;
  }

  /* operation result this is status object */
  status = result;
  
  return result;
}

bool __fastcall Crycon::SetCipher(const char * cipher_name) {
  int result = str_list_search(cipher_name, ALGORITM_NAME, ALGORITM_NAME_COUNT);

  if (-1 == result) {
    status = CIPHER_NOT_ENTER;
    return false;
  }

  ctx->cipher = (cipher_t)result;
  return true;
}

int __fastcall Crycon::SetKeyLen(const char * key_size, int * aes) {
  int result;

  if (ctx->cipher == THREEFISH) {
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
    else {
      status = KEY_SIZE_NOT_ENTER;
      return false;
    }
  }
  else
  if (ctx->cipher == BLOWFISH) {
    ctx->real_key_length = 448;
  }
  else { /*  AES || SERPENT || TWOFISH  */
    result = str_list_search(key_size, CHAR_KEY_LENGTH_AES, CHAR_KEY_LENGTH_AES_COUNT);

    if (result == -1) {
      status = KEY_SIZE_NOT_ENTER;
      return false;
    }

    if (ctx->cipher == AES) {
      /* 0 == 128, 1 == 192, 2 = 256 bits */
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

  switch(ctx->cipher) {
    case AES:
      ctx->vector_length = 16;
      cipher_length = 4 * (*aes + 1) * 4;
      break;

    case BLOWFISH:
      ctx->vector_length =  8;
      cipher_length = sizeof(BLOWFISH_CTX);
      break;

    case SERPENT:
      ctx->vector_length = 16;
      cipher_length = sizeof(SERPENT_CTX);
      break;

    case THREEFISH:
      ctx->vector_length = ctx->real_key_length;
      cipher_length = sizeof(THREEFISH_CTX);
      break;

    case TWOFISH:
      ctx->vector_length = 16;
      cipher_length = sizeof(TWOFISH_CTX);
      break;
  }

  ctx->real_key_length = ctx->real_key_length / 8;
  ctx->new_key_length  = ctx->real_key_length;

  cipher_ptr = (void *)malloc(cipher_length);

  if (NULL == cipher_ptr) {
    status = CANNOT_ALLOCATE_MEMORY;
    return false;
  }

  ctx->real_key = (uint8_t *)malloc(ctx->real_key_length);

  if (NULL == ctx->real_key) {
    status = CANNOT_ALLOCATE_MEMORY;
    return false;
  }

  ctx->new_key = (uint8_t *)malloc(ctx->new_key_length);

  if (NULL == ctx->new_key) {
    status = CANNOT_ALLOCATE_MEMORY;
    return false;
  }

  ctx->vector = (uint8_t *)malloc(ctx->vector_length);

  if (NULL == ctx->vector) {
    status = CANNOT_ALLOCATE_MEMORY;
    return false;
  }

  meminit(ctx->real_key, 0x00, ctx->real_key_length);
  meminit(ctx->new_key,  0x00, ctx->new_key_length);
  meminit(ctx->vector,   0x00, ctx->vector_length);
  meminit(cipher_ptr,    0x00, cipher_length);

  return true;
}

void _fastcall Crycon::SetOperation(const int operation) {
  ctx->operation = operation;

  if (ENCRYPT == ctx->operation) {
    StrongRandomGen(ctx->vector, ctx->vector_length);
  }

  status_buffer_pos = snprintf(ctx->status_buffer, STATUS_BUFFER_SIZE,
    STR_ENCRYPT_FILE_PROC,
    OPERATION_NAME[ctx->operation ? 1 : 0],
    ALGORITM_NAME[ctx->cipher]);
}

void __fastcall Crycon::SetStatus(const int x) {
  status = x;
}

bool __fastcall Crycon::GenKeyFromFile(const char * key_file) {
  int real_read = readfromfile(key_file, ctx->new_key, ctx->new_key_length);

  if ((real_read > 0) && (real_read < (int)(ctx->new_key_length))) {
    status = DATA_FROM_FILE_SMALL;
    return false;
  }
  else
  if ((real_read == 0) || (real_read == -1)) {
    status = KEY_FILE_NOT_OPENNED;
    return false;
  }

  /* all data by read */
  if (KDFCLOMUL(ctx, ctx->new_key, ctx->new_key_length, ctx->real_key, ctx->real_key_length) == false) {
    status = CANNOT_ALLOCATE_MEMORY;
    return false;
  }

  meminit(ctx->new_key, 0x00, ctx->new_key_length);
  /* if by read ctx->real_key_length byte */
  return true;
}

bool __fastcall Crycon::KeyGenFromPass(const char * password) {
  int length = (int)x_strnlen(password, 256);

  if (length < 8 || length > 256) {
    status = KEY_LEN_INCORRECT;
    return false;
  }

  if (KDFCLOMUL(ctx, password, length, ctx->real_key, ctx->real_key_length) == false) {
    status = CANNOT_ALLOCATE_MEMORY;
    return false;
  }

  return true;
}

void __fastcall Crycon::InitCipherKey() {
  switch(ctx->cipher) {
    case AES:
      rijndael_key_encrypt_init((uint32_t *)cipher_ptr, ctx->real_key, ctx->real_key_length * 8);
      break;
    case BLOWFISH:
      blowfish_init((BLOWFISH_CTX *)cipher_ptr, ctx->real_key, ctx->real_key_length);
      break;
    case SERPENT:
      serpent_init((SERPENT_CTX *)cipher_ptr, ctx->real_key_length * 8, ctx->real_key);
      break;
    case THREEFISH:
      threefish_init((THREEFISH_CTX *)cipher_ptr, (threefishkeysize_t)(ctx->real_key_length * 8),
                     (uint64_t *)ctx->real_key, (uint64_t *)ctx->real_key);
      break;
    case TWOFISH:
      twofish_init((TWOFISH_CTX *)cipher_ptr, ctx->real_key, ctx->real_key_length);
      break;
  }
}

bool __fastcall Crycon::CorrectSizeOfFile(const char * name) {
  fsize        = SizeOfFile(name);
  fsize_check  = size_correct(ctx, fsize);

  if (fsize_check != OK) {
    status = fsize_check;
    return false;
  }

  if (DECRYPT == ctx->operation)
    fsize -= PASS_SALT_SIZE;

  div = (double)fsize * 0.01L;

  fsize_check  = size_check(fsize);
  fsize_double = sizetodoubleprint(fsize_check, (double)fsize);

  return true;
}

void __fastcall Crycon::HashCalculate(size_t count) {
  size_t       i = 0;
  size_t remnant = count;
  /* if operation == ENCRYPT then calculate hash sum of ciphertext */
  uint8_t * data = ctx->operation ? ctx->plain_text : ctx->cipher_text;

  while (i < count) {
    if (remnant < LENGTH_DATA_FOR_CHECK) {
      sha256_update(ctx->sha256sum, data + i, remnant);
    }
    else { /* if remnant >= LENGTH_DATA_FOR_CHECK */
      sha256_update(ctx->sha256sum, data + i, LENGTH_DATA_FOR_CHECK);
    }

          i += LENGTH_DATA_FOR_CHECK;
    remnant -= LENGTH_DATA_FOR_CHECK;
  }
}

bool __fastcall Crycon::HMAC_Check(size_t pos) {
  return !(bool)_memcmp_s(ctx->plain_text + pos, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);
}

void __fastcall Crycon::ShowStatus() {
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
    case CANNOT_ALLOCATE_MEMORY:
      MessageForUser(MB_ICONERROR + MB_OK, STR_PROGRAMM_NAME,
                     STR_MEMORY_BLOCKED);
      break;
    case CONTROL_SUM_FILE_ERROR:
	  MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_CONTROL_SUM_FILE_ERROR);
      break;
    case DATA_FROM_FILE_SMALL:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_DATA_FROM_FILE_SMALL);
      break;
    case KEY_LEN_INCORRECT:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_KEY_LEN_INCORRECT);
      break;
    case KEY_FILE_NOT_OPENNED:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_KEY_FILE_NOT_OPENNED);
      break;
    case CIPHER_NOT_ENTER:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_CIPHER_NOT_ENTER);
      break;
    case KEY_SIZE_NOT_ENTER:
      MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                     STR_KEY_SIZE_NOT_ENTER);
      break;
  }
}

void AddListKeys(const cipher_t cipher) {
  int i;

  Form1->KeyLenBox->Items->Clear();

  if (cipher == THREEFISH) {
	for (i = 0; i < 3; i++) {
	  Form1->KeyLenBox->Items->Add(CHAR_KEY_LENGTH_THREEFISH[i]);
	}
	Form1->KeyLenBox->Text = AnsiString(CHAR_KEY_LENGTH_THREEFISH[0]);
  }
  else {
	for (i = 0; i < 3; i++) {
	  Form1->KeyLenBox->Items->Add(CHAR_KEY_LENGTH_AES[i]);
	}
	Form1->KeyLenBox->Text = AnsiString(CHAR_KEY_LENGTH_AES[0]);
  }
}

void CryptProvDelete(HCRYPTPROV * hcp) {
  if (*hcp) {
    CryptReleaseContext(*hcp, 0);
    *hcp = NULL;
  }
}

void __fastcall TForm1::Label9Click(TObject *Sender) {
  DeleteCriticalSection(&CrSec);
  CryptProvDelete(&hcrypt);

  CipherBox->Clear();
  KeyLenBox->Clear();

  InputFile->Clear();
  OutputFile->Clear();
  PassLen->Clear();

  MemoKey->Clear();

  Form1->Label7->Caption = "";

  Form1->Close();
}
//---------------------------------------------------------------------------

const char * CharA_Or_CharOV(size_t length) {
  return (24 == length || 128 == length) ? " бита" : " бит";
}

void __fastcall TForm1::ButtonStartClick(TObject *Sender) {
  int result;
  extern int AES_Rounds; /* in rijndael.c source code file */
  uint32_t error_delete;
  String UnicodeMsg = STR_STOP_PROCESS;

  EnterCriticalSection(&Form1->CrSec);
  if (PROCESSING) {
    thread->Suspend();
    if (MessageForUser(MB_ICONQUESTION + MB_YESNO, STR_PROGRAMM_NAME, UnicodeMsg.c_str()) == IDYES) {
      PROCESSING = false;
      LeaveCriticalSection(&Form1->CrSec);
      
      thread->Resume();
      thread->Terminate();
      //thread->WaitFor();
      //delete thread;
    }
    else {
      thread->Resume();
      LeaveCriticalSection(&Form1->CrSec);
    }

    return;
  }
  LeaveCriticalSection(&Form1->CrSec);
/*****************************************************************************/  
  if (x_strnlen(InputFile->Text.c_str(), MAX_SIZE_EDIT_STR) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_INPUT_FILENAME_EMPTY);
    return;
  }

  if (x_strnlen(OutputFile->Text.c_str(), MAX_SIZE_EDIT_STR) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_OUTPUT_FILENAME_EMPTY);
    return;
  }

  if (x_strnlen(MemoKey->Text.c_str(), MAX_SIZE_EDIT_STR) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_PASS_OR_KEYFILE_EMPTY);
    return;
  }
/*****************************************************************************/
  if (InputFile->Text == OutputFile->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_IN_OUT_FILENAME_EQUAL);
    return;
  }

  if (InputFile->Text == MemoKey->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_OUT_KEY_FILENAME_EQUAL);
    return;
  }

  if (OutputFile->Text == MemoKey->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_IN_KEY_FILENAME_EQUAL);
    return;
  }
/*****************************************************************************/
  if (FileExists(InputFile->Text) == False) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_IN_FILENAME_NFOUND);
    return;
  }

  if (FileExists(OutputFile->Text) == True) {
    UnicodeMsg = STR_OUT_FILE_FOUND;

    if (MessageForUser(MB_ICONWARNING + MB_YESNO, STR_WARNING_MSG, UnicodeMsg.c_str()) == IDNO) {
      MessageForUser(MB_ICONINFORMATION + MB_OK, STR_PROGRAMM_NAME,
                     STR_CHANGE_OUT_FILENAME);
      return;
    }
  }
/*****************************************************************************/
/* if object to delete, then show status operation in window */
  thread = new Crycon(true);

  if (NULL == thread) {
    MessageForUser(MB_ICONERROR + MB_OK, STR_PROGRAMM_NAME,
                   STR_MEMORY_BLOCKED);
    return;
  }

  if (thread->SetCipher(CipherBox->Text.c_str()) == false) {
    delete thread;
    return;
  }

  if (thread->SetKeyLen(KeyLenBox->Text.c_str(), &AES_Rounds) == false) {
    delete thread;
    return;
  }

  if (RadioEncrypt->Checked) {
    result = ENCRYPT;
  }
  else
  if (RadioDecrypt->Checked) {
    result = DECRYPT;
  }
  else {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
                   STR_OPERATION_NOT_ENTER);
    delete thread;
    return;
  }

  thread->SetOperation(result);

  if (thread->CorrectSizeOfFile(InputFile->Text.c_str()) == false) {
    delete thread;
    return;
  }

  if (thread->SetInputFile(InputFile->Text.c_str()) == false) {
    delete thread;
    return;
  }

  if (thread->SetOutputFile(OutputFile->Text.c_str()) == false) {
    delete thread;
    return;
  }

  if (thread->PassSaltInit() != OK) {
    delete thread;
    return;
  }

  if (FileExists(MemoKey->Text) == true) {
    UnicodeMsg =
#ifdef PTCL_RUSSIAN_LANGUAGE
    "Использовать " + IntToStr(thread->GetKeySize() * 8) + "-битный ключ шифрования из файла?\n";
#else
    "Use " + IntToStr(thread->GetKeySize() * 8) + "-bit encryption key from file?\n";
#endif

    if (thread->GeneratingCryptKey(UnicodeMsg.c_str()) == false) {
      delete thread;
      return;
    }
	
    ButtonStart->Enabled = false;
    if (thread->GenKeyFromFile(MemoKey->Text.c_str()) == false) {
      ButtonStart->Enabled = true;
      delete thread;
      return;
    }
    ButtonStart->Enabled = true;
  }
  else {
    UnicodeMsg =
#ifdef PTCL_RUSSIAN_LANGUAGE
    "Сгенерировать " + IntToStr(thread->GetKeySize() * 8) +
    "-битный ключ шифрования из пароля?\n";
#else
    "Generate " + IntToStr(thread->GetKeySize() * 8) +
    "-bit encryption key from password?\n";
#endif

    if (thread->GeneratingCryptKey(UnicodeMsg.c_str()) == false) {
      delete thread; // FUCKING BORLAND!!! WHY DOUBLE FREE OBJECT???
      return;
    }

    ButtonStart->Enabled = false;
    if (thread->KeyGenFromPass(MemoKey->Text.c_str()) == false) {
      ButtonStart->Enabled = true;
      delete thread;
      return;
    }
    ButtonStart->Enabled = true;
  }

  UnicodeMsg =
#ifdef PTCL_RUSSIAN_LANGUAGE
  "Приступить к выполнению выбранной операции?\n\n"
  "Операция:\t" + String(OPERATION_NAME[thread->GetOperation() ? 1 : 0]) + "\n"
  "Алгоритм:\t" + String(ALGORITM_NAME[thread->GetCipher()]) + CIPHER_MODE + "\n"
  "Длина ключа:\t" + IntToStr(thread->GetKeySize() * 8).c_str() +
    CharA_Or_CharOV(thread->GetKeySize());
#else
  "Proceed with the operation you selected?\n\n"
  "Operation:\t" + String(OPERATION_NAME[thread->GetOperation() ? 1 : 0]) + "\n"
  "Algorithm:\t" + String(ALGORITM_NAME[thread->GetCipher()]) + CIPHER_MODE + "\n"
  "Key length:\t" + IntToStr(thread->GetKeySize() * 8).c_str() + " bit";
#endif

  if (MessageForUser(MB_ICONQUESTION + MB_YESNO, STR_PROGRAMM_NAME, UnicodeMsg.c_str()) == IDNO) {
    thread->SetStatus(OPERATION_BREAK);
    delete thread;
    return;
  }

  Form1->ProgressBar1->Position = 0;
  Form1->ProgressBar1->Update();

  ButtonStart->Caption = STR_STOP;
  ButtonStart->Update();

  thread->InitCipherKey();
  thread->Resume();
}

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action) {
  DeleteCriticalSection(&CrSec);
  CryptProvDelete(&hcrypt);
}

void __fastcall TForm1::FormCreate(TObject *Sender) {
  SETTINGS settings;

  InitializeCriticalSection(&CrSec);
  srand(time(NULL) + (uint32_t)(&settings));

  if (!CryptAcquireContext(&hcrypt, NULL, NULL, PROV_RSA_FULL, 0)) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG,
#ifdef PTCL_RUSSIAN_LANGUAGE
      "Криптопровайдер Microsoft Windows недоступен!"
#else
      "Microsoft Windows cryptographic provider is unavailable!"
#endif
    );
  }

  LastHopeEntropyInit(); /* WARNING: custom entropy generator! */

  settings.top_color        = FORM_HEAD_COLOR;
  settings.top_text_color   = FORM_HEAD_TEXT_COLOR;
  settings.top_text_b_color = FORM_HEAD_TEXT_B_COLOR;
  settings.prog_bar_color   = clGreen;
  settings.cipher           = AES;
  settings.key_size         = 128;
  settings.pass_gen_size    =  32;
  settings.operation        = ENCRYPT;

  init_settings(SETTINGS_FILENAME, &settings);
  SendMessage(ProgressBar1->Handle, PBM_SETBARCOLOR, 0, settings.prog_bar_color);

  for (int i = 0; i < 5; i++) {
    CipherBox->Items->Add(ALGORITM_NAME[i]);
  }

  Form1->Caption         = STR_PROGRAMM_NAME;
  Form1->Label1->Caption = STR_PROGRAMM_NAME;

  Form1->Label2->Caption = STR_CRYPT_ALG;
  Form1->Label4->Caption = STR_INPUT_FILENAME;
  Form1->Label5->Caption = STR_OUTPUT_FILENAME;
  Form1->Label3->Caption = STR_KEY_LENGTH;
  Form1->Label6->Caption = STR_KEY_OR_KEY_FILENAME;
  Form1->Label7->Caption = STR_STATUS;

  Form1->ButtonInput->Caption  = STR_OPEN;
  Form1->ButtonOutput->Caption = STR_OPEN;
  Form1->ButtonKey->Caption    = STR_OPEN;
  Form1->ButtonStart->Caption  = STR_START;
  Form1->ButtonKeyGen->Caption = STR_KEYGEN;

  Form1->RadioEncrypt->Caption = STR_EN;
  Form1->RadioDecrypt->Caption = STR_DE;

/* this is code work with data from configurate file */

  Form1->Shape1->Pen->Color    = settings.top_color;
  Form1->Shape1->Brush->Color  = settings.top_color;

  Form1->Shape2->Pen->Color    = settings.top_color;
  //Form1->Shape2->Brush->Color  = settings.top_color;

  Form1->Label1->Color         = settings.top_text_b_color;
  Form1->Label8->Color         = settings.top_text_b_color;
  Form1->Label9->Color         = settings.top_text_b_color;

  Form1->Label1->Font->Color   = settings.top_text_color;
  Form1->Label8->Font->Color   = settings.top_text_color;
  Form1->Label9->Font->Color   = settings.top_text_color;

  Form1->PassLen->Text         = IntToStr(settings.pass_gen_size);
  Form1->CipherBox->Text       = AnsiString(ALGORITM_NAME[settings.cipher]);

  if (settings.cipher == BLOWFISH) {
    Label3->Visible = False;
    Form1->KeyLenBox->Visible = False;
  }
  else { /* other ciphers */
    AddListKeys(settings.cipher);

    Label3->Visible = True;
    Form1->KeyLenBox->Visible = True;

    Form1->KeyLenBox->Text = IntToStr(settings.key_size);
  }

  switch(settings.operation) {
    case ENCRYPT:  Form1->RadioEncrypt->Checked = true;
                   break;
    case DECRYPT:  Form1->RadioDecrypt->Checked = true;
                   break;

  }

  Form1->CipherBox->Style = csDropDown;
  Form1->CipherBox->Style = csDropDown;

/* this is code work with data from configurate file */

  Form1->ProgressBar1->Min = 0;
  Form1->ProgressBar1->Max = 100;
}
//---------------------------------------------------------------------------


void PathToStrings(const AnsiString &FileName, AnsiString &path, AnsiString &name, AnsiString &ext) {
  path = ExtractFilePath(FileName);
  name = ExtractFileName(FileName);
  ext  = ExtractFileExt(FileName);
}

void __fastcall TForm1::ButtonInputClick(TObject *Sender) {
  OpenDialog1->Title = STR_INPUT_FILENAME;

  AnsiString path;
  AnsiString name;
  AnsiString ext;

  if (OpenDialog1->Execute()) {
    PathToStrings(OpenDialog1->FileName, path, name, ext);

    Form1->InputFile->Clear();
    Form1->InputFile->Text = OpenDialog1->FileName;

    Form1->OutputFile->Clear();

    if (ext.LowerCase() == EXT_CRYCON) {
      if (name.Length() < 8) {
        Form1->OutputFile->Text = path + EMPTY_FILENAME;
      }
      else {
        Form1->OutputFile->Text = path + name.SetLength(name.Length() - 7);
      }
    }
    else {
      Form1->OutputFile->Text = OpenDialog1->FileName + EXT_CRYCON;
    }
  }

  OpenDialog1->FileName = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonOutputClick(TObject *Sender) {
  SaveDialog1->Title = STR_OUTPUT_FILENAME;

  AnsiString path;
  AnsiString name;
  AnsiString ext;

  if (SaveDialog1->Execute()) {
    PathToStrings(SaveDialog1->FileName, path, name, ext);

    Form1->OutputFile->Clear();

    if (Form1->InputFile->Text == SaveDialog1->FileName) {
      if (ext.LowerCase() == EXT_CRYCON) {
        if (name.Length() < 8) {
          Form1->OutputFile->Text = path + EMPTY_FILENAME;
        }
        else {
          Form1->OutputFile->Text = path + name.SetLength(name.Length() - 7);
        }
      }
      else {
        Form1->OutputFile->Text = SaveDialog1->FileName + EXT_CRYCON;
      }
    }
    else {
      Form1->OutputFile->Text = SaveDialog1->FileName;
    }
  }
  
  SaveDialog1->FileName = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonKeyClick(TObject *Sender) {
  OpenDialog1->Title = STR_KEY_FILENAME;

  if (OpenDialog1->Execute()) {
    Form1->MemoKey->Clear();
    Form1->MemoKey->Lines->Text = OpenDialog1->FileName;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Shape1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ReleaseCapture();
  Form1->Perform(WM_SYSCOMMAND, 0xF012, 0);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ReleaseCapture();
  Form1->Perform(WM_SYSCOMMAND, 0xF012, 0);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CipherBoxChange(TObject *Sender) {
  int i, result;

  result = str_list_search(AnsiString(CipherBox->Text).c_str(), ALGORITM_NAME, ALGORITM_NAME_COUNT);

  if (result == -1) {
    return;
  }

  if (result == BLOWFISH) {
    Label3->Visible = False;
    KeyLenBox->Visible = False;
  }
  else {
    AddListKeys((cipher_t)result);

    Label3->Visible = True;
    KeyLenBox->Visible = True;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonKeyGenClick(TObject *Sender) {
  int len;

  len = atoi(AnsiString(PassLen->Text).c_str());

  if ((len < 8) || (len > 256)) {
    MessageForUser(MB_ICONWARNING + MB_OK, STR_WARNING_MSG, STR_ENTER_8_TO_256);
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

  StrongRandomGen(memory->input, len);

  arc4_init(arc4_ctx, memory->input, len);
  arc4(arc4_ctx, memory->input, memory->output, len);

  meminit(memory->input, 0x00, len);

  base64encode(memory->output, memory->input, len);
  memory->input[len] = 0x00;

  MemoKey->Clear();
  MemoKey->Lines->Text = AnsiString((char*)memory->input);
  Application->ProcessMessages();

  meminit(arc4_ctx, 0x00, sizeof(ARC4_CTX));
  meminit(memory, 0x00, sizeof(PASSWORD_MEMORY_CTX));

  free(memory);
  free(arc4_ctx);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label8Click(TObject *Sender)
{
  Form2->Label4->Caption = STR_PROGRAMM_NAME;

  Form2->Shape1->Brush->Color = Form1->Shape1->Brush->Color;
  Form2->Shape1->Pen->Color   = Form1->Shape1->Pen->Color;

  Form2->Shape2->Pen->Color   = Form1->Shape2->Pen->Color;

  Form2->Label4->Color        = Form1->Label1->Color;
  Form2->Label4->Font->Color  = Form1->Label1->Font->Color;
  
  Form2->Label5->Color        = Form1->Label9->Color;
  Form2->Label5->Font->Color  = Form1->Label9->Font->Color;

  Form2->Label2->Caption      = PTCL_LICENSE_INFORMATION;
  Form2->Label3->Caption      = PTCL_AUTHORS_INFORMATION;

  Form2->Show();        
}

TColor InverseColor(const TColor color) {
  return TColor((~(uint32_t)color) & 0x00FFFFFF);
}

void __fastcall TForm1::Label8MouseEnter(TObject *Sender) {
  Label8->Font->Color = InverseColor(Label8->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label8MouseLeave(TObject *Sender) {
  Label8->Font->Color = InverseColor(Label8->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label9MouseEnter(TObject *Sender) {
  Label9->Font->Color = InverseColor(Label9->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label9MouseLeave(TObject *Sender) {
  Label9->Font->Color = InverseColor(Label9->Font->Color);
}
void __fastcall TForm1::FormDestroy(TObject *Sender) {
  DeleteCriticalSection(&CrSec);
  CryptProvDelete(&hcrypt);
}
//---------------------------------------------------------------------------


