#include <hosocket.h>
#include <hothread.h>
#include <stdlib.h>
#include "vlakna.h"
#include "zpravy.h"
#include "zprava.h"
#include "onevent.h"
#include "global.h"

void vlaknoVerze(void *) {
  semaforSocketu->down();
  if (pSocket) {
    Zprava *z = new Zprava(CHYBA_INIT_SOCKET_2);
    HoThread::sendEvent((void *) z);
    semaforSocketu->up();
  }
  pSocket = new HoSocket();
  pSocket->connect("localhost" /*"nemec2"*/, 5001);
  semaforSocketu->up();
  int i = 0;
  Zprava *z = new Zprava(ZPRAVA_VERZE, 4, (unsigned char *)&i);
  if (!z->send()) {
    HoThread::sendEvent((void *) z);
    return;
  }
  z->recv();
  HoThread::sendEvent((void *) z);
}
 
void vlaknoLogin(void *v) {
  char **c = (char **) v;

  if (stav != STAV_PRED_LOGIN) return;
  
  Zprava *z = new Zprava;
  
  z->initLogin(c[0], c[1]);
  free(c);
  if (!z->send()) {
    HoThread::sendEvent((void *) z);
    return;
  }
  z->recv();
  HoThread::sendEvent((void *) z);
}

void vlaknoZiju(void *) {

  while (1) {
    Zprava *z = new Zprava;
    z->initZiju();

    if (!z->send()) {
      HoThread::sendEvent((void *) z);
      return;
    }
    delete z;
 
    char posledni;
    bool konec;
    do {
      z = new Zprava;
      konec = !z->recv();
      posledni = z->posledni;
      printf("konec %i posledni %i typ %i\n", konec, posledni, z->typ);
      HoThread::sendEvent((void *) z);
      if (konec) return;
    } while (!posledni);
  }
}

void vlaknoBeznaZprava(void *v) {
  Zprava *z = (Zprava *) v;
  if (!z->send()) {
    HoThread::sendEvent((void *) z);
    return;
  }
  delete z;
}

