#include <io.h>
#include <vcl.h>
#include <time.h>
#include <windows.h>

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

#include "src/base64.h"

#include "src/xtalw.h"
#include "src/clomul.h"

#include "Unit1.h"
#include "Unit2.h"

#define ENCRYPT             0x00
#define DECRYPT             0xDE
                  
#define PERIOD                 4
#define SHA_256_HASH_LENGTH   32

#define BLOCK_SIZE          1024
#define DATA_SIZE           4096

#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

enum {
  ARC4      = 0,
  AES       = 1,
  SERPENT   = 2,
  TWOFISH   = 3,
  BLOWFISH  = 4,
  THREEFISH = 5
};

const float cas = 4.87; /* ((float)Form1->Shape4->Width / (float)100) or (488 ??? / 100) */

const char * PARAM_READ_BYTE    = "rb";
const char * PARAM_WRITE_BYTE   = "wb";
const char * PARAM_REWRITE_BYTE = "r+b";

const TColor FORM_HEAD_COLOR = TColor(0x00623E00);

const char * CHAR_KEY_LENGTH_AES[] ={
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
  1024,
  1048576,
  1073741824
};

const char * CHAR_SIZE_DATA[] = {
  "Áò",
  "ÊèÁ",
  "ÌåÁ",
  "ÃèÁ"
};

const char * OPERATION_NAME[] = {
  "Øèôðîâàíèå",
  "Ðàñøèôðîâêà",
  "Ïîòîêîâàÿ îáðàáîòêà"
};

const char * ALGORITM_NAME[] = {
  "ARC4",
  "AES-CFB",
  "SERPENT-CFB",
  "TWOFISH-CFB",
  "BLOWFISH-CFB",
  "THREEFISH-CFB"
};

const char * PROGRAMM_NAME   = "PlexusTCL Crypter 4.92 10MAY21 [RU]";

const char * MEMORY_BLOCKED  = "Îøèáêà âûäåëåíèÿ ïàìÿòè!";

const char * OK_MSG          = PROGRAMM_NAME;
const char * WARNING_MSG     = "Âíèìàíèå!";
const char * ERROR_MSG       = "!!! Îøèáêà !!!";

const char * INPUT_FILENAME  = "Ôàéë äëÿ øèôðîâàíèÿ";
const char * OUTPUT_FILENAME = "Ôàéë íàçíà÷åíèÿ";
const char * KEY_FILENAME    = "Êëþ÷ øèôðîâàíèÿ";

ARC4_CTX      * arc4_ctx      = NULL;
uint32_t      * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
TWOFISH_CTX   * twofish_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

typedef struct {
  uint8_t input [DATA_SIZE];
  uint8_t output[DATA_SIZE];
} MEMORY_CTX;

__fastcall TForm1::TForm1(TComponent* Owner): TForm(Owner) {
}

void __fastcall TForm1::Button1Click(TObject *Sender) {
  OpenDialog1->Title = INPUT_FILENAME;
  if (OpenDialog1->Execute()) {
    Edit1->Clear();
    Edit1->Text = OpenDialog1->FileName;

    if ((strlen(Form1->Edit2->Text.c_str()) > 0) &&
        (Form1->Edit1->Text == Form1->Edit2->Text)) {

      Form1->Edit2->Text = Form1->Edit2->Text + ".crycon";
    }
  }
}

void __fastcall TForm1::Button2Click(TObject *Sender) {
  SaveDialog1->Title = OUTPUT_FILENAME;
  if (SaveDialog1->Execute()) {
    Form1->Edit2->Clear();
    Form1->Edit2->Text = SaveDialog1->FileName;

    if ((strlen(Form1->Edit1->Text.c_str()) > 0) &&
        (Form1->Edit1->Text == Form1->Edit2->Text)) {

      Form1->Edit2->Text = Form1->Edit2->Text + ".crycon";
    }
  }
}

void __fastcall TForm1::Button3Click(TObject *Sender) {
  OpenDialog1->Title = KEY_FILENAME;
  if (OpenDialog1->Execute()) {
    Form1->Memo1->Clear();
    Form1->Memo1->Lines->Text = OpenDialog1->FileName;
  }
}

