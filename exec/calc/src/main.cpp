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

class MojeOkno: public HoWindow {
  public:
    MojeOkno(HoWindow *parent, const HoString &title):
    HoWindow(parent, title){};
};

void push(HoPushButton *b, void *param) {
  char* arg[2];
  arg[0] = e->getText().toString();
  arg[1] = NULL;
  if (!hoExec(arg[0], arg)) {
    HoMsgBox::msgBox("Chyba", "Nepodaøilo se spustit program");
  }
  free(arg[0]);
}

HoWindow *hlavni;
int homain(int argc, char * argv[]){
  HoApp *a;

  a = new HoApp(argc, argv);
  hlavni = new MojeOkno(NULL, HoString("Spou¹tìè", 0));
  e = new HoEdit(hlavni, /*"kcalc"*/ "C:\\windows\\system32\\calc.exe", 50);
  new HoPushButton(hlavni, "Exec", push, hlavni);
  hlavni->pack();
  hlavni->create();
  a->run();
  delete a;
  return 0;
}

