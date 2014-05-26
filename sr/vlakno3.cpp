#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <holog.h>
#include <hoserversocket.h>
#include <hothread.h>
#include "global.h"

static HoServerSocket *ladiciSocket;

static void konec() {
  // zavøu ladící socket
  ladiciSocket->close();

  // promìnná, která øekne hlavnímu vláknu, ¾e má konèit
  mamKoncit = true;

  // ukonèím vlákno hlavní fronty
  Zprava *null = NULL;
  hlavniFronta->push(null);
  semaforKlienti.down();
  for (SCKlienti::iterator i = klienti.begin(); i != klienti.end(); i++) {
    Zprava *null = NULL;
    i->second->fronta->push(null);
  }
  semaforKlienti.up();

  // probudím hlavní vlákno, které vísí na accept
  HoSocket s;
  s.connect("localhost", port);

  // probudim kostici vlakno
  semaforKosteni.up();

}


void Vlakno3(void * param) {
  int port = (int) param;
  holog.log(I3, "Ovladaci Vlakno3 spusteno na portu %i\n", port);

  ladiciSocket = new HoServerSocket(port, 500);
  if (!ladiciSocket->listenBind()) {
    holog.log(F2, "Nelze listenBind\n");
    exit(1);
  }
  bool kon = false;
  while (!kon) {
    HoSocket *client = ladiciSocket->accept();
    if (!client) {
      holog.log(F2, "Nelze pøijmout soket\n");
      exit(1);
    }
    char co;
    holog.log(I1, "pred recv\n");
    int kolik = client->recv((void *) &co, 1);
    if (kolik == 1) {
      switch (co) {
        case 'v':
          semaforKlienti.down();
          semaforStolky.down();
          klienti.debugPrint(stdout);
          stolky.print(stdout);
          semaforStolky.up();
          semaforKlienti.up();
          break;
        case 'k':
          holog.log(I4, "Prej mam koncit\n");
          konec();
          kon = true;
          break;
        default:
          goto els;
      }
    } else {
       els:
       holog.log(W2, "Chyba ovladace\n");
    }
    delete client;
  }
  delete ladiciSocket;
}

