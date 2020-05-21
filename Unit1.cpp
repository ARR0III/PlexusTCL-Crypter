#include <io.h>
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

const float cas = 4.87; /* ((float)Form1->Shape4->Width / (float)100) or (488 ��� / 100) */

const TColor FORM_HEAD_COLOR = TColor(0x00623E00);

const uint32_t INT_SIZE_DATA[] = {1024, 1048576, 1073741824};

const char * CHAR_SIZE_DATA[] = {"��", "��", "��", "��"};

const char * OPERATION_NAME[] = {"����������", "�����������", "��������� ���������"};

const char * ALGORITM_NAME[] =  {"ARC4", "AES-CFB", "SERPENT-CFB",
                                 "BLOWFISH-CFB", "THREEFISH-512-CFB"};

const char * PROGRAMM_NAME = "PlexusTCL Crypter 4.46 21MAY20 [RU]";

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
  OpenDialog1->Title = "���� ��� ���������";
  if (OpenDialog1->Execute()) {
    Edit1->Clear();
    Edit1->Text = OpenDialog1->FileName;
  }
}

void __fastcall TForm1::Button2Click(TObject *Sender) {
  SaveDialog1->Title = "���� ����������";
  if (SaveDialog1->Execute()) {
    Edit2->Clear();
    Edit2->Text = SaveDialog1->FileName;
  }
}

void __fastcall TForm1::Button3Click(TObject *Sender) {
  OpenDialog1->Title = "���� ����������";
  if (OpenDialog1->Execute()) {
    Memo1->Clear();
    Memo1->Lines->Text = OpenDialog1->FileName;
  }
}

void __fastcall TForm1::FormCreate(TObject *Sender) {
  srand((uint32_t)time(NULL));

  for (int i = 0; i < 5; i++)
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

  data[0] ^= (uint8_t)position.x;
  data[1] ^= (uint8_t)position.y;

  position.x = position.y = 0;
}

void password_to_key(SHA256_CTX * sha256_ctx,
                     const uint8_t * password, const uint32_t password_len,
                     uint8_t * key, const uint32_t key_len) {

  uint32_t i, j, k;
  uint32_t count = key_len + (password_len * 2) - 1;

  uint8_t  hash[SHA256_BLOCK_SIZE];

  sha256_init(sha256_ctx);

  for (i = k = 0; i < key_len; ++i, ++k) {
    for (j = 0; j < count; ++j) {
      sha256_update(sha256_ctx, password, password_len);
      sha256_final(sha256_ctx, hash);
    }

    if (k == SHA256_BLOCK_SIZE)
      k = 0;

    /* This is good idea ???
       password[0] ^= hash[0] ^ hash[k]; */

    key[i] = hash[k];
  }

  memset(hash, 0x00, SHA256_BLOCK_SIZE);
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

void centreal(int * real) {
  if (*real > 100) {
    *real = 100;
  }
}

long int file_of_size(FILE * fpointer) {
  long int result;

  fseek(fpointer, 0, SEEK_END);
  result = ftell(fpointer);
  fseek(fpointer, 0, SEEK_SET);

  return result;
}

int erasedfile(uint8_t * filename) {
  FILE * f = fopen(filename, "r+b");

  if (f == NULL) {
    return -1;
  }

  long int fsize = file_of_size(f);
  long int position = 0;

  if (fsize <= 0) {
    fclose(f);
    return -1;
  }

  uint8_t * data = NULL;
            data = (uint8_t *)calloc(BLOCK_SIZE, 1);

  if (data == NULL) {
    fclose(f);
    return -1;
  }

  int real = 0;
  int past = 0;

  float div = (float)fsize / 100.0;
  int   check;

  size_t realread;

  while (position < fsize) {
    realread = fread(data, 1, BLOCK_SIZE, f);

    if (realread > 0) {
      memset(data, 0x00, realread);
      fseek(f, position, SEEK_SET);

      if (fwrite(data, 1, realread, f) != realread) {
        fclose(f);
        free(data);
        return -1;
      }
      else
        fflush(f);

      position += realread;
    }

    real = (int)((float)position / div + 0.1);

    centreal(&real);

    if (real > past) {
      if ((real % 4) == 0) {
        Form1->Shape4->Width = (int)((float)real * cas) + 1;
        check = size_check(position);

        Form1->Label9->Caption = "����������� �����; ����������: " +
        (check ? FloatToStrF(((float)position / (float)INT_SIZE_DATA[check - 1]), ffFixed, 4, 2) :
                 IntToStr(position)) +
        " " + CHAR_SIZE_DATA[check] + "; ��������: " + IntToStr(real) + " %" ;

        Application->ProcessMessages();
      }

      past = real;
    }
  }

  free(data);

  check = chsize(fileno(f), 0);
  fclose(f);

  if (check != 0) {
    return -1;
  }
  else
    return 0;
}

int filecrypt(const uint8_t * finput, const uint8_t * foutput, uint8_t * vector,
              size_t block_size, int cipher, int operation) {

  FILE * fi = fopen(finput, "rb");

  if (fi == NULL) {
    return -1;
  }

  long int fsize = file_of_size(fi);
  long int position = 0;

  if ((fsize == -1L) || (fsize == 0)) {
    fclose(fi);
    return -3;
  }

  FILE * fo = fopen(foutput, "wb");

  if (fo == NULL) {
    fclose(fi);
    return -2;
  }

  int memory_ctx_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = NULL;
               memory = (MEMORY_CTX *) calloc(1, memory_ctx_len);

  if (memory == NULL) {
    fclose(fi);
    fclose(fo);
    return -4;
  }

  float div = (float)fsize / 100.0;

  int check;

  size_t realread;
  size_t nblock;

  int real = 0;
  int past = 0;

  Form1->Shape4->Width = 0;

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

                        return -5;
                      }
                      else {
                        fflush(fo);
                      }
                      break;
        case DECRYPT: if (fread(vector, 1, block_size, fi) != block_size) {
                        fclose(fi);
                        fclose(fo);

                        free(memory);

                        return -6;
                      }
                      position += block_size;
                      break;
      }
    }

    realread = fread(memory->data, 1, DATA_SIZE, fi);

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

      return -5;
    }
    else {
      fflush(fo);
    }

    position += realread;
    real = (int)((float)position / div + 0.1);

    centreal(&real);

    if (real > past) {
      if ((real % 4) == 0) {
        Form1->Shape4->Width = (int)((float)real * cas) + 1;

        check = size_check(position);

        Form1->Label9->Caption = AnsiString(OPERATION_NAME[cipher ? (operation ? 1 : 0) : 2]) +
        ": " + AnsiString(ALGORITM_NAME[cipher]) + "; ����������: " +
        (check ? FloatToStrF(((float)position / (float)INT_SIZE_DATA[check - 1]), ffFixed, 4, 2) :
                 IntToStr(position)) +
        " " + CHAR_SIZE_DATA[check] + "; ��������: " + IntToStr(real) + " %" ;

        Application->ProcessMessages();
      }

      past = real;
    }
  }

  fclose(fi);
  fclose(fo);

  memset(memory, 0x00, memory_ctx_len);
  free(memory);

  return 0;
}

