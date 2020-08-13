#include <io.h>
#include <vcl.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>

#include "src/arc4.h"
#include "src/base64.h"
#include "src/sha256.h"
#include "src/serpent.h"
#include "src/rijndael.h"
#include "src/blowfish.h"
#include "src/threefish-512.h"

#include "src/myfunctions.h"

#include "Unit1.h"
#include "Unit2.h"

#define ARC4                   0
#define AES                    1
#define SERPENT                2
#define BLOWFISH               3
#define THREEFISH              4

#define ENCRYPT             0x00
#define DECRYPT             0xDE

#define SHA_256_HASH_LENGTH   32

#define BLOCK_SIZE          1024
#define DATA_SIZE           4096

#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

const float cas = 4.87; /* ((float)Form1->Shape4->Width / (float)100) or (488 ïêñ / 100) */

const char * PARAM_READ_BYTE    = "rb";
const char * PARAM_WRITE_BYTE   = "wb";
const char * PARAM_REWRITE_BYTE = "r+b";

const TColor FORM_HEAD_COLOR = TColor(0x00623E00);

const uint32_t INT_SIZE_DATA[] = {1024, 1048576, 1073741824};

const char * CHAR_SIZE_DATA[] = {"Áò", "ÊèÁ", "ÌèÁ", "ÃèÁ"};

const char * OPERATION_NAME[] = {"Øèôðîâàíèå", "Ðàñøèôðîâêà", "Ïîòîêîâàÿ îáðàáîòêà"};

const char * ALGORITM_NAME[] =  {"ARC4", "AES-CFB", "SERPENT-CFB",
                                 "BLOWFISH-CFB", "THREEFISH-512-CFB"};

const char * PROGRAMM_NAME  = "PlexusTCL Crypter 4.64 02AUG20 [RU]";
const char * MEMORY_BLOCKED = "Îøèáêà âûäåëåíèÿ ïàìÿòè!";

ARC4_CTX      * arc4_ctx      = NULL;
uint8_t       * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

typedef struct {
  uint8_t input [DATA_SIZE];
  uint8_t output[DATA_SIZE];
} MEMORY_CTX;

__fastcall TForm1::TForm1(TComponent* Owner): TForm(Owner) {

}

void __fastcall TForm1::Button1Click(TObject *Sender) {
  OpenDialog1->Title = "Ôàéë äëÿ îáðàáîòêè";
  if (OpenDialog1->Execute()) {
    Edit1->Clear();
    Edit1->Text = OpenDialog1->FileName;
  }
}

void __fastcall TForm1::Button2Click(TObject *Sender) {
  SaveDialog1->Title = "Ôàéë íàçíà÷åíèÿ";
  if (SaveDialog1->Execute()) {
    Edit2->Clear();
    Edit2->Text = SaveDialog1->FileName;
  }
}

void __fastcall TForm1::Button3Click(TObject *Sender) {
  OpenDialog1->Title = "Êëþ÷ øèôðîâàíèÿ";
  if (OpenDialog1->Execute()) {
    Memo1->Clear();
    Memo1->Lines->Text = OpenDialog1->FileName;
  }
}

void __fastcall TForm1::FormCreate(TObject *Sender) {

  srand((unsigned int)time(NULL));

  for (char i = 0; i < 5; i++)
    ComboBox1->Items->Add(ALGORITM_NAME[i]);

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
  if ((AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[AES])) ||
      (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[SERPENT])) ) {
    Label4->Visible = True;
    ComboBox2->Visible = True;
    RadioButton1->Visible = True;
    RadioButton2->Visible = True;
  }
  else
  if ((AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[BLOWFISH])) ||
      (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH]))) {
    Label4->Visible = False;
    ComboBox2->Visible = False;
    RadioButton1->Visible = True;
    RadioButton2->Visible = True;
  }
}

void cursorpos(uint8_t * data) {
  TPoint position;

  GetCursorPos(&position);

  data[0] ^= (uint8_t)(position.x);
  data[1] ^= (uint8_t)(position.y);

  position.x = position.y = 0;
}

void password_to_key(SHA256_CTX * sha256_ctx, const uint8_t * password, const size_t password_len,
                     uint8_t * key, const size_t key_len) {

  size_t i, j, k;
  size_t count = key_len + (password_len * 2) - 1;

  uint8_t hash[SHA256_BLOCK_SIZE];

  sha256_init(sha256_ctx);

  for (i = k = 0; i < key_len; ++i, ++k) { /* MAX = 196,352 */
    for (j = 0; j < count; ++j) {
      sha256_update(sha256_ctx, password, password_len);
      sha256_final(sha256_ctx, hash);
    }

    if (k == SHA256_BLOCK_SIZE)
      k = 0;

    key[i] = hash[k];
  }

  memset((void *)hash, 0x00, SHA256_BLOCK_SIZE);
}

