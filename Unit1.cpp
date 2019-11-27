#include <vcl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
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

#define ARC4             0
#define AES              1
#define SERPENT          2
#define BLOWFISH         3
#define THREEFISH        4

#define ENCRYPT       0x00
#define DECRYPT       0xDE

#define DATA_SIZE     4096

#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

const TColor FORM_HEAD_COLOR = TColor(0x00804000);

const uint32_t INT_SIZE_DATA[] = {1024, 1048576, 1073741824};

const char * CHAR_SIZE_DATA[] = {"Áò", "Êá", "Ìá", "Ãá"};

const char * OPERATION_NAME[] = {"Øèôðîâàíèå", "Ðàñøèôðîâêà", "Ïîòîêîâàÿ îáðàáîòêà"};

const char * ALGORITM_NAME[] =  {"ARC4", "AES-CFB", "SERPENT-CFB",
                                 "BLOWFISH-CFB", "THREEFISH-512-CFB"};

const char * PROGRAMM_NAME = "PlexusTCL Crypter 4.22 27NOV19 [RU]";

uint8_t       * rijndael_ctx  = NULL;
SERPENT_CTX   * serpent_ctx   = NULL;
BLOWFISH_CTX  * blowfish_ctx  = NULL;
THREEFISH_CTX * threefish_ctx = NULL;