short vector_init(uint8_t * data, short size) {
  short i;

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
  int cipher_number, operation, ctx_len,
      key_len, real_read, block_size, i;

  if (strlen(Edit1->Text.c_str()) == 0) {
    ShowMessage("��� ��������������� ����� �� �������!");
    return;
  }

  if (strlen(Edit2->Text.c_str()) == 0) {
    ShowMessage("��� ����� ���������� �� �������!");
    return;
  }

  if (strlen(Memo1->Text.c_str()) == 0) {
    ShowMessage("��� ��������� ����� �� �������!");
    return;
  }

  if (Edit1->Text == Edit2->Text) {
    ShowMessage("����� ��������������� ����� � ����� ���������� ���������!");
    return;
  }

  if (Edit1->Text == Memo1->Text) {
    ShowMessage("����� ��������������� ����� � ��������� ����� ���������!");
    return;
  }

  if (Edit2->Text == Memo1->Text) {
    ShowMessage("����� ����� ���������� � ��������� ����� ���������!");
    return;
  }

  if (FileExists(Edit2->Text) == True) {
    if (MessageDlg("���� ���������� ����������! ������ ������ ����� �������!\n"
                   "�� ������� ��� ������ ������������ ���?", mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo) {

      ShowMessage("�������� ��� ����� ����������!");
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
    ShowMessage("�������� ���������� �� ��� ������!");
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
      ShowMessage("����� ����� ���������� �� �������!");
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
      ShowMessage("�������� �� ���� �������!");
      return;
    }
  }

  uint8_t * buffer = NULL;
            buffer = (uint8_t*)calloc(key_len, 1);

  if (buffer == NULL) {
    ShowMessage("������������ ������!");
    return;
  }

  real_read = readfromfile(Memo1->Text.c_str(), buffer, key_len);

  if (real_read > 0 && real_read < key_len) {
    ShowMessage("������ � �������� ����� ������� ����!\n\n"
                "���� �������: " + IntToStr(real_read) + " " + CHAR_SIZE_DATA[0] + "\n" +
                "����������: " + IntToStr(key_len) + " " + CHAR_SIZE_DATA[0]);
    free(buffer);
    return;
  }
  else
  if (real_read == 0 || real_read == -1) {
    real_read = strlen(Memo1->Text.c_str());
    if (real_read > 7 && real_read < 257) {
      ctx_len = sizeof(SHA256_CTX);
      SHA256_CTX * sha256_ctx = NULL;
                   sha256_ctx = (SHA256_CTX*) calloc(1, ctx_len);

      if (sha256_ctx != NULL) {
        password_to_key(sha256_ctx, (uint8_t *)Memo1->Text.c_str(), real_read, buffer, key_len);

        memset(sha256_ctx, 0x00, ctx_len);
        free(sha256_ctx);
      }
      else {
        ShowMessage("������������ ������!");

        memset(buffer, 0x00, key_len);
        free(buffer);
        return;
      }
    }
    else {
      ShowMessage("����� ���������� ����� �����������!\n\n"
                  "���� �������: " + IntToStr(real_read) + " " + CHAR_SIZE_DATA[0] + "\n" +
                  "����������: �� 8 �� 256 " + CHAR_SIZE_DATA[0]);

      memset(buffer, 0x00, key_len);
      free(buffer);
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
      ShowMessage("������������ ������!");

      memset(buffer, 0x00, key_len);
      free(buffer);

      return;
    }
  }

  if (cipher_number != ARC4 && operation == ENCRYPT) {

    srand((uint32_t)time(NULL));

    if (vector_init(vector, block_size) < (block_size - 2)) {
      ShowMessage("����������� ������ ����! ���������� �������� �� ���������!");

      memset(vector, 0x00, block_size);
      memset(buffer, 0x00, key_len);

      free(buffer);
      free(vector);

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
      ShowMessage("������������ ������!");

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

      return;
    }

    rijndael_init(buffer, rijndael_ctx);
  }
  if (cipher_number == SERPENT) {
    ctx_len = sizeof(SERPENT_CTX);
    serpent_ctx = (SERPENT_CTX *) calloc(1, ctx_len);
    if (serpent_ctx == NULL) {
      ShowMessage("������������ ������!");

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

      return;
    }
    serpent_init(serpent_ctx, key_len * 8, buffer);
  }
  else
  if (cipher_number == BLOWFISH) {
    ctx_len = sizeof(BLOWFISH_CTX);
    blowfish_ctx = (BLOWFISH_CTX*)calloc(1, ctx_len);
    if (blowfish_ctx == NULL) {
      ShowMessage("������������ ������!");

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

      return;
    }
    blowfish_init(blowfish_ctx, buffer, key_len);
  }
  else
  if (cipher_number == THREEFISH) {
    ctx_len = sizeof(THREEFISH_CTX);
    threefish_ctx = (THREEFISH_CTX *)calloc(1, ctx_len);
    if (threefish_ctx == NULL) {
      ShowMessage("������������ ������!");

      if (operation == ENCRYPT)
        memset(vector, 0x00, block_size);

      memset(buffer, 0x00, key_len);

      free(vector);
      free(buffer);

      return;
    }
    threefish_init(threefish_ctx, (uint64_t*)buffer, (uint64_t*)buffer);
  }

  memset(buffer, 0x00, key_len);
  free(buffer);

  int result = 0xDE;

  if (MessageDlg("���������� � ��������� ��������? �������� �������� ����� ����������!\n\n"
                 "��������: " + AnsiString(OPERATION_NAME[cipher_number ? (operation ? 1 : 0) : 2]) + "\n"
                 "��������: " + AnsiString(ALGORITM_NAME[cipher_number]) + "\n"
                 "����� �����: " + IntToStr(key_len * 8), mtCustom, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes) {

    Button4->Enabled = false;
    result = filecrypt(Edit1->Text.c_str(), Edit2->Text.c_str(), vector, block_size, cipher_number, operation);
  }

  switch (result) {
    case  0xDE: ShowMessage("�������� ���� ��������!");
                break;

    case  0:    ShowMessage("���� ������� ���������!");
                break;
    case -1:    ShowMessage("���� ��� ��������� �� ��� ������!");
                break;
    case -2:    ShowMessage("���� ���������� �� ��� ������!");
                break;
    case -3:    ShowMessage("���� ��� ��������� ���� ��� ��� ������ ��������� 2 ��!");
                break;
    case -4:    ShowMessage("������������ ������!");
                break;
    case -5:    ShowMessage("������ ������ � ����!");
                break;
    case -6:    ShowMessage("������ ������ �� �����!");
                break;
  }

  if (result == 0 && CheckBox1->Checked == True) {
    if (MessageDlg("�� ������� ��� ������ ���������� ���� ��� ���������?\n"
                   "������� ������ ���������� ����� ������������!", mtWarning, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes) {

      if ((erasedfile(Edit1->Text.c_str()) == 0) && (DeleteFile(Edit1->Text) == True)) {
        ShowMessage("���� ��� ���������!");
      }
      else
        ShowMessage("������ ����������� �����!");
    }
  }

  Form1->Shape4->Width = 0;
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

  if (len < 8 || len > 256) {
    ShowMessage("������� ����� �� 8 �� 256!");
    return;
  }

  int memory_ctx_len = sizeof(MEMORY_CTX);
  MEMORY_CTX * memory = NULL;
               memory = (MEMORY_CTX *)calloc(1, memory_ctx_len);

  if (memory == NULL) {
    ShowMessage("������������ ������!");
    return;
  }

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
  memset(memory, 0x00, memory_ctx_len);

  free(memory);
}

void __fastcall TForm1::Shape2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y) {
  ReleaseCapture();
  Form1->Perform(WM_SYSCOMMAND, 0xF012, 0);
}
