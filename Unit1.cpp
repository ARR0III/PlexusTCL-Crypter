#include <io.h>
#include <vcl.h>
#include <windows.h>

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

#include "Unit1.h"
#include "Unit2.h"

#include "LICENSE.h"

#define EMPTY_FILENAME "output.dat"
#define EXT_CRYCON     ".crycon"

#define OK                           0
#define READ_FILE_NOT_OPEN           1
#define WRITE_FILE_NOT_OPEN          2
#define SIZE_FILE_ERROR              3
#define WRITE_FILE_ERROR             4
#define READ_FILE_ERROR              5
#define STREAM_INPUT_CLOSE_ERROR     6
#define STREAM_OUTPUT_CLOSE_ERROR    7
#define ERROR_ALLOCATE_MEMORY        8
#define SIZE_DECRYPT_FILE_INCORRECT  9
#define STOP                        10

#define SIZE_PASSWORD_GENERATE     512
#define BLOCK_SIZE_FOR_ERASED      512

#define LENGTH_DATA_FOR_CHECK     1024

#define ENCRYPT                   0x00
#define DECRYPT                   0xDE

#define DATA_SIZE         (1024*1024*8) /* 8 MiB */

#define MINIMAL(a,b) (((a) < (b)) ? (a) : (b))

#define SET_START_STREAM                 \
  do {                                   \
    EnterCriticalSection(&Form1->CrSec); \
    PROCESSING = true;                   \
    LeaveCriticalSection(&Form1->CrSec); \
  } while(0)

#define SET_STOP_STREAM                  \
  do {                                   \
    EnterCriticalSection(&Form1->CrSec); \
    PROCESSING = false;                  \
    LeaveCriticalSection(&Form1->CrSec); \
  } while(0)

#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

bool PROCESSING = false;

typedef enum cipher_number_enum {
  AES       = 0,
  SERPENT   = 1,
  TWOFISH   = 2,
  BLOWFISH  = 3,
  THREEFISH = 4
} cipher_t;

const char * PARAM_APPEND_BYTE  = "ab";
const char * PARAM_READ_BYTE    = "rb";
const char * PARAM_WRITE_BYTE   = "wb";
const char * PARAM_REWRITE_BYTE = "r+b";

const TColor FORM_HEAD_COLOR = TColor(0x00623E00);

const char * CHAR_KEY_LENGTH_AES[] = {
  "128",
  "192",
  "256"
};

const char * CHAR_KEY_LENGTH_THREEFISH[] ={
  "256",
  "512",
  "1024"
};

const uint32_t INT_SIZE_DATA[] = {
  (uint32_t)1 << 10, /* KiB */
  (uint32_t)1 << 20, /* MeB */
  (uint32_t)1 << 30  /* GiB */
};

const char * CHAR_SIZE_DATA[] = {
  "��",
  "���",
  "���",
  "���"
};

const char * OPERATION_NAME[] = {
  "����������",
  "�����������",
};

const char * ALGORITM_NAME[] = {
  "AES-CFB",
  "SERPENT-CFB",
  "TWOFISH-CFB",
  "BLOWFISH-CFB",
  "THREEFISH-CFB"
};

const char * START_STR = "�����";
const char * STOP_STR  = "����";

const char * PROGRAMM_NAME    = "PlexusTCL Crypter 5.09 08JUN24 [RU]";
const char * MEMORY_BLOCKED   = "������ ��������� ������!";

const char * OK_MSG           = PROGRAMM_NAME;
const char * WARNING_MSG      = "��������!";
const char * ERROR_MSG        = "������!";

const char * INPUT_FILENAME   = "���� ��� ����������";
const char * OUTPUT_FILENAME  = "���� ����������";
const char * KEY_FILENAME     = "���� ����������";

uint32_t      * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
TWOFISH_CTX   * twofish_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

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
  SHA256_CTX * sha256sum;           /* memory for sha256 hash function */
  size_t       sha256sum_length;    /* size struct to pointer ctx->sha256sum */

  uint8_t    * vector;              /* initialized vector for crypt data */
  size_t       vector_length;       /* block size cipher execution */

  uint8_t    * temp_buffer;         /* temp_buffer for temp key data */
  size_t       temp_buffer_length;  /* size buffer for crypt key */

  uint8_t      input  [DATA_SIZE];  /* memory for read */
  uint8_t      output [DATA_SIZE];  /* memory for write */

  int          operation;           /* ENCRYPT == 0x00 or DECRYPT == 0xDE */
  cipher_t     cipher_number;       /* search type name cipher_number_enum */
} GLOBAL_MEMORY;

__fastcall TForm1::TForm1(TComponent* Owner): TForm(Owner) {
}

