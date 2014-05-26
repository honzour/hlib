#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <hothread.h>
#include <hoserversocket.h>
#include "struktury.h"
#include "zpravy.h"
#include "holog.h"
#include "vlakno2.h"
#include "vlakno3.h"
#include "vlakno4.h"
#include "vlaknors.h"
#include "global.h"
#include "rutiny.h"
#include "fronta.h"

/*
Vlákno 1 (hlavní)             sr.cpp
 - vytvoøí socket, vytvoøí hlavní frontu
 - visí na accept

Vlákno 2                      vlakno2.cpp
 - zpracováná zprávy do¹lé hlavní frontou, odpovìdi dává do pøíslu¹né vedlej¹í
   fronty
 - visí na read fronta

Vlákno 3                      vlakno3.cpp
 - èeká na ovládacím portu
 - visí na accept

Vlákno 4                      vlakno4.cpp
 - jednou za x sekund projde v¹echny u¾ivatele a vykostí mrtvolky
 - jednou za y sekund projde v¹echny u¾ivatele a odpoví na ZIJU
 - visí na sleep

                              vlaknok.cpp
Vlákno k a¾ k + n - 1 (n je poèet pøipojených klientských programù)
 - ète ze své vedlej¹í fronty, pøíjde-li zpráva, po¹le ji po socketu
 - visí na read fronta

                              vlaknors.cpp
Vlákno k + n a¾ k + 2 * n - 1 (n je poèet pøipojených klientských programù)
 - ète ze socketu, pøíjde-li zpráva, po¹le ji po hlavní frontì
 - visí na read socket


Ke strukturám se pøistupuje pod semaforem.

*/
HoServerSocket *mainSocket;
int homain(int argc, char ** const argv) {
  struct sockaddr_in sockName;
  struct sockaddr_in clientInfo;
  
  holog.init(/*"/dev/tty"*/NULL, 0, 0, 0, 0, 0);
  holog.log(I3, "Server spu¹tìn\n");
  if (argc != 3) {
    holog.log(F1, "Syntaxe sr port ovladaci_port\n");
    return -1;
  }
 
  hlavniFronta = new FrontaZprav; 
  port = atoi(argv[1]);
  ovladaci_port = atoi(argv[2]);
  mainSocket = new HoServerSocket(port, 50);
  if (!mainSocket->listenBind() /*|| !mainSocket->setBlocking(false)*/) {
    holog.log(F2, "Nepodaøilo se vytvoøit socket\n");
    delete mainSocket;
    return -1;
  }

  HoThread vlakno(VlaknoHlavniPipy);

  if (!vlakno.run()) {
    holog.log(F2, "Problém s vytvoøením vlákna hlavní pípy\n");
    return -1;
  }

  HoThread vlakno3(Vlakno3, (void *) ovladaci_port);
  if (!vlakno3.run()) {
    holog.log(F2, "Problém s vytvoøením ovládacího vlákna\n");
    return -1;
  }
  
  HoThread vlakno4(Vlakno4);
  if (!vlakno4.run()) {
    holog.log(F2, "Problém s vytvoøením kostícího vlákna\n");
    return -1;
  }

  while (1) {
    HoSocket *client = mainSocket->accept();
    if (!client) {
      holog.log(F2, "Neproslo accept\n");
      break;
    }
    if (mamKoncit) {
      holog.log(I4, "Mam koncit\n");
      delete client;
      break;
    }
    holog.log(I2, "Accept novy hrac, socket = %p\n", client);
    HoThread vlaknoOdHrace(VlaknoOdHrace, (void *) client);
    vlaknoOdHrace.run();
  }
//  semaforKosteni.up();
  holog.log(I4, "Hlavni vlakno jde spat na 10 vterin\n");
  HoThread::sleep(10000);
  delete mainSocket;
  delete hlavniFronta;
  holog.log(I4, "Mazu klienty\n");
  SCKlienti::iterator i;
  while ((i = klienti.begin()) != klienti.end()) {
    i->second->smazFrontu();
    delete i->second;
    klienti.erase(i);
  }
  holog.log(I4, "Konec hlavniho vlakna\n");
  return 0;
}

