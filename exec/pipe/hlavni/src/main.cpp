#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "howindow.h"
#include "hopushbutton.h"
#include "hoedit.h"
#include "hoapp.h"
#include "hodc.h"
#include "hostring.h"
#include "holabel.h"
#include "hoexec.h"
#include "homsgbox.h"

HoEdit *e;
HoEdit *pip;

class MojeOkno: public HoWindow {
  public:
    MojeOkno(HoWindow *parent, const HoString &title):
    HoWindow(parent, title){};
};

void push(HoPushButton *b, void *param) {
  char* arg[2];
  arg[0] = e->getText().toString();
  arg[1] = NULL;
  puts(arg[0]);
  HoPipe *p = hoExecWithPipe(arg[0], arg);
  free(arg[0]);
  
if (!p) {
/*

	  LPVOID lpMsgBuf;
FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM | 
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &lpMsgBuf,
    0,
    NULL 
);
// Process any inserts in lpMsgBuf.
// ...
// Display the string.
MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
// Free the buffer.
LocalFree( lpMsgBuf );

*/

     HoMsgBox::msgBox("Chyba", "Chyba");
     return;
  }

  char s[50];
  memset(s, 0, sizeof(s));
  p->read(5, (void *) s);
  pip->setText(s);
}

HoWindow *hlavni;
int homain(int argc, char * argv[]){
  HoApp *a;

  a = new HoApp(argc, argv);
  hlavni = new MojeOkno(NULL, HoString("Spouštìè s pípou", 1));
  e = new HoEdit(hlavni, 
#if 0
"C:\\honza\\c\\hlib2\\exec\\pipe\\spusteny\\Debug\\exec_spusteny.exe"
#else
"/home/honza/c/hlib2/exec/pipe/spusteny/spusteny"
#endif
, 100);
  pip = new HoEdit(hlavni, "", 100);
  
  new HoPushButton(hlavni, "Exec", push, hlavni);
  hlavni->pack();
  hlavni->create();
  a->run();
  delete a;
  return 0;
}

