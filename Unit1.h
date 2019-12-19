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
        TShape *Shape3;
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
        TShape *Shape4;
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
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
