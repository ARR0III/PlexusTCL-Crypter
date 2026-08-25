//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TShape *Shape1;
        TLabel *Label5;
        TLabel *Label4;
        TShape *Shape2;
        TLabel *Label3;
        TBevel *Bevel1;
        TLabel *Label2;
        TLabel *Label1;
        void __fastcall Label5Click(TObject *Sender);
        void __fastcall Shape1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label5MouseEnter(TObject *Sender);
        void __fastcall Label5MouseLeave(TObject *Sender);
        void __fastcall Label4MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
