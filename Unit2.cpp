#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TForm2 *Form2;

__fastcall TForm2::TForm2(TComponent* Owner): TForm(Owner) {
}

void __fastcall TForm2::Label5Click(TObject *Sender) {
  Form2->Close();        
}



void __fastcall TForm2::Shape1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y) {
  ReleaseCapture();
  Form2->Perform(WM_SYSCOMMAND, 0xF012, 0);        
}
//---------------------------------------------------------------------------




void __fastcall TForm2::Label5MouseEnter(TObject *Sender)
{
  Label5->Font->Color = clRed;        
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Label5MouseLeave(TObject *Sender)
{
  Label5->Font->Color = clWhite;        
}
//---------------------------------------------------------------------------