int size_check(uint32_t size) {
  int result = 0;

  if (size < INT_SIZE_DATA[0]) {
    result = 0;
  }
  else
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

void centreal(short * real_percent) {
  if (*real_percent > 100) {
    *real_percent = 100;
  }
}

long int size_of_file(FILE * f) {

  fseek(f, 0, SEEK_END);
  long int result = ftell(f);
  fseek(f, 0, SEEK_SET);

  return result;
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

  short real_percent = 0;
  short past_percent = 0;

  float div = (float)fsize / 100.0;
  int   check;

  size_t realread;

  while (position < fsize) {
    realread = fread((void *)data, 1, BLOCK_SIZE, f);

    if (realread > 0) {
      memset((void *)data, 0x00, realread);
      fseek(f, position, SEEK_SET);

      if (fwrite((void *)data, 1, realread, f) != realread) {
        fclose(f);
        free((void *)data);
        data = NULL;
        return -1;
      }
      else
        fflush(f);

      position += (long int)realread;
    }

    real_percent = (int)((float)position / div + 0.1);

    centreal(&real_percent);

    if (real_percent > past_percent) {
      if ((real_percent % 4) == 0) {
        Form1->Shape4->Width = (int)((float)real_percent * cas) + 1;
        check = size_check(position);

        Form1->Label9->Caption = "Óíè÷òîæåíèå ôàéëà; Îáðàáîòàíî: " +
        (check ? FloatToStrF(((float)position / (float)INT_SIZE_DATA[check - 1]), ffFixed, 4, 2) :
                 IntToStr(position)) +
        " " + CHAR_SIZE_DATA[check] + "; Ïðîãðåññ: " + IntToStr(real_percent) + " %" ;

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
  else
    return 0;
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

  Form1->Shape4->Width = 0;

  while (position < fsize) {
    if (cipher != ARC4 && position == 0) {
      switch (operation) {
        case ENCRYPT: switch (cipher) {
                        case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->output);
                                        break;
                        case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)memory->output);
                                        break;
                        case BLOWFISH:  memmove(memory->output, vector, block_size);
                                        blowfish_encrypt(blowfish_ctx, (unsigned long *)memory->output, (unsigned long *)(memory->output + 4));
                                        break;
                        case THREEFISH: threefish_encrypt(threefish_ctx, (uint64_t*)(vector), (uint64_t*)memory->output);
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
        case DECRYPT: if (fread((void *)vector, 1, block_size, fi) != block_size) {
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
          case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->output + nblock);
                          break;
          case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)(memory->output + nblock));
                          break;
          case BLOWFISH:  blowfish_encrypt(blowfish_ctx, (unsigned long *)vector, (unsigned long *)(vector + 4));
                          memmove(memory->output + nblock, vector, block_size);
                          break;
          case THREEFISH: threefish_encrypt(threefish_ctx, (uint64_t*)(vector), (uint64_t*)(memory->output + nblock));
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
      if ((real_percent % 4) == 0) {
        Form1->Shape4->Width = (int)((float)real_percent * cas) + 1;

        check = size_check(position);

        Form1->Label9->Caption = AnsiString(OPERATION_NAME[cipher ? (operation ? 1 : 0) : 2]) +
        ": " + AnsiString(ALGORITM_NAME[cipher]) + "; Îáðàáîòàíî: " +
        (check ? FloatToStrF(((float)position / (float)INT_SIZE_DATA[check - 1]), ffFixed, 4, 2) :
                 IntToStr(position)) +
        " " + CHAR_SIZE_DATA[check] + "; Ïðîãðåññ: " + IntToStr(real_percent) + " %" ;

        Application->ProcessMessages();
      }

      past_percent = real_percent;
    }
  }

  fclose(fi);
  fclose(fo);

  memset((void *)memory, 0x00, memory_ctx_len);
  free((void *)memory);
  memory = NULL;

  return 0;
}

size_t vector_init(uint8_t * data, size_t size) {
  size_t i;

  for (i = 0; i < size; i++)
    data[i] = (uint8_t)genrand(0, 255);

  size = size - 2;
  cursorpos(data); // X and Y cursor position xor operation for data[0] and data[1];

  for (i = 0; i < size; i++) {
    if (data[i] == data[i + 1] && data[i + 1] == data[i + 2])
      break;
  }

  return i;
}

void __fastcall TForm1::Button4Click(TObject *Sender) {
  size_t ctx_len, block_size;
  int    key_len, real_read,
         cipher_number, operation;

  if (strlen(Edit1->Text.c_str()) == 0) {
    ShowMessage("Èìÿ îáðàáàòûâàåìîãî ôàéëà íå ââåäåíî!");
    return;
  }

  if (strlen(Edit2->Text.c_str()) == 0) {
    ShowMessage("Èìÿ ôàéëà íàçíà÷åíèÿ íå ââåäåíî!");
    return;
  }

  if (strlen(Memo1->Text.c_str()) == 0) {
    ShowMessage("Èìÿ êëþ÷åâîãî ôàéëà íå ââåäåíî!");
    return;
  }

  if (Edit1->Text == Edit2->Text) {
    ShowMessage("Èìåíà îáðàáàòûâàåìîãî ôàéëà è ôàéëà íàçíà÷åíèÿ ñîâïàäàþò!");
    return;
  }

  if (Edit1->Text == Memo1->Text) {
    ShowMessage("Èìåíà îáðàáàòûâàåìîãî ôàéëà è êëþ÷åâîãî ôàéëà ñîâïàäàþò!");
    return;
  }

  if (Edit2->Text == Memo1->Text) {
    ShowMessage("Èìåíà ôàéëà íàçíà÷åíèÿ è êëþ÷åâîãî ôàéëà ñîâïàäàþò!");
    return;
  }

  if (FileExists(Edit2->Text) == True) {
    if (MessageDlg("Ôàéë íàçíà÷åíèÿ ñóùåñòâóåò! Ñòàðûå äàííûå áóäóò óòåðÿíû!\n"
                   "Âû óâåðåíû ÷òî õîòèòå ïåðåçàïèñàòü åãî?", mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo) {

      ShowMessage("Èçìåíèòå èìÿ ôàéëà íàçíà÷åíèÿ!");
      return;
    }
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
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[BLOWFISH])) {
    cipher_number = BLOWFISH;
  }
  else
  if (AnsiString(ComboBox1->Text) == AnsiString(ALGORITM_NAME[THREEFISH])) {
    cipher_number = THREEFISH;
  }
  else {
    ShowMessage("Àëãîðèòì øèôðîâàíèÿ íå áûë âûáðàí!");
    return;
  }

  if (cipher_number == ARC4) {
    key_len = 2048;
  }
  else
  if (cipher_number == AES || cipher_number == SERPENT) { // AES or SERPENT
    if (AnsiString(ComboBox2->Text) == AnsiString("128")) {
      if (cipher_number == AES) {
        Nk = 4;
        Nr = 10;
      }
      key_len = 128;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("192")) {
      if (cipher_number == AES) {
        Nk = 6;
        Nr = 12;
      }
      key_len = 192;
    }
    else
    if (AnsiString(ComboBox2->Text) == AnsiString("256")) {
      if (cipher_number == AES) {
        Nk = 8;
        Nr = 14;
      }
      key_len = 256;
    }
    else {
      ShowMessage("Äëèíà êëþ÷à øèôðîâàíèÿ íå âûáðàíà!");
      return;
    }
  }
  else
  if (cipher_number == BLOWFISH) {
    key_len = 448;
  }
  else
  if (cipher_number == THREEFISH) {
    key_len = 512;
  }

  key_len /= 8;

  /*
    ARC4      = (key_len = 256);
    AES       = (key_len = 16 or 24 or 32;
    SERPENT   = (key_len = 16 or 24 or 32);
    BLOWFISH  = (key_len = 56);
    THREEFISH = (key_len = 64);
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
      ShowMessage("Îïåðàöèÿ íå áûëà âûáðàíà!");
      return;
    }
  }

  uint8_t * buffer = (uint8_t*)calloc(key_len, 1);

  if (buffer == NULL) {
    ShowMessage(MEMORY_BLOCKED);
    return;
  }

  real_read = readfromfile(Memo1->Text.c_str(), buffer, key_len);

  if ((real_read > 0) && (real_read < key_len)) {
    ShowMessage("Äàííûõ â êëþ÷åâîì ôàéëå ñëèøêîì ìàëî!\n\n"
                "Áûëî ñ÷èòàíî: " + IntToStr(real_read) + " " + CHAR_SIZE_DATA[0] + "\n" +
                "Íåîáõîäèìî: " + IntToStr(key_len) + " " + CHAR_SIZE_DATA[0]);
    free((void *)buffer);
    buffer = NULL;
    
    return;
  }
  else
  if ((real_read == 0) || (real_read == -1)) {
    real_read = strlen(Memo1->Text.c_str());
    if ((real_read > 7) && (real_read < 257)) {
      ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = (SHA256_CTX*) calloc(1, ctx_len);

      if (sha256_ctx != NULL) {
        password_to_key(sha256_ctx, (uint8_t *)Memo1->Text.c_str(), real_read, buffer, key_len);

        memset((void *)sha256_ctx, 0x00, ctx_len);
        free((void *)sha256_ctx);
        sha256_ctx = NULL;
      }
      else {
        ShowMessage(MEMORY_BLOCKED);

        memset((void *)buffer, 0x00, key_len);
        free((void *)buffer);
        buffer = NULL;
        return;
      }
    }
    else {
      ShowMessage("Äëèíà ñòðîêîâîãî êëþ÷à íåêîððåêòíà!\n\n"
                  "Áûëî ñ÷èòàíî: " + IntToStr(real_read) + " " + CHAR_SIZE_DATA[0] + "\n" +
                  "Íåîáõîäèìî: îò 8 äî 256 " + CHAR_SIZE_DATA[0]);

      memset((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;
      return;
    }
  }

  switch (cipher_number) {
    case AES:       block_size = 16;
                    break;
    case SERPENT:   block_size = 16;
                    break;
    case BLOWFISH:  block_size =  8;
                    break;
    case THREEFISH: block_size = 64;
                    break;
  }

  uint8_t * vector = NULL;

  if (cipher_number != ARC4) {
    vector = (uint8_t*)calloc(block_size, 1);

    if (vector == NULL) {
      ShowMessage(MEMORY_BLOCKED);
                       
      memset((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;
      return;
    }
  }

  if (cipher_number != ARC4 && operation == ENCRYPT) {

    srand((uint32_t)time(NULL));

    if (vector_init(vector, block_size) < (block_size - 2)) {
      ShowMessage("Êðèòè÷åñêàÿ îøèáêà ÃÏÑ×! Äàëüíåéøèå îïåðàöèè íå ïîçâîëåíû!");

      memset((void *)vector, 0x00, block_size);
      memset((void *)buffer, 0x00, key_len);

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
      ShowMessage(MEMORY_BLOCKED);

      memset((void *)buffer, 0x00, key_len);
      free((void *)buffer);
      buffer = NULL;

      return;
    }

    arc4_init(arc4_ctx, buffer, key_len);
  }
  if (cipher_number == AES) {
    ctx_len = Nb * (Nr + 1) * 4;
    rijndael_ctx = (uint8_t *) calloc(ctx_len, 1);
    if (rijndael_ctx == NULL) {
      ShowMessage(MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }

    rijndael_init(buffer, rijndael_ctx);
  }
  if (cipher_number == SERPENT) {
    ctx_len = sizeof(SERPENT_CTX);
    serpent_ctx = (SERPENT_CTX *) calloc(1, ctx_len);
    if (serpent_ctx == NULL) {
      ShowMessage(MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

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
      ShowMessage(MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

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
      ShowMessage(MEMORY_BLOCKED);

      if (operation == ENCRYPT)
        memset((void *)vector, 0x00, block_size);

      memset((void *)buffer, 0x00, key_len);

      free((void *)vector);
      free((void *)buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }
    threefish_init(threefish_ctx, (uint64_t *)buffer, (uint64_t *)buffer);
  }

  memset((void *)buffer, 0x00, key_len);
  free((void *)buffer);
  buffer = NULL;
  
  int result = 0xDE;

  if (MessageDlg("Ïðèñòóïèòü ê âûáðàííîé îïåðàöèè? Îòìåíèòü îïåðàöèþ áóäåò íåâîçìîæíî!\n\n"
                 "Îïåðàöèÿ: " + AnsiString(OPERATION_NAME[cipher_number ? (operation ? 1 : 0) : 2]) + "\n"
                 "Àëãîðèòì: " + AnsiString(ALGORITM_NAME[cipher_number]) + "\n"
                 "Äëèíà êëþ÷à: " + IntToStr(key_len * 8), mtCustom, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes) {

    Button4->Enabled = false;
    result = filecrypt(Edit1->Text.c_str(), Edit2->Text.c_str(), vector, block_size, cipher_number, operation);
  }

/*  
  char * CRYPT_ERROR_MESSAGE[0..6] = {"", "", ...};

  if (0xDE == result )
    ShowMessage("Îïåðàöèÿ áûëà îòìåíåíà!");
  else {
    ShowMessage(CRYPT_ERROR_MESSAGE[0 - result]);
  }
*/
  
  switch (result) {
    case  0xDE: ShowMessage("Îïåðàöèÿ áûëà îòìåíåíà!");
                break;

    case  0:    ShowMessage("Ôàéë óñïåøíî îáðàáîòàí!");
                break;
    case -1:    ShowMessage("Ôàéë äëÿ îáðàáîòêè íå áûë îòêðûò!");
                break;
    case -2:    ShowMessage("Ôàéë íàçíà÷åíèÿ íå áûë îòêðûò!");
                break;
    case -3:    ShowMessage("Ôàéë äëÿ îáðàáîòêè ïóñò èëè åãî ðàçìåð ïðåâûøàåò 2 Ãá!");
                break;
    case -4:    ShowMessage(MEMORY_BLOCKED);
                break;
    case -5:    ShowMessage("Îøèáêà çàïèñè â ôàéë!");
                break;
    case -6:    ShowMessage("Îøèáêà ÷òåíèÿ èç ôàéëà!");
                break;
  }

  if ((result == 0) && (CheckBox1->Checked == True)) {
    if (MessageDlg("Âû óâåðåíû ÷òî õîòèòå óíè÷òîæèòü ôàéë äëÿ îáðàáîòêè?\n"
                   "Ñòåðòûå äàííûå íåâîçìîæíî áóäåò âîññòàíîâèòü!", mtWarning, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes) {

      if ((erasedfile(Edit1->Text.c_str()) == 0) && (DeleteFile(Edit1->Text) == True)) {
        ShowMessage("Ôàéë áûë óíè÷òîæåí!");
      }
      else
        ShowMessage("Îøèáêà óíè÷òîæåíèÿ ôàéëà!");
    }
  }

  Form1->Shape4->Width = 0;
  Application->ProcessMessages();

  switch (cipher_number) {
    case ARC4:      memset((void *)arc4_ctx->secret_key, 0x00, key_len);
                    free((void *)arc4_ctx);
                    arc4_ctx = NULL;
                    break;

    case AES:       memset((void *)rijndael_ctx, 0x00, ctx_len);
                    free((void *)rijndael_ctx);
                    rijndael_ctx = NULL;
                    break;

    case SERPENT:   memset((void *)serpent_ctx, 0x00, ctx_len);
                    free((void *)serpent_ctx);
                    serpent_ctx = NULL;
                    break;

    case BLOWFISH:  memset((void *)blowfish_ctx, 0x00, ctx_len);
                    free((void *)blowfish_ctx);
                    blowfish_ctx = NULL;
                    break;

    case THREEFISH: memset((void *)threefish_ctx, 0x00, ctx_len);
                    free((void *)threefish_ctx);
                    threefish_ctx = NULL;
                    break;

  }

  if (vector != NULL) {
    memset((void *)vector, 0x00, block_size);
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
    ShowMessage("Ââåäèòå ÷èñëî îò 8 äî 256!");
    return;
  }

  size_t ctx_len        = sizeof(ARC4_CTX);
  size_t memory_ctx_len = sizeof(MEMORY_CTX);

  MEMORY_CTX * memory   = (MEMORY_CTX *)calloc(1, memory_ctx_len);
  arc4_ctx = (ARC4_CTX *) calloc(1, ctx_len);

  if ((memory == NULL) || (arc4_ctx == NULL)) {
    if (memory != NULL) {
      free((void *)memory);
      memory = NULL;
    }

    if (arc4_ctx != NULL) {
      free((void *)arc4_ctx);
      arc4_ctx = NULL;
    }

    ShowMessage(MEMORY_BLOCKED);
    return;
  }

  for (int i = 0; i < len; i++) {
    memory->input[i] = (uint8_t)genrand(0x00, 0xFF);
  }

  arc4_init(arc4_ctx, memory->input, len);
  arc4(arc4_ctx, memory->input, memory->output, len);

  memset((void *)memory->input, 0x00, len);

  base64encode(memory->output, memory->input, len);
  memory->input[len] = 0x00;

  Memo1->Clear();
  Memo1->Lines->Text = AnsiString((char*)memory->input);

  memset((void *)arc4_ctx->secret_key, 0x00, ctx_len);
  memset((void *)memory, 0x00, memory_ctx_len);

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