typedef struct {
  uint8_t data[DATA_SIZE];
  uint8_t post[DATA_SIZE];
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
  for (char i = 0; i < 5; i++)
    ComboBox1->Items->Add(ALGORITM_NAME[i]);

  Panel1->Color = FORM_HEAD_COLOR;
  Form1->Label6->Caption = PROGRAMM_NAME;
  Form1->Caption = PROGRAMM_NAME;
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

void sha256sum(SHA256_CTX * ctx, uint8_t * data, int len) {
  int count = (len * 2) - 1;

  sha256_init(ctx);

  for (int i = 0; i < count; i++) { /* MAX = 511 */
    sha256_update(ctx, data, len);
  }

  sha256_final(ctx);
}

void hash_init(uint8_t * input, uint8_t * output, int len) {
  int i, j;
  uint8_t temp;

  for (i = j = 0; i < len; i++) {
    temp = output[j];
    input[i] = temp;

    if (j == 32)
      j = 0;
    else
      j++;
  }
}

char size_check(uint64_t size) {
  char result = 0;

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

int erasedfile(uint8_t * filename) {
  FILE * f = fopen(filename, "r+b");

  if (f == NULL) {
    return -1;
  }

  fseek(f, 0, SEEK_END);
  long int fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (fsize <= 0) {
    fclose(f);
    return -2;
  }

  uint8_t * data = (uint8_t *)calloc(512, 1);

  if (data == NULL) {
    fclose(f);
    return -3;
  }

  short int real = 0;
  short int past = 0;

  float div = (float)fsize / 100.0;
  float cas = 4.85; /* (float)Form1->Shape2->Width / (float)100 */
  char  check;

  long int position = 0;

  while (position < fsize) {
    if (fwrite(data, 1, 512, f) != 512) {
      fclose(f);

      free(data);
      data = NULL;

      return -4;
    }
    else
      fflush(f);

    position += 512;

    real = (short int)((float)position / div + 0.1);

    if (real > past) {
      Form1->Shape2->Width = ((int)((float)real * cas)) + 2;

      check = size_check(position);

      Form1->StatusBar1->Panels->Items[0]->Text = "Óíè÷òîæåíèå ôàéëà; Îáðàáîòàíî: " +
        FloatToStrF((check ? ((float)position / (float)INT_SIZE_DATA[check - 1]) : position), ffFixed, 4, 2) +
        " " + CHAR_SIZE_DATA[check] + "; Ïðîãðåññ: " + IntToStr(real) + " %" ;

      Application->ProcessMessages();

      past = real;
    }
  }

  fclose(f);
  free(data);
  data = NULL;

  return 0;
}

int filecrypt(uint8_t * finput, uint8_t * foutput, uint8_t * vector, int block_size, int cipher, int operation) {

  FILE * fi = fopen(finput, "rb");

  if (fi == NULL) {
    return -1;
  }

  FILE * fo = fopen(foutput, "wb");

  if (fo == NULL) {
    fclose(fi);
    return -2;
  }

  fseek(fi, 0, SEEK_END);
  long int fsize = ftell(fi);
  fseek(fi, 0, SEEK_SET);

  if ((fsize == -1L) || (fsize == 0)) {
    fclose(fi);
    fclose(fo);
    return -3;
  }

  int memory_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = (MEMORY_CTX *) calloc(1, memory_len);

  if (memory == NULL) {
    fclose(fi);
    fclose(fo);
    return -4;
  }

  float div = (float)fsize / 100.0;
  float cas = 4.85; // ((float)(Form1->Shape2->Width)) / 100.0;

  char check;

  short int nblock;

  long  int position = 0;
  short int realread = 0;

  short int real_percent = 0;
  short int past_percent = 0;

  Form1->Shape2->Width = 0;

  while (position < fsize) {
    if (cipher != ARC4 && position == 0) {
      switch (operation) {
        case ENCRYPT: switch (cipher) {
                        case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->post);
                                        break;
                        case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)memory->post);
                                        break;
                        case BLOWFISH:  memmove(memory->post, vector, block_size);
                                        blowfish_encrypt(blowfish_ctx, (unsigned long *)memory->post, (unsigned long *)(memory->post + 4));
                                        break;
                        case THREEFISH: threefish_encrypt(threefish_ctx, (U64*)(vector), (U64*)memory->post);
                                        break;
                      }
                      memmove(vector, memory->post, block_size);
                      if (fwrite(vector, 1, block_size, fo) != block_size) {
                        fclose(fi);
                        fclose(fo);

                        free(memory);
                        memory = NULL;

                        return -5;
                      }
                      else
                        fflush(fo);
                      break;
        case DECRYPT: if (fread(vector, 1, block_size, fi) != block_size) {
                        fclose(fi);
                        fclose(fo);

                        free(memory);
                        memory = NULL;

                        return -6;
                      }
                      position += block_size;
                      break;
      }
    }

    realread = (short int)fread(memory->data, 1, DATA_SIZE, fi);

    if (cipher == ARC4) {
      arc4(memory->data, memory->post, realread);
    }
    else {
      for (nblock = 0; nblock < realread; nblock += block_size) {
        switch (cipher) {
          case AES:       rijndael_encrypt(rijndael_ctx, vector, memory->post + nblock);
                          break;
          case SERPENT:   serpent_encrypt(serpent_ctx, (unsigned long *)vector, (unsigned long *)(memory->post + nblock));
                          break;
          case BLOWFISH:  blowfish_encrypt(blowfish_ctx, (unsigned long *)vector, (unsigned long *)(vector + 4));
                          memmove(memory->post + nblock, vector, block_size);
                          break;
          case THREEFISH: threefish_encrypt(threefish_ctx, (U64*)(vector), (U64*)(memory->post + nblock));
                          break;
        }

        strxor(memory->post + nblock, memory->data + nblock, block_size);
        memmove(vector, (operation ? memory->data : memory->post) + nblock, block_size);
      }
    }

    if (fwrite(memory->post, 1, realread, fo) != realread) {
      fclose(fi);
      fclose(fo);

      free(memory);
      memory = NULL;

      return -5;
    }
    else
      fflush(fo);

    position += realread;
    real_percent = (short int)((float)position / div + 0.1);

    if (real_percent > past_percent) {
      Form1->Shape2->Width = ((int)((float)real_percent * cas)) + 2;

      check = size_check(position);

      Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(OPERATION_NAME[cipher ? (operation ? 1 : 0) : 2]) +
        ": " + AnsiString(ALGORITM_NAME[cipher]) + "; Îáðàáîòàíî: " +
        FloatToStrF((check ? ((float)position / (float)INT_SIZE_DATA[check - 1]) : position), ffFixed, 4, 2) +
        " " + CHAR_SIZE_DATA[check] + "; Ïðîãðåññ: " + IntToStr(real_percent) + " %" ;

      Application->ProcessMessages();

      past_percent = real_percent;
    }
  }

  fclose(fi);
  fclose(fo);

  memset(memory, 0x00, memory_len);
  free(memory);
  memory = NULL;

  return 0;
}