/* INPUT FILE */
void __fastcall TForm1::Button1Click(TObject *Sender) {
  OpenDialog1->Title = INPUT_FILENAME;

  AnsiString path;
  AnsiString name;
  AnsiString ext;

  if (OpenDialog1->Execute()) {
    path = ExtractFilePath(OpenDialog1->FileName);
    name = ExtractFileName(OpenDialog1->FileName);
    ext  = ExtractFileExt(OpenDialog1->FileName);

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
}

/* OUTPUT FILE */
void __fastcall TForm1::Button2Click(TObject *Sender) {
  SaveDialog1->Title = OUTPUT_FILENAME;

  AnsiString path;
  AnsiString name;
  AnsiString ext;

  if (SaveDialog1->Execute()) {
    path = ExtractFilePath(SaveDialog1->FileName);
    name = ExtractFileName(SaveDialog1->FileName);
    ext  = ExtractFileExt(SaveDialog1->FileName);

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

void __fastcall TForm1::Button3Click(TObject *Sender) {
  OpenDialog1->Title = KEY_FILENAME;

  if (OpenDialog1->Execute()) {
    Form1->Memo1->Clear();
    Form1->Memo1->Lines->Text = OpenDialog1->FileName;
  }
}

void __fastcall TForm1::FormCreate(TObject *Sender) {
  unsigned int trash[2]; /* buffer 8 byte for trash from stack */

  srand((unsigned int)((trash[0] ^ (unsigned int)time(NULL)) + trash[1]));

  SendMessage(ProgressBar1->Handle, PBM_SETBARCOLOR, 0, clGreen);

  InitializeCriticalSection(&CrSec);

  for (int i = 0; i < 5; i++) {
    ComboBox1->Items->Add(ALGORITM_NAME[i]);
  }

  Form1->Caption         = PROGRAMM_NAME;
  Form1->Label6->Caption = PROGRAMM_NAME;

  Form1->Shape1->Pen->Color   = FORM_HEAD_COLOR;
  Form1->Shape2->Brush->Color = FORM_HEAD_COLOR;
  Form1->Shape2->Pen->Color   = FORM_HEAD_COLOR;

  Form1->Label5->Color = FORM_HEAD_COLOR;
  Form1->Label6->Color = FORM_HEAD_COLOR;
  Form1->Label7->Color = FORM_HEAD_COLOR;

  Form1->ProgressBar1->Min = 0;
  Form1->ProgressBar1->Max = 100;
}

void __fastcall TForm1::ComboBox1Change(TObject *Sender) {
  int i;

  if ( (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[AES]))     ||
       (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[SERPENT])) ||
       (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[TWOFISH])) ||
       (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH])) ) {

    ComboBox2->Items->Clear();

    if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH])) {
      for (i = 0; i < 3; i++) {
        ComboBox2->Items->Add(CHAR_KEY_LENGTH_THREEFISH[i]);
      }
    }
    else {
      for (i = 0; i < 3; i++) {
        ComboBox2->Items->Add(CHAR_KEY_LENGTH_AES[i]);
      }
    }

    Label4->Visible = True;
    ComboBox2->Visible = True;
    RadioButton1->Visible = True;
    RadioButton2->Visible = True;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[BLOWFISH])) {
    Label4->Visible = False;
    ComboBox2->Visible = False;
    RadioButton1->Visible = True;
    RadioButton2->Visible = True;
  }
}

void free_global_memory(GLOBAL_MEMORY * ctx, const size_t ctx_length) {
  if (ctx->sha256sum && ctx->sha256sum_length > 0) {
    meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
  }

  if (ctx->vector && ctx->vector_length > 0) {
    meminit(ctx->vector, 0x00, ctx->vector_length);
  }

  if (ctx->temp_buffer && ctx->temp_buffer_length > 0) {
    meminit(ctx->temp_buffer, 0x00, ctx->temp_buffer_length);
  }

  free(ctx->vector);
  free(ctx->sha256sum);
  free(ctx->temp_buffer);

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
  function meminit32 this is analog standart C library memset function
*/
  meminit(&position, 0x00, sizeof(TPoint));
}

void KDFCLOMUL(GLOBAL_MEMORY * ctx,
               const uint8_t * password, const size_t password_len,
                     uint8_t * key,      const size_t key_len) {

  uint32_t i, j, k;
  uint32_t count = 0;

  short real, past = 0;
  float div = (float)(key_len) / 100.0;

  for (i = 1; i <= password_len; ++i) { /* dynamic counter generate */
    count ^= (uint32_t)(CRC32(password, i) + CLOMUL_CONST);
    count -= (password_len + key_len + CLOMUL_CONST + i);
  }

  count  &= CRC32(password, password_len);
  count >>= 18;
  count  |= ((uint32_t)1 << 14);
  count  *= CLOMUL_CONST;

  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
  sha256_init(ctx->sha256sum);

  i = k = 0;

  while (i < key_len) {
    for (j = 0; j < count; ++j) {
      sha256_update(ctx->sha256sum, password, password_len);
    }

    sha256_final(ctx->sha256sum);

    if (SHA256_BLOCK_SIZE == k) {
      k = 0;
    }

    key[i] = ctx->sha256sum->hash[k];

    i++;
    k++;

    real = (short)((float)i / div + 0.1);

    if (real > 100) {
      real = 100;
    }

    if (real > past) {
      Form1->ProgressBar1->Position = real;

      Form1->Label9->Caption = "��������� "
                             + IntToStr(key_len * 8)  + "-������� ����� �� "
                             + IntToStr(password_len) + "-����������� ������: "
                             + IntToStr(real) + " %";
      Application->ProcessMessages();
      past = real;
    }
  }

  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
}

