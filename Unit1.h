//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>

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
  KEY_SIZE         = 1,
  OPERATION        = 2,
  PASS_GEN_SIZE    = 3,
  TOP_COLOR        = 4,
  TOP_TEXT_B_COLOR = 5,
  TOP_TEXT_COLOR   = 6
} config_key;

typedef struct {
  cipher_t cipher;

  TColor   top_color;
  TColor   top_text_color;
  TColor   top_text_b_color;
  TColor   prog_bar_color;

  int  key_size;
  int  pass_gen_size;
  int  operation;
} SETTINGS;

typedef struct {
  uint8_t input  [SIZE_PASSWORD_GENERATE];
  uint8_t output [SIZE_PASSWORD_GENERATE];
} PASSWORD_MEMORY_CTX;

typedef struct {
  uint8_t ipad[HMAC_DATA_BUFFER];
  uint8_t opad[HMAC_DATA_BUFFER];
  uint8_t temp[SHA256_BLOCK_SIZE];
} HMAC_CTX;

typedef struct {
  FILE       * file_input;
  FILE       * file_output;

  uint8_t    * hash_matrix;
  size_t       hash_matrix_length;

  uint8_t    * vector;             /* initialized vector for crypt data */
  size_t       vector_length;      /* block size cipher execution */

  uint8_t    * real_key;           /* real_key for temp key data */
  size_t       real_key_length;    /* size buffer for crypt key */

  uint8_t    * new_key;            /* real_key for temp key data */
  size_t       new_key_length;     /* size buffer for crypt key */

  int          operation;          /* ENCRYPT == 0x00 or DECRYPT == 0xDE */
  cipher_t     cipher;             /* search type name cipher_number_enum */  

  unsigned char pass_salt[PASS_SALT_SIZE];
  unsigned char plain_text[DATA_SIZE];
  unsigned char cipher_text[DATA_SIZE];

  char status_buffer[STATUS_BUFFER_SIZE];

  SHA256_CTX   sha256sum[sizeof(SHA256_CTX)];          /* memory for sha256 hash function */
  HMAC_CTX     hmac_data;
} GLOBAL_MEMORY;

//---------------------------------------------------------------------------
class Crycon : public TThread {
  typedef struct tagTHREADNAME_INFO {
    DWORD  dwType;      // must be 0x1000
    LPCSTR szName;      // pointer to name (in user addr space)
    DWORD  dwThreadID;  // thread ID (-1=caller thread)
    DWORD  dwFlags;     // reserved for future use, must be zero
  } THREADNAME_INFO;

private:
  GLOBAL_MEMORY * ctx;
  
  void * cipher_ptr;
  size_t cipher_length;
  
  fsize_t fsize, position;
  double div, fsize_double;

  int status;
  int check,
      fsize_check,
      status_buffer_pos,
      real_percent,
      past_percent;

  size_t nblock, realread, re_keying;
   
  void __fastcall Init();
  void __fastcall Clear();

  int __fastcall WriteSalt();
  int __fastcall ReadSalt();

  void __fastcall ShowStatus();
  void __fastcall ProgressBar();
  void __fastcall ButtonUpdate();

  void __fastcall Encrypt();
  
  void __fastcall InternalReKeying();
  void __fastcall HashCalculate(const size_t size);
  void __fastcall HMAC_Calculate();
  bool __fastcall HMAC_Check(const size_t pos);
protected:
  void __fastcall Execute();

public:
  __fastcall  Crycon(bool CreateSuspended);
  __fastcall ~Crycon();

  bool __fastcall SetInputFile(const char * name);
  bool __fastcall SetOutputFile(const char * name);
  int  __fastcall PassSaltInit();

  bool __fastcall GeneratingCryptKey(const char * message);

  bool __fastcall SetCipher(const char * cipher_name);
  int  __fastcall SetKeyLen(const char * key_size, int * aes);
  void __fastcall SetOperation(const int operation);
  void __fastcall SetErased();
  bool __fastcall GenKeyFromFile(const char * key_file);
  bool __fastcall KeyGenFromPass(const char * password);  
  void __fastcall InitCipherKey();
  
  bool __fastcall CorrectSizeOfFile(const char * name);

  void __fastcall SetStatus(const int x);
  
  FILE *   __fastcall GetFile();
  fsize_t  __fastcall GetFileSize();
  int      __fastcall GetStatus();
  size_t   __fastcall GetKeySize();
  int      __fastcall GetOperation();
  cipher_t __fastcall GetCipher();
};

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TShape *Shape1;
        TShape *Shape2;
        TComboBox *CipherBox;
        TLabel *Label1;
        TLabel *Label2;
        TComboBox *KeyLenBox;
        TLabel *Label3;
        TEdit *InputFile;
        TEdit *OutputFile;
        TLabel *Label4;
        TLabel *Label5;
        TButton *ButtonInput;
        TButton *ButtonOutput;
        TButton *ButtonKeyGen;
        TEdit *PassLen;
        TRadioButton *RadioEncrypt;
        TRadioButton *RadioDecrypt;
        TLabel *Label6;
        TMemo *MemoKey;
        TButton *ButtonKey;
        TButton *ButtonStart;
        TProgressBar *ProgressBar1;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        void __fastcall Label9Click(TObject *Sender);
        void __fastcall ButtonStartClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ButtonInputClick(TObject *Sender);
        void __fastcall ButtonOutputClick(TObject *Sender);
        void __fastcall ButtonKeyClick(TObject *Sender);
        void __fastcall Shape1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall CipherBoxChange(TObject *Sender);
        void __fastcall ButtonKeyGenClick(TObject *Sender);
        void __fastcall Label8Click(TObject *Sender);
        void __fastcall Label8MouseEnter(TObject *Sender);
        void __fastcall Label8MouseLeave(TObject *Sender);
        void __fastcall Label9MouseEnter(TObject *Sender);
        void __fastcall Label9MouseLeave(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall  TForm1(TComponent* Owner);
  CRITICAL_SECTION CrSec;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
