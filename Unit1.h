//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Crycon : public TThread {
  typedef struct tagTHREADNAME_INFO {
    DWORD  dwType;      // must be 0x1000
    LPCSTR szName;      // pointer to name (in user addr space)
    DWORD  dwThreadID;  // thread ID (-1=caller thread)
    DWORD  dwFlags;     // reserved for future use, must be zero
  } THREADNAME_INFO;

private:
  void SetName();

protected:
  void __fastcall Execute();

public:
  __fastcall Crycon(bool CreateSuspended);

};
//---------------------------------------------------------------------------
class TForm1 : public TForm {
__published:	// IDE-managed Components
        TShape *Shape2;
        TLabel *Label7;
        TLabel *Label5;
        TLabel *Label6;
        TShape *Shape1;
        TLabel *Label1;
        TLabel *Label4;
        TLabel *Label3;
        TLabel *Label2;
        TLabel *Label8;
        TLabel *Label9;
        TComboBox *ComboBox1;
        TComboBox *ComboBox2;
        TEdit *Edit2;
        TEdit *Edit1;
        TButton *Button1;
        TButton *Button2;
        TCheckBox *CheckBox1;
        TRadioButton *RadioButton2;
        TRadioButton *RadioButton1;
        TEdit *Edit3;
        TButton *Button5;
        TMemo *Memo1;
        TButton *Button3;
        TButton *Button4;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TProgressBar *ProgressBar1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Label5Click(TObject *Sender);
        void __fastcall Label7Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Shape2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label5MouseEnter(TObject *Sender);
        void __fastcall Label5MouseLeave(TObject *Sender);
        void __fastcall Label7MouseLeave(TObject *Sender);
        void __fastcall Label7MouseEnter(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        CRITICAL_SECTION CrSec;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