void __fastcall TForm1::Button4Click(TObject *Sender) {
  short int cipher_number, operation,
            key_len, real_read,
            block_size, i;

  if (Edit1->Text == "") {
    ShowMessage("Èìÿ îáðàáàòûâàåìîãî ôàéëà íå ââåäåíî!");
    return;
  }

  if (Edit2->Text == "") {
    ShowMessage("Èìÿ ôàéëà íàçíà÷åíèÿ íå ââåäåíî!");
    return;
  }

  if (Memo1->Text == "") {
    ShowMessage("Èìÿ êëþ÷åâîãî ôàéëà íå ââåäåíî!");
    return;
  }

  if (strcmpi(Edit1->Text.c_str(), Edit2->Text.c_str()) == 0) {
    ShowMessage("Èìåíà îáðàáàòûâàåìîãî ôàéëà è ôàéëà íàçíà÷åíèÿ ñîâïàäàþò!");
    return;
  }

  if (strcmpi(Edit1->Text.c_str(), Memo1->Text.c_str()) == 0) {
    ShowMessage("Èìåíà îáðàáàòûâàåìîãî ôàéëà è êëþ÷åâîãî ôàéëà ñîâïàäàþò!");
    return;
  }

  if (strcmpi(Edit2->Text.c_str(), Memo1->Text.c_str()) == 0) {
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

  key_len = (key_len / 8);

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
    ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");
    return;
  }

  int ctx_len;
  real_read = (short int)readfromfile(Memo1->Text.c_str(), buffer, key_len);

  if (real_read > 0 && real_read < key_len) {
    ShowMessage("Äàííûõ â êëþ÷åâîì ôàéëå ñëèøêîì ìàëî!\n\n"
                "Áûëî ñ÷èòàíî: " + IntToStr(real_read) + " " + CHAR_SIZE_DATA[0] + "\n" +
                "Íåîáõîäèìî: " + IntToStr(key_len) + " " + CHAR_SIZE_DATA[0]);
    free(buffer);
    buffer = NULL;
    return;
  }
  else
  if (real_read == 0 || real_read == -1) {
    real_read = strlen(Memo1->Text.c_str());
    if (real_read > 7 && real_read < 2049) {
      ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = (SHA256_CTX*) calloc(1, ctx_len);

      if (sha256_ctx != NULL) {
        sha256sum(sha256_ctx, Memo1->Text.c_str(), real_read);
        hash_init(buffer, sha256_ctx->hash, key_len);

        memset(sha256_ctx, 0x00, ctx_len);
        free(sha256_ctx);
        sha256_ctx = NULL;
      }
      else {
        ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");

        memset(buffer, 0x00, key_len);
        free(buffer);
        buffer = NULL;
      }
    }
    else {
      ShowMessage("Äëèíà ñòðîêîâîãî êëþ÷à íåêîððåêòíà!\n\n"
                  "Áûëî ñ÷èòàíî: " + IntToStr(real_read) + " " + CHAR_SIZE_DATA[0] + "\n" +
                  "Íåîáõîäèìî: îò 8 äî 2048 " + CHAR_SIZE_DATA[0]);

      memset(buffer, 0x00, key_len);
      free(buffer);
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
      ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");

      memset(buffer, 0x00, key_len);
      free(buffer);
      buffer = NULL;

      return;
    }
  }

  if (cipher_number != ARC4 && operation == ENCRYPT) {

    srand(time(NULL));

    for (i = 0; i < block_size; i++)
      vector[i] = (uint8_t)genrand(0, 255);

    for (i = 0; i < (block_size - 2); i++) {
      if (vector[i] == vector[i + 1] && vector[i + 1] == vector[i + 2])
        break;
    }

    if (i < (block_size - 2)) {
      ShowMessage("Êðèòè÷åñêàÿ îøèáêà! Ñèñòåìíîå âðåìÿ îñòàíîâëåíî!\n"
                  "Äàëüíåéøèå îïåðàöèè íå ïîçâîëåíû!");

      memset(vector, 0x00, block_size);
      memset(buffer, 0x00, key_len);

      free(buffer);
      free(vector);

      buffer = NULL;
      vector = NULL;

      Form1->Close();
    }
  }

  if (cipher_number == ARC4) {
    arc4_init(buffer, key_len);
  }
  if (cipher_number == AES) {
    ctx_len = Nb * (Nr + 1) * 4;
    rijndael_ctx = (uint8_t *) calloc(ctx_len, 1);
    if (rijndael_ctx == NULL) {
      ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");

      memset(vector, 0x00, block_size);
      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

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
      ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");

      memset(vector, 0x00, block_size);
      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

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
      ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");

      memset(vector, 0x00, block_size);
      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

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
      ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");

      memset(vector, 0x00, block_size);
      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

      vector = NULL;
      buffer = NULL;

      return;
    }
    threefish_init(threefish_ctx, (uint64_t*)buffer, (uint64_t*)buffer);
  }

  memset(buffer, 0x00, key_len);
  free(buffer);
  buffer = NULL;

  int result = 0xDE;

  if (MessageDlg("Ïðèñòóïèòü ê âûáðàííîé îïåðàöèè? Îòìåíèòü îïåðàöèþ áóäåò íåâîçìîæíî!\n\n"
                 "Îïåðàöèÿ: " + AnsiString(OPERATION_NAME[cipher_number ? (operation ? 1 : 0) : 2]) + "\n"
                 "Àëãîðèòì: " + AnsiString(ALGORITM_NAME[cipher_number]) + "\n"
                 "Äëèíà êëþ÷à: " + IntToStr(key_len * 8), mtCustom, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes) {

    Button4->Enabled = false;
    result = filecrypt(Edit1->Text.c_str(), Edit2->Text.c_str(), vector, block_size, cipher_number, operation);
  }

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
    case -4:    ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");
                break;
    case -5:    ShowMessage("Îøèáêà çàïèñè â ôàéë!");
                break;
    case -6:    ShowMessage("Îøèáêà ÷òåíèÿ èç ôàéëà!");
                break;
  }

  if (result == 0 && CheckBox1->Checked == True) {
    if (MessageDlg("Âû óâåðåíû ÷òî õîòèòå óíè÷òîæèòü ôàéë äëÿ îáðàáîòêè?\n"
                   "Ñòåðòûå äàííûå íåâîçìîæíî áóäåò âîññòàíîâèòü!", mtWarning, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes) {

      if ((erasedfile(Edit1->Text.c_str()) == 0) && (DeleteFile(Edit1->Text) == True)) {
        ShowMessage("Ôàéë áûë óíè÷òîæåí!");
      }
      else
        ShowMessage("Îøèáêà óíè÷òîæåíèÿ ôàéëà!");
    }
  }

  Form1->Shape2->Width = 0;
  Application->ProcessMessages();

  switch (cipher_number) {
    case ARC4:      memset(secret_key, 0x00, key_len);
                    break;

    case AES:       memset(rijndael_ctx, 0x00, ctx_len);
                    free(rijndael_ctx);
                    rijndael_ctx = NULL;
                    break;

    case SERPENT:   memset(serpent_ctx, 0x00, ctx_len);
                    free(serpent_ctx);
                    serpent_ctx = NULL;
                    break;

    case BLOWFISH:  memset(blowfish_ctx, 0x00, ctx_len);
                    free(blowfish_ctx);
                    blowfish_ctx = NULL;
                    break;

    case THREEFISH: memset(threefish_ctx, 0x00, ctx_len);
                    free(threefish_ctx);
                    threefish_ctx = NULL;
                    break;

  }

  if (vector != NULL) {
    memset(vector, 0x00, block_size);
    free(vector);
    vector = NULL;
  }

  Button4->Enabled = true;
}

void __fastcall TForm1::Label5Click(TObject *Sender) {
  Form1->Close();
}

void __fastcall TForm1::Panel1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y) {
  ReleaseCapture();
  Form1->Perform(WM_SYSCOMMAND, 0xF012, 0);

}