void __fastcall TForm1::FormCreate(TObject *Sender) {

  srand((unsigned int)time(NULL));

  for (char i = 0; i < 6; i++) {
    ComboBox1->Items->Add(ALGORITM_NAME[i]);
  }

  Form1->Caption = PROGRAMM_NAME;

  Form1->Label6->Caption = PROGRAMM_NAME;

  Form1->Shape1->Pen->Color   = FORM_HEAD_COLOR;

  Form1->Shape2->Brush->Color = FORM_HEAD_COLOR;
  Form1->Shape2->Pen->Color   = FORM_HEAD_COLOR;

  Form1->Label5->Color = FORM_HEAD_COLOR;
  Form1->Label6->Color = FORM_HEAD_COLOR;
  Form1->Label7->Color = FORM_HEAD_COLOR;
}

void __fastcall TForm1::ComboBox1Change(TObject *Sender) {
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[ARC4])) {
    Label4->Visible = False;
    ComboBox2->Visible = False;
    RadioButton1->Visible = False;
    RadioButton2->Visible = False;
  }
  else
  if ( (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[AES]))    ||
       (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[SERPENT])) ||
       (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[TWOFISH])) ||
       (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH])) ) {

    ComboBox2->Items->Clear();

    if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH])) {
      for (char i = 0; i < 3; i++) {
        ComboBox2->Items->Add(CHAR_KEY_LENGTH_THREEFISH[i]);
      }
    }
    else {
      for (char i = 0; i < 3; i++) {
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

void cursorpos(uint8_t * data) {
  TPoint position; /* position cursor write in initialization vector */

  GetCursorPos(&position);

  data[0] ^= (uint8_t)(position.x);
  data[1] ^= (uint8_t)(position.y);
/*
  position->x = 0;
  position->y = 0;
  function meminit this is analog system memset function!
*/
  meminit((void *)&position, 0x00, sizeof(TPoint));
}

void centreal(short * real_percent) {
  if (*real_percent > 100) {
    *real_percent = 100;
  }
}

void * KDFCLOMUL(SHA256_CTX * sha256_ctx,
              const uint8_t * password, const size_t password_len,
                    uint8_t * key,      const size_t key_len) {

  uint32_t i, j, k;
  uint32_t count = 0;
  uint8_t  hash[SHA256_BLOCK_SIZE];

  short real = 0;
  short past = 0;

  float div = (float)(key_len) / 100.0;

  for (i = 0; i < password_len; ++i) {  /* dynamic generate count */
    count ^= (uint32_t)(CRC32(password, i) + CLOMUL_CONST);
    count -= (password_len + key_len + CLOMUL_CONST + i);
  }

  count  &= CRC32(password, password_len);;
  count >>= 18;
  count  |= ((uint32_t)1 << 14);
  count  *= CLOMUL_CONST;

  sha256_init(sha256_ctx);

  i = k = 0;

  while (i < key_len) {
    for (j = 0; j < count; ++j) {
      sha256_update(sha256_ctx, password, password_len);
    }

    sha256_final(sha256_ctx, hash);

    if (k == SHA256_BLOCK_SIZE) {
      k = 0;
    }

    key[i] = hash[k];

    ++i;
    ++k;

    real = (short)((float)i / div + 0.1);

    centreal(&real);

    if (real > past) {
      if ((real % PERIOD) == 0) {
        Form1->Shape4->Width   = (int)((float)real * cas) + 1;
        Form1->Label9->Caption = "Ãåíåðàöèÿ "
                               + IntToStr(key_len * 8)  + "-áèòíîãî êëþ÷à èç "
                               + IntToStr(password_len) + "-ñèìâîëüíîãî ïàðîëÿ: "
                               + IntToStr(real) + " %";
        Application->ProcessMessages();
      }
      past = real;
    }
  }

  meminit((void *)hash, 0x00, SHA256_BLOCK_SIZE);

  return key;
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

int erasedfile(const char * filename) {
  FILE * f = fopen(filename, PARAM_REWRITE_BYTE);

  if (f == NULL) {
    return -1;
  }

  long int fsize    = size_of_file(f);
  long int position = 0;

  if ((fsize == -1L) || (fsize == 0)) {
    fclose(f);
    return -1;
  }

  uint8_t * data = (uint8_t *)calloc(BLOCK_SIZE, 1);

  if (data == NULL) {
    fclose(f);
    return -1;
  }

  int   fsize_check = size_check(fsize);
  float fsize_float = sizetofloatprint(fsize_check, (float)fsize);

  short real_percent = 0;
  short past_percent = 0;

  float div = (float)fsize / 100.0;
  int   check;

  size_t realread;

  while (position < fsize) {
    realread = fread((void *)data, 1, BLOCK_SIZE, f);

    if (realread > 0) {
      meminit((void *)data, 0x00, realread);
      fseek(f, position, SEEK_SET);

      if (fwrite((void *)data, 1, realread, f) != realread) {
        fclose(f);
        free((void *)data);
        data = NULL;
        return -1;
      }
      else {
        fflush(f);
      }

      position += (long int)realread;
    }

    real_percent = (short)((float)position / div + 0.1);

    centreal(&real_percent);

    if (real_percent > past_percent) {
      if ((real_percent % PERIOD) == 0) {
        Form1->Shape4->Width = (int)((float)real_percent * cas) + 1;
        check = size_check(position);

        Form1->Label9->Caption = "Óíè÷òîæåíèå ôàéëà; Îáðàáîòàíî: " +
        (check ? FloatToStrF(((float)position / (float)INT_SIZE_DATA[check - 1]), ffFixed, 4, 2) :
                 IntToStr(position)) + " " + CHAR_SIZE_DATA[check] + " èç " +
        (check ? FloatToStrF(fsize_float, ffFixed, 4, 2) : IntToStr((int)(fsize_float + 0.1))) +
                 " " + CHAR_SIZE_DATA[fsize_check] + "; Ïðîãðåññ: " + IntToStr(real_percent) + " %" ;

        Application->ProcessMessages();
      }
      past_percent = real_percent;
    }
  }

  free((void *)data);
  data = NULL;

  check = chsize(fileno(f), 0);
  fclose(f);

  if (check != 0) {
    return -1;
  }
  else {
    return 0;
  }
}

int filecrypt(const char * finput, const char * foutput, uint8_t * vector,
              const size_t block_size, const int cipher, const int operation) {

  FILE * fi = fopen(finput, PARAM_READ_BYTE);

  if (fi == NULL) {
    return -1;
  }

  long int fsize = size_of_file(fi);
  long int position = 0;

  if ((fsize == -1L) || (fsize == 0)) {
    fclose(fi);
    return -3;
  }

  FILE * fo = fopen(foutput, PARAM_WRITE_BYTE);

  if (fo == NULL) {
    fclose(fi);
    return -2;
  }

  size_t memory_ctx_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = (MEMORY_CTX *) calloc(1, memory_ctx_len);

  if (memory == NULL) {
    fclose(fi);
    fclose(fo);
    return -4;
  }

  float div = (float)fsize / 100.0;

  int check;

  size_t nblock;
  size_t realread;

  short real_percent = 0;
  short past_percent = 0;

  int   fsize_check = size_check(fsize);
  float fsize_float = sizetofloatprint(fsize_check, (float)fsize);

  Form1->Shape4->Width = 0;

  while (position < fsize) {
    if (cipher != ARC4 && position == 0) {
      switch (operation) {
        case ENCRYPT:
	  switch (cipher) {
            case AES:
	      rijndael_encrypt(rijndael_ctx, vector, memory->output);
              break;
            case SERPENT:
	      serpent_encrypt(serpent_ctx, (uint32_t *)vector, (uint32_t *)memory->output);
              break;
	    case TWOFISH:
	      twofish_encrypt(twofish_ctx, vector, memory->output);
	      break;
            case BLOWFISH:
              memmove(memory->output, vector, block_size);
              blowfish_encrypt(blowfish_ctx, (uint32_t *)memory->output, (uint32_t *)(memory->output + 4));
              break;
             case THREEFISH:
               threefish_encrypt(threefish_ctx, (uint64_t*)vector, (uint64_t*)memory->output);
               break;
	   }
		  
          memmove(vector, memory->output, block_size);
          
	  if (fwrite((void *)vector, 1, block_size, fo) != block_size) {
            fclose(fi);
            fclose(fo);

            free((void *)memory);
            memory = NULL;

            return -5;
          }
          else {
            fflush(fo);
          }
          break;
		   
        case DECRYPT:
	  if (fread((void *)vector, 1, block_size, fi) != block_size) {
            fclose(fi);
            fclose(fo);

            free((void *)memory);
            memory = NULL;

            return -6;
          }
          position += (long int)block_size;
          break;
      }
    }

    realread = fread((void *)memory->input, 1, DATA_SIZE, fi);

    if (cipher == ARC4) {
      arc4(arc4_ctx, memory->input, memory->output, realread);
    }
    else {
      for (nblock = 0; nblock < realread; nblock += block_size) {
        switch (cipher) {
          case AES:
	    rijndael_encrypt(rijndael_ctx, vector, memory->output + nblock);
            break;
          case SERPENT:
	    serpent_encrypt(serpent_ctx, (uint32_t *)vector, (uint32_t *)(memory->output + nblock));
            break;
          case TWOFISH:
	    twofish_encrypt(twofish_ctx, vector, memory->output + nblock);
	    break;
          case BLOWFISH:
	    blowfish_encrypt(blowfish_ctx, (uint32_t *)vector, (uint32_t *)(vector + 4));
            memmove(memory->output + nblock, vector, block_size);
            break;
          case THREEFISH:
	    threefish_encrypt(threefish_ctx, (uint64_t*)vector, (uint64_t*)(memory->output + nblock));
            break;
        }

        strxor(memory->output + nblock, memory->input + nblock, block_size);
        memmove(vector, (operation ? memory->input : memory->output) + nblock, block_size);
      }
    }

    if (fwrite((void *)memory->output, 1, realread, fo) != realread) {
      fclose(fi);
      fclose(fo);

      free((void *)memory);
      memory = NULL;

      return -5;
    }
    else {
      fflush(fo);
    }

    position += (long int)realread;
    real_percent = (short)((float)position / div + 0.1);

    centreal(&real_percent);

    if (real_percent > past_percent) {
      if ((real_percent % PERIOD) == 0) {
        Form1->Shape4->Width = (int)((float)real_percent * cas) + 1;

        check = size_check(position);

        Form1->Label9->Caption = AnsiString(OPERATION_NAME[cipher ? (operation ? 1 : 0) : 2]) +
        ": " + AnsiString(ALGORITM_NAME[cipher]) + "; Îáðàáîòàíî: " +
        (check ? FloatToStrF(((float)position / (float)(INT_SIZE_DATA[check - 1])), ffFixed, 4, 2) :
                 IntToStr(position)) + " " + CHAR_SIZE_DATA[check] + " èç " +
        (check ? FloatToStrF(fsize_float, ffFixed, 4, 2) : IntToStr((int)(fsize_float + 0.1))) + " " + CHAR_SIZE_DATA[fsize_check] + "; Ïðîãðåññ: " + IntToStr(real_percent) + " %" ;

        Application->ProcessMessages();
      }
      past_percent = real_percent;
    }
  }

  fclose(fi);
  fclose(fo);

  meminit((void *)memory, 0x00, memory_ctx_len);
  free((void *)memory);
  memory = NULL;

  return 0;
}

static size_t vector_init(uint8_t * data, size_t size) {
  size_t i;
  size_t stack_trash; /* NOT initialized */

  for (i = 0; i < size; i++) {
    data[i] = (uint8_t)i ^ (uint8_t)genrand(0x00, 0xFF);
  }

  data[0] ^= (uint8_t)((uint8_t)stack_trash ^ (uint8_t)genrand(0x00, 0xFF));

  cursorpos(data); // X and Y cursor position xor operation for data[0] and data[1];

  size = size - 2;

  for (i = 0; i < size; i++) {
    if ((data[i] == data[i + 1]) && (data[i + 1] == data[i + 2])) {
      break;
    }
  }

  return i;
}

uint32_t MessageForUser(const int tumbler,
                        const char * head,
                        const char * message) {
                        
  uint32_t result = Application->MessageBox(message, head, tumbler);

  return result;
}

void __fastcall TForm1::Button4Click(TObject *Sender) {
  size_t ctx_len, block_size;
  int    key_len, real_read,
         cipher_number, operation;

  String UnicodeMsg = "";

  if (__strnlen(Edit1->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Èìÿ îáðàáàòûâàåìîãî ôàéëà íå ââåäåíî!");
    return;
  }

  if (__strnlen(Edit2->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Èìÿ ôàéëà íàçíà÷åíèÿ íå ââåäåíî!");
    return;
  }

  if (__strnlen(Memo1->Text.c_str(), 2048) == 0) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Ïàðîëü èëè èìÿ êëþ÷åâîãî ôàéëà íå ââåäåíû!");
    return;
  }

  if (Edit1->Text == Edit2->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Èìåíà îáðàáàòûâàåìîãî ôàéëà è ôàéëà íàçíà÷åíèÿ ñîâïàäàþò!");
    return;
  }

  if (Edit1->Text == Memo1->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Èìåíà îáðàáàòûâàåìîãî ôàéëà è êëþ÷åâîãî ôàéëà ñîâïàäàþò!");
    return;
  }

  if (Edit2->Text == Memo1->Text) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Èìåíà ôàéëà íàçíà÷åíèÿ è êëþ÷åâîãî ôàéëà ñîâïàäàþò!");
    return;
  }

  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[ARC4])) {
    cipher_number = ARC4;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[AES])) {
    cipher_number = AES;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[SERPENT])) {
    cipher_number = SERPENT;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[TWOFISH])) {
    cipher_number = TWOFISH;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[BLOWFISH])) {
    cipher_number = BLOWFISH;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH])) {
    cipher_number = THREEFISH;
  }
  else {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Àëãîðèòì øèôðîâàíèÿ íå áûë âûáðàí!");
    return;
  }

  key_len = 0;

  if (cipher_number == ARC4) {
    key_len = 2048;
  }
  else
  if (cipher_number == AES ||
      cipher_number == SERPENT ||
      cipher_number == TWOFISH) { // AES or SERPENT
    if (AnsiString(ComboBox2->Text) == AnsiString("128")) {
      if (cipher_number == AES) {
        AES_Rounds = 10;
      }
      key_len = 128;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("192")) {
      if (cipher_number == AES) {
        AES_Rounds = 12;
      }
      key_len = 192;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("256")) {
      if (cipher_number == AES) {
        AES_Rounds = 14;
      }
      key_len = 256;
    }
    else {
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "Äëèíà êëþ÷à øèôðîâàíèÿ íå áûëà âûáðàíà!");
      return;
    }
  }
  else
  if (cipher_number == BLOWFISH) {
    key_len = 448;
  }
  else
  if (cipher_number == THREEFISH) {
    if (AnsiString(ComboBox2->Text) == AnsiString("256")) {
      key_len = 256;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("512")) {
      key_len = 512;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("1024")) {
      key_len = 1024;
    }
    else {
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "Äëèíà êëþ÷à øèôðîâàíèÿ íå áûëà âûáðàíà!");
      return;
    }
  }

  key_len = (key_len / 8);

  /*
    ARC4      = (key_len = 256);
    AES       = (key_len = 16 or 24 or 32;
    SERPENT   = (key_len = 16 or 24 or 32);
    BLOWFISH  = (key_len = 56);
    THREEFISH = (key_len = 32 or 64 or 128);
  */

  operation = ENCRYPT;

  if (cipher_number != ARC4) {
    if (RadioButton1->Checked) {
      operation = ENCRYPT;
    }
    else
    if (RadioButton2->Checked) {
      operation = DECRYPT;
    }
    else {
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                     "Îïåðàöèÿ íå áûëà âûáðàíà!");
      return;
    }
  }

  if (FileExists(Edit2->Text) == True) {
    UnicodeMsg = "Ôàéë íàçíà÷åíèÿ ñóùåñòâóåò! Ñòàðûå äàííûå áóäóò óòåðÿíû!\n"
                 "Âû óâåðåíû ÷òî õîòèòå ïåðåçàïèñàòü åãî?";
    if (MessageForUser(MB_ICONWARNING + MB_YESNO, WARNING_MSG, UnicodeMsg.c_str()) == IDNO) {
    
      MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                     "Èçìåíèòå èìÿ ôàéëà íàçíà÷åíèÿ!");
      UnicodeMsg = "";
      return;
    }
    UnicodeMsg = "";
  }

  UnicodeMsg = "Ñãåíåðèðîâàòü " + IntToStr(key_len * 8) + "-áèòíûé êëþ÷ øèôðîâàíèÿ?";

  if (MessageForUser(MB_ICONINFORMATION + MB_YESNO, OK_MSG, UnicodeMsg.c_str()) == IDNO) {
    UnicodeMsg = "";
    return;
  }

  UnicodeMsg = "";

  uint8_t * buffer = (uint8_t*)calloc(key_len, 1);

  if (buffer == NULL) {
    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);
    return;
  }

  real_read = readfromfile(Memo1->Text.c_str(), buffer, key_len);

  if ((real_read > 0) && (real_read < key_len)) {
    UnicodeMsg = "Íåäîñòàòî÷íî äàííûõ â êëþ÷åâîì ôàéëå!\n\n"
                 "Áûëî ñ÷èòàíî:\t" + IntToStr(real_read) + " Áò\n"
                 "Íåîáõîäèìî:\t" + IntToStr(key_len) + " Áò";
    Application->MessageBox(UnicodeMsg.c_str(), WARNING_MSG, MB_ICONWARNING + MB_OK);

    UnicodeMsg = "";
    free((void *)buffer);
    buffer = NULL;

    return;
  }
  else
  if ((real_read == 0) || (real_read == -1)) {
    real_read = (int)__strnlen(Memo1->Text.c_str(), 256);

    if ((real_read > 7) && (real_read < 257)) {
      ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = (SHA256_CTX*) calloc(1, ctx_len);

      if (sha256_ctx != NULL) {
        Button4->Enabled = False;
        /* Crypt key generator; generate crypt key from password */
        KDFCLOMUL(sha256_ctx, (uint8_t *)Memo1->Text.c_str(), real_read, buffer, key_len);
        Button4->Enabled = True;

        meminit((void *)sha256_ctx, 0x00, ctx_len);
        free((void *)sha256_ctx);
        sha256_ctx = NULL;
      }
      else {
        MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

        meminit((void *)buffer, 0x00, key_len);
        free((void *)buffer);
        buffer = NULL;
        return;
      }
    }
    else {
      UnicodeMsg = "Äëèíà ñòðîêîâîãî êëþ÷à íåêîððåêòíà!\n\n"
                   "Áûëî ñ÷èòàíî:\t" + IntToStr(real_read) + " Áò\n"
                   "Íåîáõîäèìî:\tîò 8 äî 256 Áò";
      MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG, UnicodeMsg.c_str());

      UnicodeMsg = "";
      meminit((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;
      return;
    }
  }

  switch (cipher_number) {
    case AES:
      block_size = 16;
      break;
    case SERPENT:
      block_size = 16;
      break;
    case TWOFISH:
      block_size = 16;
      break;
    case BLOWFISH:
      block_size =  8;
      break;
    case THREEFISH:
      block_size = key_len;
      break;
  }

  uint8_t * vector = NULL;

  if (cipher_number != ARC4) {
    vector = (uint8_t*)calloc(block_size, 1);

    if (vector == NULL) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      meminit((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;
      return;
    }
  }

  if (cipher_number != ARC4 && operation == ENCRYPT) {

    srand((unsigned int)time(NULL));

    if (vector_init(vector, block_size) < (block_size - 2)) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                     "Êðèòè÷åñêàÿ îøèáêà ÃÏÑ×! Äàëüíåéøèå îïåðàöèè íå ïîçâîëåíû!"
                     "\nÑèñòåìíîå âðåìÿ îñòàíîâëåíî? Ïðîâåðüòå ñèñòåìíûå ÷àñû!");

      meminit((void *)vector, 0x00, block_size);
      meminit((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      Form1->Close();
    }
  }

  if (cipher_number == ARC4) {
    ctx_len = sizeof(ARC4_CTX);
    arc4_ctx = (ARC4_CTX *)calloc(1, ctx_len);

    if (arc4_ctx == NULL) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      meminit((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;

      return;
    }

    arc4_init(arc4_ctx, buffer, key_len);
  }
  if (cipher_number == AES) {
    ctx_len = 4 * (AES_Rounds + 1) * 4;
    rijndael_ctx = (uint32_t *) calloc(ctx_len, 1);
    if (rijndael_ctx == NULL) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        meminit((void *)vector, 0x00, block_size);

      meminit((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }

    AES_Rounds =
      rijndael_key_encrypt_init(rijndael_ctx, buffer, key_len * 8);
  }
  if (cipher_number == TWOFISH) {
    ctx_len = sizeof(TWOFISH_CTX);
    twofish_ctx = (TWOFISH_CTX *) calloc(1, ctx_len);
    if (twofish_ctx == NULL) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        meminit((void *)vector, 0x00, block_size);

      meminit((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }
    twofish_init(twofish_ctx, buffer, key_len);
  }
  else
  if (cipher_number == SERPENT) {
    ctx_len = sizeof(SERPENT_CTX);
    serpent_ctx = (SERPENT_CTX *) calloc(1, ctx_len);
    if (serpent_ctx == NULL) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        meminit((void *)vector, 0x00, block_size);

      meminit((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }
    serpent_init(serpent_ctx, key_len * 8, buffer);
  }
  else
  if (cipher_number == BLOWFISH) {
    ctx_len = sizeof(BLOWFISH_CTX);
    blowfish_ctx = (BLOWFISH_CTX*)calloc(1, ctx_len);
    if (blowfish_ctx == NULL) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        meminit((void *)vector, 0x00, block_size);

      meminit((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }
    blowfish_init(blowfish_ctx, buffer, key_len);
  }
  else
  if (cipher_number == THREEFISH) {
    ctx_len = sizeof(THREEFISH_CTX);
    threefish_ctx = (THREEFISH_CTX *)calloc(1, ctx_len);
    if (threefish_ctx == NULL) {
      MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        meminit((void *)vector, 0x00, block_size);

      meminit((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }
    threefish_init(threefish_ctx, (threefishkeysize_t)(key_len * 8),
                   (uint64_t *)buffer, (uint64_t *)buffer);
  }

  meminit((void *)buffer, 0x00, key_len);
  free((void *)buffer);
  buffer = NULL;

  int result = 0xDE;

  UnicodeMsg =
    "Ïðèñòóïèòü ê âûáðàííîé îïåðàöèè? Îñòàíîâèòü îïåðàöèþ áóäåò íåâîçìîæíî!\n\n"
    "Îïåðàöèÿ:\t" + String(OPERATION_NAME[cipher_number ? (operation ? 1 : 0) : 2]) + "\n"
    "Àëãîðèòì:\t" + String(ALGORITM_NAME[cipher_number]) + "\n"
    "Äëèíà êëþ÷à:\t" + IntToStr(key_len * 8).c_str() + " áèò" +
                       (24 == key_len || 128 == key_len ? "à" : "îâ");

  if (MessageForUser(MB_ICONQUESTION + MB_YESNO, OK_MSG, UnicodeMsg.c_str()) == IDYES) {

    Button4->Enabled = false;
    result = filecrypt(Edit1->Text.c_str(), Edit2->Text.c_str(), vector, block_size, cipher_number, operation);
  }

  UnicodeMsg = "";
  
/*
  if (MessageDlg("Ïðèñòóïèòü ê âûáðàííîé îïåðàöèè? Îòìåíèòü îïåðàöèþ áóäåò íåâîçìîæíî!\n\n"
                 "Îïåðàöèÿ:\t" + AnsiString(OPERATION_NAME[cipher_number ? (operation ? 1 : 0) : 2]) + "\n"
                 "Àëãîðèòì:\t" + AnsiString(ALGORITM_NAME[cipher_number]) + "\n"
                 "Äëèíà êëþ÷à:\t" + IntToStr(key_len * 8) + " áèò" + (24 == key_len ? "à" : "îâ"), mtInformation, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes) {

      Button4->Enabled = false;
      result = filecrypt(Edit1->Text.c_str(), Edit2->Text.c_str(), vector, block_size, cipher_number, operation);
  }
*/

  switch (result) {
    case  0xDE: MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                               "Îïåðàöèÿ áûëà îòìåíåíà!");
                break;

    case  0:    MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                               "Ôàéë óñïåøíî îáðàáîòàí!");
                break;
    case -1:    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                               "Ôàéë äëÿ îáðàáîòêè íå áûë îòêðûò!");
                break;
    case -2:    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                               "Ôàéë íàçíà÷åíèÿ íå áûë îòêðûò!");
                break;
    case -3:    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                               "Ôàéë äëÿ îáðàáîòêè ïóñò èëè åãî ðàçìåð"
                               " ïðåâûøàåò 2 ÃèÁ!");
                break;
    case -4:    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                               MEMORY_BLOCKED);
                break;
    case -5:    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                               "Îøèáêà çàïèñè â ôàéë!");
                break;
    case -6:    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                               "Îøèáêà ÷òåíèÿ èç ôàéëà!");
                break;
  }

  if ((result == 0) && (CheckBox1->Checked == True)) {
    UnicodeMsg = "Âû óâåðåíû ÷òî õîòèòå óíè÷òîæèòü ôàéë äëÿ îáðàáîòêè?\n"
                 "Ñòåðòûå äàííûå áóäåò íåâîçìîæíî âîññòàíîâèòü!";

    if (MessageForUser(MB_ICONWARNING + MB_YESNO, WARNING_MSG,
                       UnicodeMsg.c_str()) == IDYES) {
      Button4->Enabled = false;

      if ((erasedfile(Edit1->Text.c_str()) == 0) &&
          (DeleteFile(Edit1->Text)         == True)) {

        MessageForUser(MB_ICONINFORMATION + MB_OK, OK_MSG,
                       "Ôàéë áûë óíè÷òîæåí!");
      }
      else
        MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG,
                       "Îøèáêà óíè÷òîæåíèÿ ôàéëà!");
    }
    UnicodeMsg = "";
  }

  Form1->Shape4->Width = 0;
  Application->ProcessMessages();

  switch (cipher_number) {
    case ARC4:      meminit((void *)arc4_ctx->secret_key, 0x00, key_len);
                    free((void *)arc4_ctx);
                    arc4_ctx = NULL;
                    break;

    case AES:       meminit((void *)rijndael_ctx, 0x00, ctx_len);
                    free((void *)rijndael_ctx);
                    rijndael_ctx = NULL;
                    break;

    case SERPENT:   meminit((void *)serpent_ctx, 0x00, ctx_len);
                    free((void *)serpent_ctx);
                    serpent_ctx = NULL;
                    break;

    case TWOFISH:   meminit((void *)twofish_ctx, 0x00, ctx_len);
                    free((void *)twofish_ctx);
                    twofish_ctx = NULL;
                    break;

    case BLOWFISH:  meminit((void *)blowfish_ctx, 0x00, ctx_len);
                    free((void *)blowfish_ctx);
                    blowfish_ctx = NULL;
                    break;

    case THREEFISH: meminit((void *)threefish_ctx, 0x00, ctx_len);
                    free((void *)threefish_ctx);
                    threefish_ctx = NULL;
                    break;

  }

  if (vector != NULL) {
    meminit((void *)vector, 0x00, block_size);
    free((void *)vector);
    vector = NULL;
  }
  Button4->Enabled = true;
}