int size_check(uint32_t size) {
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

long int size_of_file(FILE * f) {

  if (fseek(f, 0, SEEK_END) != 0) {
    return (-1);
  }

  long int result = ftell(f);

  if (fseek(f, 0, SEEK_SET) != 0) {
    return (-1);
  }

  return result;
}

float sizetofloatprint(const int status, const float size) {
  return (status ? (size / (float)INT_SIZE_DATA[status - 1]) : size);
}

int erased_head_of_file(const char * filename) {
  size_t counter = 16;
  size_t result  = 0;  
  FILE * f;

  unsigned char * data = (unsigned char *)malloc(BLOCK_SIZE_FOR_ERASED);

  if (!data) {
    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);
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

int erasedfile(const char * filename) {
  FILE * f = fopen(filename, PARAM_REWRITE_BYTE);

  if (!f) {
    return -1;
  }

  long int fsize    = size_of_file(f);
  long int position = 0;

  if (fsize <= 0L) {
    fclose(f);
    return -1;
  }

  uint8_t * data = (uint8_t *)malloc(DATA_SIZE);

  if (!data) {
    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);
    fclose(f);
    return -1;
  }

  int   fsize_check = size_check(fsize);
  float fsize_float = sizetofloatprint(fsize_check, (float)fsize);

  short real_percent;
  short past_percent = 0;

  float div = (float)fsize / 100.0;
  int   check;

  size_t realread;
  size_t size_for_erased ;

  while (position < fsize) {	
    size_for_erased = (fsize - position);

    if (size_for_erased > DATA_SIZE) {
      size_for_erased = DATA_SIZE;
    }

/*****************************************************************************/
    EnterCriticalSection(&Form1->CrSec);
    if (false == PROCESSING) {
      LeaveCriticalSection(&Form1->CrSec);
      meminit(data, 0x00, DATA_SIZE);
      free(data);
      fclose(f);
      return 0xDE;
    }
    LeaveCriticalSection(&Form1->CrSec);
/*****************************************************************************/

    realread = fread(data, 1, size_for_erased, f);
    meminit(data, 0x00, realread);
    
    fseek(f, position, SEEK_SET);
    
/*****************************************************************************/
    EnterCriticalSection(&Form1->CrSec);
    if (false == PROCESSING) {
      LeaveCriticalSection(&Form1->CrSec);
      meminit(data, 0x00, DATA_SIZE);
      free(data);
      fclose(f);
      return 0xDE;
    }
    LeaveCriticalSection(&Form1->CrSec);
/*****************************************************************************/

    if (fwrite(data, 1, realread, f) != realread) {
      fclose(f);
      free(data);

      return -1;
    }
    else {
      fflush(f);
    }

    position += (long int)realread;

    real_percent = (short)((float)position / div + 0.1);

    if (real_percent > 100) {
      real_percent = 100;
    }

    if (real_percent > past_percent) {
      Form1->ProgressBar1->Position = real_percent;

      check = size_check(position);
    
      Form1->Label9->Caption = "����������� �����; ����������: " +
      (check ? FloatToStrF(((float)position / (float)INT_SIZE_DATA[check - 1]), ffFixed, 4, 2) :
               IntToStr(position)) + " " + CHAR_SIZE_DATA[check] + " �� " +
      (check ? FloatToStrF(fsize_float, ffFixed, 4, 2) : IntToStr((int)(fsize_float + 0.1))) +
               " " + CHAR_SIZE_DATA[fsize_check] + "; ��������: " + IntToStr(real_percent) + " %" ;

      Application->ProcessMessages();

      past_percent = real_percent;
    }
  }

  free(data);

  check = chsize(fileno(f), 0);
  fclose(f);

  if (check != 0) {
    return -1;
  }

  return erased_head_of_file(filename);
}

void cipher_free(void * ctx, size_t ctx_length) {
  meminit(ctx, 0x00, ctx_length);
  free(ctx);
}

void hmac_sha256_uf(GLOBAL_MEMORY * ctx) {
	
  size_t hmac_ctx_length = sizeof(HMAC_CTX);	
  HMAC_CTX * hmac_ctx = (HMAC_CTX *)malloc(hmac_ctx_length);

  if (!hmac_ctx) {
    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);
    return;
  }

  int i;
  size_t size_copy_data = MINIMAL(ctx->temp_buffer_length, SHA256_BLOCK_SIZE);

  /* copy hash sum file in local buffer "hash" */
  memcpy(hmac_ctx->hash, ctx->sha256sum->hash, SHA256_BLOCK_SIZE);

  /* generate two secret const for hash update */
  memcpy(hmac_ctx->KEY_0, ctx->temp_buffer, size_copy_data);
  memcpy(hmac_ctx->KEY_1, ctx->temp_buffer, size_copy_data);

  /* if length temp_buffer equal or more SHA256_BLOCK_SIZE then cycle NOT executable */
  for (i = ctx->temp_buffer_length; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] = 0x00;
    hmac_ctx->KEY_1[i] = 0x00;
  }

  for (i = 0; i < SHA256_BLOCK_SIZE; i++) {
    hmac_ctx->KEY_0[i] ^= 0x55; /* simbol 'U', decimal  85, bits 01010101 */
    hmac_ctx->KEY_1[i] ^= 0x66; /* simbol 'f', decimal 102, bits 10101010 */
  }

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

  /* clear  buffers for security */
  meminit(hmac_ctx, 0x00, hmac_ctx_length);
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

