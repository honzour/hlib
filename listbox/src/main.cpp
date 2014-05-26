#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <howindow.h>
#include <hopushbutton.h>
#include <hoedit.h>
#include <hoapp.h>
#include <hodc.h>
#include <hostring.h>
#include <holistbox.h>
#include <hosocket.h>

HoApp *a;

HoWindow *hlavniOkno;
int fx, fy;

void change(int i) {
  char s[256];

  sprintf(s, "Oznaèeno %i", i);
  hlavniOkno->setText(s);

}

HoListbox *l; 

void pridej(HoPushButton *, void *) {
  l->addString("pøidáno");
}

void uber(HoPushButton *, void *) {
  l->removeString(1);
}


int homain(int argc,char * argv[]){
 
  a = new HoApp(argc, argv);
  a->getDefaultFontSize(&fx, &fy);
  hlavniOkno =
    new HoWindow(NULL, HoString("Listbox", 1),
      50, 50, 700, 520, HoWindow::styleSizing | HoWindow::styleCaption);
  l = new HoListbox(hlavniOkno, fx * 20, 5 * ((fy * 3) / 2), change);
  l->addString("kokos");
  l->addString("fikus");
  l->addString("jukos");

  new HoPushButton(hlavniOkno, "Pøidej", pridej);
  new HoPushButton(hlavniOkno, "Uber", uber);

  hlavniOkno->pack();
  hlavniOkno->create();
  a->run();
  delete a;
  return 0;
}