void __fastcall TForm1::Label7Click(TObject *Sender) {
 Form2->Panel1->Color   = FORM_HEAD_COLOR;
 Form2->Label6->Caption = PROGRAMM_NAME;
 Form2->Show();
}

void __fastcall TForm1::Button5Click(TObject *Sender) {
  int len = atoi(Edit3->Text.c_str());

  if (len < 8 || len > 256) {
    ShowMessage("Ââåäèòå ÷èñëî îò 8 äî 256!");
    return;
  }

  int ctx_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = (MEMORY_CTX *) calloc(1, ctx_len);

  if (memory == NULL) {
    ShowMessage("Íåäîñòàòî÷íî ïàìÿòè!");
    return;
  }

  srand(time(NULL));

  for (int i = 0; i < len; i++) {
    memory->data[i] = (uint8_t)genrand(0, 255);
  }

  arc4_init(memory->data, len);
  arc4(memory->data, memory->post, len);

  memset(memory->data, 0x00, len);

  base64encode(memory->post, memory->data, len);
  memory->data[len] = 0x00;

  Memo1->Clear();
  Memo1->Lines->Text = AnsiString((char*)memory->data);

  memset(secret_key, 0x00, 256);
  memset(memory, 0x00, ctx_len);

  free(memory);
  memory = NULL;
}