void __fastcall TForm1::Label5Click(TObject *Sender) {
  Form1->Close();
}

void __fastcall TForm1::Label7Click(TObject *Sender) {
  Form2->Label4->Caption = PROGRAMM_NAME;

  Form2->Shape1->Brush->Color = FORM_HEAD_COLOR;
  Form2->Shape1->Pen->Color   = FORM_HEAD_COLOR;

  Form2->Shape2->Pen->Color   = FORM_HEAD_COLOR;

  Form2->Label4->Color = FORM_HEAD_COLOR;
  Form2->Label5->Color = FORM_HEAD_COLOR;
  Form2->Show();
}

void __fastcall TForm1::Button5Click(TObject *Sender) {
  int len = atoi(Edit3->Text.c_str());

  if ((len < 8) || (len > 256)) {
    MessageForUser(MB_ICONWARNING + MB_OK, WARNING_MSG,
                   "Ââåäèòå ÷èñëî îò 8 äî 256!");
    return;
  }

  size_t ctx_len        = sizeof(ARC4_CTX);
  size_t memory_ctx_len = sizeof(MEMORY_CTX);

  MEMORY_CTX * memory   = (MEMORY_CTX *)calloc(1, memory_ctx_len);
               arc4_ctx = (ARC4_CTX *)  calloc(1, ctx_len);

  if ((memory == NULL) || (arc4_ctx == NULL)) {
    if (memory != NULL) {
      free((void *)memory);
      memory = NULL;
    }

    if (arc4_ctx != NULL) {
      free((void *)arc4_ctx);
      arc4_ctx = NULL;
    }

    MessageForUser(MB_ICONERROR + MB_OK, ERROR_MSG, MEMORY_BLOCKED);
    return;
  }

  for (int i = 0; i < len; i++) {
    memory->input[i] = (uint8_t)genrand(0x00, 0xFF);
  }

  arc4_init(arc4_ctx, memory->input, len);
  arc4(arc4_ctx, memory->input, memory->output, len);

  meminit((void *)memory->input, 0x00, len);

  base64encode(memory->output, memory->input, len);
  memory->input[len] = 0x00;

  Memo1->Clear();
  Memo1->Lines->Text = AnsiString((char*)memory->input);

  meminit((void *)arc4_ctx->secret_key, 0x00, ctx_len);
  meminit((void *)memory, 0x00, memory_ctx_len);

  free((void *)memory);
  free((void *)arc4_ctx);

  memory = NULL;
  arc4_ctx = NULL;
}

void __fastcall TForm1::Shape2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y) {
  ReleaseCapture();
  Form1->Perform(WM_SYSCOMMAND, 0xF012, 0);
}

