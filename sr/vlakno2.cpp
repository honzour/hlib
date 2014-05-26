#include <time.h>
#include <holog.h>
#include <globruti.h>
#include "vlakno2.h"
#include "vlakno4.h"
#include "zpravy.h"
#include "struktury.h"
#include "global.h"
#include "rutiny.h"
#include "vlaknok.h"
#include "zprava.h"
#include "zpracuj.h"

#include <sys/types.h>
#include <signal.h>

static bool ZpracujZpravuZHlavniPipy(Zprava *zprava) {
  holog.log(D2, "ZpracujZpravuZHlavniPipy %i\n", zprava->typ);

  semaforKlienti.down();
  SCKlienti::iterator i = klienti.find(zprava->socket);
  if (i == klienti.end()) {
    semaforKlienti.up();
    holog.log(W3, "Klient fdSocket = %p nenalezen\n", zprava->socket);
    return false;
  }
  if (i->second->vykostit) {
    holog.log(W3, "Odmítám zpracovat zprávu, vykostit = %i, fdSocket = %p\n",
      (int) i->second->vykostit, zprava->socket);
    semaforKlienti.up();
    return false;
  }
  if (!i->second->id &&
      zprava->typ != ZPRAVA_VERZE &&
      zprava->typ != ZPRAVA_LOGIN) {
      PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
      Vykosti(zprava->socket, false);
      holog.log(W3, "Odmítám zpracovat zprávu nepøihlá¹eného\n",
      (int) i->second->vykostit, zprava->socket);
      semaforKlienti.up();
      return false;
  }
  i->second->aktiv = time(NULL);

  switch (zprava->typ) {
// Chyba ètení nebo kontroly zprávy - odstøihnout hráèe, 0
    case ZPRAVA_ERROR:
      ZpracujError(zprava);
      break;
// Verze klienta, int
    case ZPRAVA_VERZE:
      ZpracujVerze(zprava);
      break;
// Nic, 0
    case ZPRAVA_ZIJU:
      ZpracujZiju(zprava);
      break;
// Pøihlá¹ení, rùzné >= 2, | utf8 login | utf8 heslo |
    case ZPRAVA_LOGIN:
      ZpracujLogin(zprava);
      break;
// Pøihlá¹ení, bool bily?
    case ZPRAVA_STOLEK:
      ZpracujStolek(zprava);
      break;
    case ZPRAVA_PRISEDL:
      ZpracujPrisedl(zprava);
      break;
    case ZPRAVA_CHAT:
      ZpracujChat(zprava);
      break;
     case ZPRAVA_ODSEDL:
      ZpracujOdsedl(zprava);
      break;
     case ZPRAVA_TAH:
      ZpracujTah(zprava);
      break;
    case ZPRAVA_PRESEDAVAM:
      ZpracujPresedavam(zprava);
      break;
    case ZPRAVA_START:
      ZpracujStart(zprava);
      break;
    case ZPRAVA_VZDAVAM:
      ZpracujVzdavam(zprava);
      break;
    case ZPRAVA_NAVRHUJI_REMIS:
      ZpracujRemis(zprava);
      break;
    default:
      holog.log(E3, "ZpracujZpravuZHlavniPipy Neznámá zpráva %i %i\n",
        zprava->typ, zprava->delka);
  }
  semaforKlienti.up();
  return true;
}

void VlaknoHlavniPipy(void *) {
  Zprava *zprava;
  holog.log(I3, "VlaknoHlavniPipy spusteno\n"); 
  while (zprava = hlavniFronta->pop()) {
    if (!mamKoncit) ZpracujZpravuZHlavniPipy(zprava);
    delete zprava;
  }
  semaforStolky.down();
  SCStolky::iterator i;
  for (i = stolky.begin(); i != stolky.end(); i++) {
    delete i->second;
  }
  stolky.clear();
  semaforStolky.up();

  semaforKlienti.down();
  for (SCKlienti::iterator si = klienti.begin(); si != klienti.end(); i++) {
    si->first->close();
  }
  semaforKlienti.up();

  holog.log(I3, "Konci vlakno hlavni pipy\n");
}