int size_correct(GLOBAL_MEMORY * ctx, long int fsize) {
  if (0L == fsize) {
    return SIZE_FILE_ERROR;
  }

  if (ENCRYPT == ctx->operation) {
    if ((long int)(fsize + SHA256_BLOCK_SIZE + ctx->vector_length) <= 0L) {
      return SIZE_FILE_ERROR;
    }
  }
  else {
    /* if fsize < minimal size for decrypt */
    if (fsize < (long int)(SHA256_BLOCK_SIZE + ctx->vector_length + 1)) {
      return SIZE_DECRYPT_FILE_INCORRECT;
    }
  }

  return OK;
}

int filecrypt(GLOBAL_MEMORY * ctx) {
  register long int fsize;
  register long int position = 0;
	
  float div, fsize_float;

  int check, fsize_check;

  size_t nblock;
  size_t realread;

  short real_percent;
  short past_percent = 0;

  FILE * fi = fopen(Form1->Edit1->Text.c_str(), PARAM_READ_BYTE);

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

  FILE * fo = fopen(Form1->Edit2->Text.c_str(), PARAM_WRITE_BYTE);

  if (!fo) {
    if (fclose(fi) == -1)
      return STREAM_INPUT_CLOSE_ERROR;
    else
      return WRITE_FILE_NOT_OPEN;
  }

  div = (float)fsize / 100.0;
  
  fsize_check = size_check(fsize);
  fsize_float = sizetofloatprint(fsize_check, (float)fsize);

  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
  sha256_init(ctx->sha256sum);

  while (position < fsize) {  
/*****************************************************************************/
    EnterCriticalSection(&Form1->CrSec);
    if (false == PROCESSING) {
      LeaveCriticalSection(&Form1->CrSec);
      meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
      return close_in_out_files(fi, fo, 0xDE);
    }
    LeaveCriticalSection(&Form1->CrSec);
/*****************************************************************************/
    if (0 == position) {
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
            memcpy(ctx->output, ctx->vector, ctx->vector_length);
            blowfish_encrypt(blowfish_ctx, (uint32_t *)ctx->output, (uint32_t *)(ctx->output + 4));
            break;
          case THREEFISH:
            threefish_encrypt(threefish_ctx, (uint64_t*)ctx->vector, (uint64_t*)ctx->output);
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
        position += (int32_t)ctx->vector_length;
      }
    }
/*****************************************************************************/
    EnterCriticalSection(&Form1->CrSec);
    if (false == PROCESSING) {
      LeaveCriticalSection(&Form1->CrSec);
      meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
      return close_in_out_files(fi, fo, 0xDE);
    }
    LeaveCriticalSection(&Form1->CrSec);
/*****************************************************************************/
    realread = fread(ctx->input, 1, DATA_SIZE, fi);

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
/*****************************************************************************/
    EnterCriticalSection(&Form1->CrSec);
    if (false == PROCESSING) {
      LeaveCriticalSection(&Form1->CrSec);
      meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);
      return close_in_out_files(fi, fo, 0xDE);
    }
    LeaveCriticalSection(&Form1->CrSec);
/*****************************************************************************/
    if (fwrite(ctx->output, 1, realread, fo) != realread) {
      return close_in_out_files(fi, fo, WRITE_FILE_ERROR);
    }

    fflush(fo);

    if (real_percent > past_percent) {
      Form1->ProgressBar1->Position = real_percent;

      check = size_check(position);

      Form1->Label9->Caption = AnsiString(OPERATION_NAME[ctx->operation ? 1 : 0 ]) +
      ": " + AnsiString(ALGORITM_NAME[ctx->cipher_number]) + "; ����������: " +
      (check ? FloatToStrF(((float)position / (float)(INT_SIZE_DATA[check - 1])), ffFixed, 4, 2) :
               IntToStr(position)) + " " + CHAR_SIZE_DATA[check] + " �� " +
      (check ? FloatToStrF(fsize_float, ffFixed, 4, 2) : IntToStr((int)(fsize_float + 0.1))) + " " + CHAR_SIZE_DATA[fsize_check] + "; ��������: " + IntToStr(real_percent) + " %" ;

      Application->ProcessMessages();
      past_percent = real_percent;
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

      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "����������� ����� ����� �� ��������� � ���������!\n"
                     "�������� ���� ��������� ��� ��� ����������� ������������ ����!");
    }
  }

  meminit(ctx->sha256sum, 0x00, ctx->sha256sum_length);

  return close_in_out_files(fi, fo, OK);
}

