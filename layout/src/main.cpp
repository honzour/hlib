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
#include <holabel.h>
#include <hosocket.h>
#include <hoimage.h>

HoApp *a;


HoWindow *hlavniOkno;
int fx, fy;

void push(HoPushButton *, void *) {
  HoPushButton *button =
    new HoPushButton(hlavniOkno, HoString("Pøidej", 1),
                     fx * 20, (fy * 5) / 2, push);
  button->create();
  hlavniOkno->pack();
}

void pop(HoPushButton *, void *) {
  HoWindow *m = (HoWindow *) hlavniOkno->windowTree->getMinKey();
  m->destroy();
  hlavniOkno->pack();
}

int homain(int argc,char * argv[]){
 
  a = new HoApp(argc, argv);
  a->getDefaultFontSize(&fx, &fy);
  hlavniOkno =
    new HoWindow(NULL, HoString("Layout", 1),
      50, 50, 700, 520, HoWindow::styleSizing | HoWindow::styleCaption);
  new HoLabel(hlavniOkno, " Klikni na to", fx * 20, (fy * 3) / 2);
  new HoPushButton(hlavniOkno, HoString("Pøidej", 1),
                     fx * 20, (fy * 5) / 2, push);
  new HoPushButton(hlavniOkno, HoString("Uber", 1),
                     fx * 20, (fy * 5) / 2, pop);
  hlavniOkno->pack();
  hlavniOkno->create();
  a->run();
  delete a;
  return 0;
}

