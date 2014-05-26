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
Vl�kno 1 (hlavn�)             sr.cpp
 - vytvo�� socket, vytvo�� hlavn� frontu
 - vis� na accept

Vl�kno 2                      vlakno2.cpp
 - zpracov�n� zpr�vy do�l� hlavn� frontou, odpov�di d�v� do p��slu�n� vedlej��
   fronty
 - vis� na read fronta

Vl�kno 3                      vlakno3.cpp
 - �ek� na ovl�dac�m portu
 - vis� na accept

Vl�kno 4                      vlakno4.cpp
 - jednou za x sekund projde v�echny u�ivatele a vykost� mrtvolky
 - jednou za y sekund projde v�echny u�ivatele a odpov� na ZIJU
 - vis� na sleep

                              vlaknok.cpp
Vl�kno k a� k + n - 1 (n je po�et p�ipojen�ch klientsk�ch program�)
 - �te ze sv� vedlej�� fronty, p��jde-li zpr�va, po�le ji po socketu
 - vis� na read fronta

                              vlaknors.cpp
Vl�kno k + n a� k + 2 * n - 1 (n je po�et p�ipojen�ch klientsk�ch program�)
 - �te ze socketu, p��jde-li zpr�va, po�le ji po hlavn� front�
 - vis� na read socket


Ke struktur�m se p�istupuje pod semaforem.

*/
HoServerSocket *mainSocket;
int homain(int argc, char ** const argv) {
  struct sockaddr_in sockName;
  struct sockaddr_in clientInfo;
  
  holog.init(/*"/dev/tty"*/NULL, 0, 0, 0, 0, 0);
  holog.log(I3, "Server spu�t�n\n");
  if (argc != 3) {
    holog.log(F1, "Syntaxe sr port ovladaci_port\n");
    return -1;
  }
 
  hlavniFronta = new FrontaZprav; 
  port = atoi(argv[1]);
  ovladaci_port = atoi(argv[2]);
  mainSocket = new HoServerSocket(port, 50);
  if (!mainSocket->listenBind() /*|| !mainSocket->setBlocking(false)*/) {
    holog.log(F2, "Nepoda�ilo se vytvo�it socket\n");
    delete mainSocket;
    return -1;
  }

  HoThread vlakno(VlaknoHlavniPipy);

  if (!vlakno.run()) {
    holog.log(F2, "Probl�m s vytvo�en�m vl�kna hlavn� p�py\n");
    return -1;
  }

  HoThread vlakno3(Vlakno3, (void *) ovladaci_port);
  if (!vlakno3.run()) {
    holog.log(F2, "Probl�m s vytvo�en�m ovl�dac�ho vl�kna\n");
    return -1;
  }
  
  HoThread vlakno4(Vlakno4);
  if (!vlakno4.run()) {
    holog.log(F2, "Probl�m s vytvo�en�m kost�c�ho vl�kna\n");
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

