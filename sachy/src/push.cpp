#include <hopushbutton.h>
#include <hothread.h>
#include <hoapp.h>
#include <homsgbox.h>
#include "global.h"
#include "push.h"
#include "vlakna.h"
#include "onevent.h"

void pushLogin(HoPushButton *, void *) {
  if (stav != STAV_PRED_PUSH) return;
  stav = STAV_PRED_VERZE;
  HoThread t(vlaknoVerze, NULL);
  t.run();
}

void pushTah(int stolek, int tah) {
  Zprava *z = new Zprava;
  z->initTah(stolek, tah);
  HoThread t(vlaknoBeznaZprava, z);
  t.run();
}

void pushNovyStolek(HoPushButton *, void *) {
  if (stav != STAV_PO_LOGIN) return;
  Zprava *z = new Zprava;
  z->initNovyStolek(true, 5, 2);
  HoThread t(vlaknoBeznaZprava, z);
  t.run();
}

void pushPrisednout(HoPushButton *, void *) {
  if (stav != STAV_PO_LOGIN) return;
  SCStolek *stolek = stolkyListbox->getAktStolek();
  if (!stolek) {
    HoMsgBox::msgBox(HoString("Chyba"), HoString("Není oznaèen stolek.", 0));
    return;
  }
  Zprava *z = new Zprava;
  z->initPrisednout(stolek->id, false);
  HoThread t(vlaknoBeznaZprava, z);
  t.run();
}

void pushPozorovatel(HoPushButton *, void *) {
  if (stav != STAV_PO_LOGIN) return;
/*  HoThread t(vlaknoNovyStolek, NULL);
  t.run();*/
}

void pushZavrelStolek(int id) {
  if (stav != STAV_PO_LOGIN) return;
  Zprava *z = new Zprava;
  z->initOdsednout(id);
  HoThread t(vlaknoBeznaZprava, z);
  t.run();
}

void pushChat(HoPushButton *, void *) {
  if (stav != STAV_PO_LOGIN) return;
  Zprava *z = new Zprava;
  z->initChat(0, chatEdit->getText().toUtf8());
  HoThread t(vlaknoBeznaZprava, z);
  chatEdit->setText("");
  t.run();
}