void random_vector_init(uint8_t * data, size_t size) {
  
  size_t i;
  size_t arc4_size   = sizeof(ARC4_CTX);
  size_t vector_size = size;
  
  ARC4_CTX * arc4_memory = (ARC4_CTX *)malloc(arc4_size);
  uint8_t * vector_memory = (uint8_t *)malloc(vector_size);
  
  if (!arc4_memory || !vector_memory) {
    free(vector_memory);
    free(arc4_memory);
	
    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);	
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
}

size_t vector_init(uint8_t * data, size_t size) {
	
  size_t i;
  size_t stack_trash[2]; /* NOT initialized == ALL OK */
  
  for (i = 0; i < size; i++) {
    data[i] = (uint8_t)i ^ (uint8_t)genrand(0x00, 0xFF);
  }

  /* random data from stack xor initialized vector */
  (*(uint32_t *)data) ^= (uint32_t)stack_trash[0] +
                         (uint32_t)genrand(0x00000000, 0xFFFFFFFF) ^
                         (uint32_t)stack_trash[1];

  cursorpos(data); // X and Y cursor position xor operation for data[0] and data[1];

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

char * CharA_Or_CharOV(size_t length) {
  return (24 == length || 128 == length) ? "�" : "��";
}

int GeneratingCryptKey(const char * message) {
  if (MessageForUser(MB_ICONINFORMATION + MB_YESNO, OK_MSG, message) == IDNO) {
    return IDNO;
  }
  
  return IDYES;
}


void __fastcall TForm1::Button4Click(TObject *Sender) {
/* �� ���� ��������� ������ �����, ��� ����������� ������ ������� �������������
   �� ����� C++, ������ ��� � ����� C ����������� ����� ���� ������ */

  String UnicodeMsg = "�������� ��������?";

  /* if stream create and execute then blocking stream and set PROCESSING in false
     stream to check PROCESSING and if PROCESSING == false, stream killself*/
  EnterCriticalSection(&Form1->CrSec);
  if (PROCESSING) {
    if (MessageForUser(MB_ICONQUESTION + MB_YESNO, OK_MSG, UnicodeMsg.c_str()) == IDYES) {
      /* �� ����� ������� � ������ MessageForUser �������� ����� ���������� */
      PROCESSING = false;

      Button4->Caption = START_STR;
      Form1->ProgressBar1->Position = 0;
      Form1->ProgressBar1->Update();
    }

    LeaveCriticalSection(&Form1->CrSec);

    return;
  }
  LeaveCriticalSection(&Form1->CrSec);

  UnicodeMsg = "";

  if (x_strnlen(Edit1->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "��� ��������������� ����� �� �������!");
    return;
  }

  if (x_strnlen(Edit2->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "��� ����� ���������� �� �������!");
    return;
  }

  if (x_strnlen(Memo1->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "������ ��� ��� ��������� ����� �� �������!");
    return;
  }

  if (Edit1->Text == Edit2->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "����� ��������������� ����� � ����� ���������� ���������!");
    return;
  }

  if (Edit1->Text == Memo1->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "����� ��������������� ����� � ��������� ����� ���������!");
    return;
  }

  if (Edit2->Text == Memo1->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "����� ����� ���������� � ��������� ����� ���������!");
    return;
  }

  size_t memory_length = sizeof(GLOBAL_MEMORY);
  GLOBAL_MEMORY * memory = (GLOBAL_MEMORY *)malloc(memory_length);

  if (!memory) {
    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);
    return;
  }

  meminit(memory, 0x00, memory_length);

  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[AES])) {
    memory->cipher_number = AES;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[SERPENT])) {
    memory->cipher_number = SERPENT;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[TWOFISH])) {
    memory->cipher_number = TWOFISH;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[BLOWFISH])) {
    memory->cipher_number = BLOWFISH;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH])) {
    memory->cipher_number = THREEFISH;
  }
  else {
    free_global_memory(memory, memory_length);

    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "�������� ���������� �� ��� ������!");
    return;
  }

  extern int AES_Rounds; /* in rijndael.c source code file */

  memory->temp_buffer_length = 0;

  if (memory->cipher_number == AES ||
      memory->cipher_number == SERPENT ||
      memory->cipher_number == TWOFISH) {
    if (AnsiString(ComboBox2->Text) == AnsiString("128")) {
      if (memory->cipher_number == AES) {
        AES_Rounds = 10;
      }
      memory->temp_buffer_length = 128;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("192")) {
      if (memory->cipher_number == AES) {
        AES_Rounds = 12;
      }
      memory->temp_buffer_length = 192;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("256")) {
      if (memory->cipher_number == AES) {
        AES_Rounds = 14;
      }
      memory->temp_buffer_length = 256;
    }
    else {
      free_global_memory(memory, memory_length);
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "����� ����� ���������� �� ���� �������!");
      return;
    }
  }
  else
  if (memory->cipher_number == BLOWFISH) {
    memory->temp_buffer_length = 448;
  }
  else
  if (memory->cipher_number == THREEFISH) {
    if (AnsiString(ComboBox2->Text) == AnsiString("256")) {
      memory->temp_buffer_length = 256;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("512")) {
      memory->temp_buffer_length = 512;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("1024")) {
      memory->temp_buffer_length = 1024;
    }
    else {
      free_global_memory(memory, memory_length);
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "����� ����� ���������� �� ���� �������!");
      return;
    }
  }

  memory->temp_buffer_length = (memory->temp_buffer_length / 8);

  /*
    AES       = (temp_buffer_length = 16 or 24 or 32;
    TWOFISH   = (temp_buffer_length = 16 or 24 or 32);
    SERPENT   = (temp_buffer_length = 16 or 24 or 32);
    BLOWFISH  = (temp_buffer_length = 56);
    THREEFISH = (temp_buffer_length = 32 or 64 or 128);
  */

  memory->operation = ENCRYPT; /* fusking warnings c++ builder !*/

  if (RadioButton1->Checked) {
    memory->operation = ENCRYPT;
  }
  else
  if (RadioButton2->Checked) {
    memory->operation = DECRYPT;
  }
  else {
    free_global_memory(memory, memory_length);
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "�������� �� ���� �������!");
    return;
  }

  if (FileExists(Edit2->Text) == True) {
    UnicodeMsg = "���� ���������� ����������! ������ ������ ����� �������!\n"
                 "�� ������� ��� ������ ������������ ���?";

    if (MessageForUser(MB_ICONWARNING + MB_YESNO, WARNING_MSG, UnicodeMsg.c_str()) == IDNO) {
      free_global_memory(memory, memory_length);

      MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                     "�������� ��� ����� ����������!");
      UnicodeMsg = "";
      return;
    }

    UnicodeMsg = "";
  }

  UnicodeMsg = "";

  memory->temp_buffer = (uint8_t*)malloc(memory->temp_buffer_length);

  if (!memory->temp_buffer) {
    free_global_memory(memory, memory_length);

    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

    return;
  }

  memory->sha256sum_length = sizeof(SHA256_CTX);
  memory->sha256sum = (SHA256_CTX *)malloc(memory->sha256sum_length);

  if (!memory->sha256sum) {
    free_global_memory(memory, memory_length);

    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

    return;
  }
  
  int real_read = readfromfile(Memo1->Text.c_str(), memory->temp_buffer, memory->temp_buffer_length);

  if (real_read == (int)(memory->temp_buffer_length)) {
    UnicodeMsg = "������������ " + IntToStr(memory->temp_buffer_length * 8) + "-������ ���� ���������� �� �����?\n"; 
	
    if (GeneratingCryptKey(UnicodeMsg.c_str()) == IDNO) {
      free_global_memory(memory, memory_length);
      UnicodeMsg = "";
      return;
    }
    /*
    if (DoubleSecurityWitchPassword() == true) {
      GeneratingKeyWitchPassword(memory->temp_buffer, memory->temp_buffer_length, password);
    }
    */
  }
  else
  if ((real_read > 0) && (real_read < (int)(memory->temp_buffer_length))) {
    free_global_memory(memory, memory_length);

    UnicodeMsg = "������������ ������ � �������� �����!\n\n"
                 "���� �������:\t" + IntToStr(real_read) + " ��\n"
                 "����������:\t" + IntToStr(memory->temp_buffer_length) + " ��";

    Application->MessageBox(UnicodeMsg.c_str(), WARNING_MSG, MB_ICONWARNING + MB_OK);

    UnicodeMsg = "";

    return;
  }
  else
  if ((real_read == 0) || (real_read == -1)) {	
    real_read = (int)x_strnlen(Memo1->Text.c_str(), 256);

    if ((real_read > 7) && (real_read < 257)) {
      UnicodeMsg = "������������� " + IntToStr(memory->temp_buffer_length * 8) + "-������ ���� ���������� �� ������?\n";
	
      if (GeneratingCryptKey(UnicodeMsg.c_str()) == IDNO) {
        free_global_memory(memory, memory_length);
        UnicodeMsg = "";
        return;
      }

      Button4->Enabled = False;

      Form1->ProgressBar1->Position = 0;
      Form1->ProgressBar1->Update();

      /* Crypt key generator; generate crypt key from password */
      KDFCLOMUL(memory, (uint8_t *)Memo1->Text.c_str(), real_read,
                memory->temp_buffer, memory->temp_buffer_length);

      Button4->Enabled = True;
    }
    else {
      free_global_memory(memory, memory_length);

      UnicodeMsg = "����� ���������� ����� �����������!\n\n"
                   "���� �������:\t" + IntToStr(real_read) + " ��\n"
                   "����������:\t�� 8 �� 256 ��";

      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG, UnicodeMsg.c_str());

      UnicodeMsg = "";

      return;
    }
  }

  UnicodeMsg = "";

  switch (memory->cipher_number) {
    case AES:
      memory->vector_length = 16;
      break;
    case SERPENT:
      memory->vector_length = 16;
      break;
    case TWOFISH:
      memory->vector_length = 16;
      break;
    case BLOWFISH:
      memory->vector_length =  8;
      break;
    case THREEFISH:
      memory->vector_length = memory->temp_buffer_length;
      break;
  }

  memory->vector = (uint8_t*)malloc(memory->vector_length);

  if (!memory->vector) {
    free_global_memory(memory, memory_length);

    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

    return;
  }

  meminit(memory->vector, 0x00, memory->vector_length);

  if (ENCRYPT == memory->operation) {
    if (vector_init(memory->vector, memory->vector_length) < (memory->vector_length - 2)) {
      free_global_memory(memory, memory_length);

      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "����������� ������ ����! ���������� �������� �� ���������!"
                     "\n��������� ����� �����������? ��������� ��������� ����!");

      Form1->Close();
    }
  }

  size_t cipher_length  = 0;
  void * cipher_pointer = NULL;

  if (AES == memory->cipher_number) {
    cipher_length  = 4 * (AES_Rounds + 1) * 4;
    rijndael_ctx   = (uint32_t *)calloc(cipher_length, 1);
    cipher_pointer = (void *)rijndael_ctx;

    if (!rijndael_ctx) {
      free_global_memory(memory, memory_length);

      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      return;
    }

    AES_Rounds =
      rijndael_key_encrypt_init(rijndael_ctx, memory->temp_buffer, memory->temp_buffer_length * 8);
  }
  if (memory->cipher_number == TWOFISH) {
    cipher_length  = sizeof(TWOFISH_CTX);
    twofish_ctx    = (TWOFISH_CTX *)calloc(1, cipher_length);
    cipher_pointer = (void *)twofish_ctx;

    if (!twofish_ctx) {
      free_global_memory(memory, memory_length);

      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      return;
    }
    twofish_init(twofish_ctx, memory->temp_buffer, memory->temp_buffer_length);
  }
  else
  if (memory->cipher_number == SERPENT) {
    cipher_length = sizeof(SERPENT_CTX);
    serpent_ctx = (SERPENT_CTX *) calloc(1, cipher_length);
    cipher_pointer = (void *)serpent_ctx;

    if (!serpent_ctx) {
      free_global_memory(memory, memory_length);

      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      return;
    }
    serpent_init(serpent_ctx, memory->temp_buffer_length * 8, memory->temp_buffer);
  }
  else
  if (memory->cipher_number == BLOWFISH) {
    cipher_length = sizeof(BLOWFISH_CTX);
    blowfish_ctx = (BLOWFISH_CTX*)calloc(1, cipher_length);
    cipher_pointer = (void *)blowfish_ctx;

    if (!blowfish_ctx) {
      free_global_memory(memory, memory_length);

      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      return;
    }
    blowfish_init(blowfish_ctx, memory->temp_buffer, memory->temp_buffer_length);
  }
  else
  if (memory->cipher_number == THREEFISH) {
    cipher_length = sizeof(THREEFISH_CTX);
    threefish_ctx = (THREEFISH_CTX *)calloc(1, cipher_length);
    cipher_pointer = (void *)threefish_ctx;

    if (!threefish_ctx) {
      free_global_memory(memory, memory_length);

      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      return;
    }
    threefish_init(threefish_ctx, (threefishkeysize_t)(memory->temp_buffer_length * 8),
                   (uint64_t *)memory->temp_buffer, (uint64_t *)memory->temp_buffer);
  }

  int result = 0xDE;

  UnicodeMsg =
    "���������� � ��������� ���� ��������?\n\n"
    "��������:\t" + String(OPERATION_NAME[memory->operation ? 1 : 0]) + "\n"
    "��������:\t" + String(ALGORITM_NAME[memory->cipher_number]) + "\n"
    "����� �����:\t" + IntToStr(memory->temp_buffer_length * 8).c_str() + " ���" +
                       CharA_Or_CharOV(memory->temp_buffer_length);

  if (MessageForUser(MB_ICONQUESTION + MB_YESNO, OK_MSG, UnicodeMsg.c_str()) == IDYES) {

    Button4->Caption = STOP_STR;

    Form1->ProgressBar1->Position = 0;
    Form1->ProgressBar1->Update();

/*****************************************************************************/
    SET_START_STREAM;
      result = filecrypt(memory);
    SET_STOP_STREAM;
/*****************************************************************************/

    Button4->Caption = START_STR;
  }

  UnicodeMsg = "";

  switch (result) {
    case  0xDE:
      MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                     "�������� ���� ��������!");
      break;
    case OK:
      MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                     "���� ������� ���������!");
      break;
    case READ_FILE_NOT_OPEN:
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "���� ��� ��������� �� ��� ������!");
      break;
    case WRITE_FILE_NOT_OPEN:
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "���� ���������� �� ��� ������!");
      break;
    case SIZE_FILE_ERROR:
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "���� ��� ��������� ���� ��� ��� ������"
                     " ��������� 2 ���!");
      break;
    case WRITE_FILE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "������ ������ � ����!");
      break;
    case READ_FILE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "������ ������ �� �����!");
      break;
    case STREAM_INPUT_CLOSE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "����� ����� �� ��� ������!");
      break;
    case STREAM_OUTPUT_CLOSE_ERROR:
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "����� ������ �� ��� ������!");
      break;
    case SIZE_DECRYPT_FILE_INCORRECT:
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "������ ����� ��� ����������� �����������!\n"
                     "�������������� ���� ����� ��� ����������?");
  }

  if ((result == OK) && (CheckBox1->Checked == True)) {
    UnicodeMsg = "�� ������� ��� ������ ���������� ���� ��� ���������?\n"
                 "������� ������ ����� ���������� ������������!";

    if (MessageForUser(MB_ICONWARNING + MB_YESNO, WARNING_MSG,
                       UnicodeMsg.c_str()) == IDYES) {

      Button4->Caption = STOP_STR;               

      Form1->ProgressBar1->Position = 0;
      Form1->ProgressBar1->Update();

      UnicodeMsg = "";

/*****************************************************************************/
      SET_START_STREAM;
        result = erasedfile(Edit1->Text.c_str());
      SET_STOP_STREAM;
/*****************************************************************************/

      if (result == 0) {
        if (DeleteFile(Edit1->Text) != False) {
          MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                        "���� ���������� ��� ���������!");
        }
        else {
          /* if file rewriting and not delete -> show error code for user */
          unsigned long error_delete = GetLastError();

          UnicodeMsg = "������ �������� �����!\n"
                       "����:\n"
                       + Form1->Edit1->Text + "\n\n"
                       "��� ����������� �� �� ��� ������ � �����!\n"
                       "��� ������: " + IntToStr(error_delete);

          MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, UnicodeMsg.c_str());
        }
      }
      else
      if (result == 0xDE) {
        MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,"�������� ���� ��������!");
      }
      else {
        MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                      "������ ���������� �����!\n"
                      "�������� ���� �� ���������� ��� ������� �� ������!");
      }
    }
  }


  UnicodeMsg = "";

  cipher_free(cipher_pointer, cipher_length);
  free_global_memory(memory, memory_length);

  Button4->Caption = START_STR; 

  Form1->ProgressBar1->Position = 0;
  Form1->ProgressBar1->Update();

  Application->ProcessMessages();
}

