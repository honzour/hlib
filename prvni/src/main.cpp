#include <stdio.h>
#include "howindow.h"
#include "hopushbutton.h"
#include "hoedit.h"
#include "hoapp.h"
#include "hodc.h"
#include "hostring.h"

class MojeOkno: public HoWindow{
  public:
    MojeOkno(HoWindow *parent, const HoString &title):
    HoWindow(parent, title){};
    virtual void paint(HoDC *dc);
    virtual int create();
};

void MojeOkno::paint(HoDC *dc){
  int i;

  for (i = 0; i < 5; i++)
  dc->fillRectangle(10 + 20 * i, 10 + 20 * i, 100, 100);
  dc->setForeground(0xFFFF, 0xFFFF, 255);
  dc->fillEllipse(50, 50, 100, 100);
  dc->setForeground(0, 0xFFFF, 0);
  dc->drawLine(200, 0, 0, 200);
  dc->drawString(20, 110, HoString("úpìl ïábelské ódy",1));
}

void okno(HoPushButton *, void *) {
  HoWindow *w = new HoWindow(NULL,"Nic");
  w->create();
}
HoWindow *hlavni, *edl;

void titul(HoPushButton *, void *) {
  hlavni->setText(edl->getText());
}


int MojeOkno::create(){
  new HoPushButton(this, HoString("Nové okno", 1), 20, 20, 200, 50, okno);
  new HoPushButton(this, HoString("Zmìò titulek", 1), 20, 80, 200, 50, titul);
  edl = new HoEdit(this, HoString("Pøíli¹ ¾lu»ouèký kùò", 0), 20, 140, 200, 50);
  HoWindow::create();
  return 1;
}

int homain(int argc,char * argv[]){
  HoApp *a;

  a = new HoApp(argc, argv);
  hlavni = new MojeOkno(NULL, "Moje Okno");
  hlavni->create();
  a->run();
  delete a;
  return 0;
}

