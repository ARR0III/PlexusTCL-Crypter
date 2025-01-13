//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit2.cpp", Form2);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
  unsigned int trash[2];
  
  srand((trash[0] ^ (unsigned int)time(NULL)) + trash[1]);
	
  try {
    Application->Initialize();
    Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->Run();
  }
  catch (Exception &exception) {
    Application->ShowException(&exception);
  }
  
  catch (...) {
    try {
      throw Exception("");
    }
    catch (Exception &exception) {
      Application->ShowException(&exception);
    }
  }
  return 0;
}
//---------------------------------------------------------------------------