void __fastcall TForm1::Label5Click(TObject *Sender) {
  DeleteCriticalSection(&CrSec);

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
  Form2->Label4->Caption = PROGRAMM_NAME;

  Form2->Shape1->Brush->Color = FORM_HEAD_COLOR;
  Form2->Shape1->Pen->Color   = FORM_HEAD_COLOR;

  Form2->Shape2->Pen->Color   = FORM_HEAD_COLOR;

  Form2->Label4->Color = FORM_HEAD_COLOR;
  Form2->Label5->Color = FORM_HEAD_COLOR;

  Form2->Label2->Caption = PTCL_LICENSE_INFORMATION;
  Form2->Label3->Caption = PTCL_AUTHORS_INFORMATION;

  Form2->Show();
}

void __fastcall TForm1::Button5Click(TObject *Sender) {
  int len = atoi(Edit3->Text.c_str());

  if ((len < 8) || (len > 256)) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "������� ����� �� 8 �� 256!");
    return;
  }

  size_t cipher_len              = sizeof(ARC4_CTX);
  size_t password_memory_ctx_len = sizeof(PASSWORD_MEMORY_CTX);

  /* allocate memory and NOT clear memory */
  PASSWORD_MEMORY_CTX * memory   = (PASSWORD_MEMORY_CTX *)malloc(password_memory_ctx_len);
  ARC4_CTX * arc4_ctx = (ARC4_CTX *)malloc(cipher_len);

  if (!memory || !arc4_ctx) {
    free(memory);
    free(arc4_ctx);

    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);
    return;
  }

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

  meminit(arc4_ctx, 0x00, cipher_len);
  meminit(memory, 0x00, password_memory_ctx_len);

  free(memory);
  free(arc4_ctx);
}

void __fastcall TForm1::Shape2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y) {
  ReleaseCapture();
  Form1->Perform(WM_SYSCOMMAND, 0xF012, 0);
}
void __fastcall TForm1::Label5MouseEnter(TObject *Sender)
{
  Label5->Font->Color = clRed;      
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label5MouseLeave(TObject *Sender)
{
  Label5->Font->Color = clWhite;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label7MouseLeave(TObject *Sender)
{
  Label7->Font->Color = clWhite;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Label7MouseEnter(TObject *Sender)
{
  Label7->Font->Color = clRed;        
}
//---------------------------------------------------------------------------






void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  DeleteCriticalSection(&CrSec);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
  DeleteCriticalSection(&CrSec);        
}
//---------------------------------------------------------------------------

