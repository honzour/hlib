#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <howindow.h>
#include <hopushbutton.h>
#include <hoapp.h>
#include <hostring.h>
#include <holabel.h>
#include <hothread.h>

HoApp *a;


HoWindow *hlavniOkno;
int fx, fy;

static void onEvent(void *param) {
  hlavniOkno->setText(HoString("Došla zpráva", 1));
}

static void vlakno(void *param) {
  HoThread::sendEvent((void *)17);
}

static void push(HoPushButton *, void *) {
  HoThread t(vlakno, NULL);
  t.run();
}

int homain(int argc,char * argv[]){
 
  a = new HoApp(argc, argv);
  a->userEvent = onEvent;
  a->getDefaultFontSize(&fx, &fy);
  hlavniOkno =
    new HoWindow(NULL, HoString("Vlákno", 1),
      50, 50, 700, 520, HoWindow::styleSizing | HoWindow::styleCaption);
  new HoLabel(hlavniOkno, " Klikni na to", fx * 20, (fy * 3) / 2);
  new HoPushButton(hlavniOkno, HoString("Vlákno", 1),
                     fx * 20, (fy * 5) / 2, push);
  hlavniOkno->pack();
  hlavniOkno->create();
  a->run();
  delete a;
  return 0;
}

