#include <string.h>
#include <errno.h>
#include "holog.h"
#include "rutiny.h"
#include "global.h"
#include "zpravy.h"
#include "vlakno4.h"

/**
Funkce po�le klientovi (pokud byloZiju) v�echny zpr�vy v�etn� odpov�di na �iju.
Pokud nebylo �iju, ned�l� se nic.

Vol� se pokud:
1. jednou za 10 vte�in (odpov�� na �iju)
2. klientovi do�la zpr�va (pokud !byloZiju, nestane se nic)
3. od klienta do�lo �iju (nap��klad do�la zpr�va, kdy� nebylo �iju)

@param i ukazatel na klienta
@param iJenZiju m� se poslat zpr�va, i kdy� krom� �iju ��dn� nen�
*/
void posliZpravyKlientovi(SCKlienti::iterator i, bool iJenZiju = false) {
  // nepos�lat mrtvole, nepos�lat, pokud nen� na co odpov�d�t
  if (i->second->vykostit || !i->second->byloZiju) return;
  // nepos�lat, pokud bych poslal jen ziju a nem�m to d�lat
  if (!iJenZiju && i->second->zpravy.empty()) return;

  i->second->byloZiju = false;
  bool byloNeco = false;
  while (!i->second->zpravy.empty()) {
    Zprava *z = i->second->zpravy.front();
    z->posledni = 0;
    OdesliZpravuKeHraci(z, i->second->fronta);
    i->second->zpravy.pop();
    byloNeco = true;
  }
  /*if (!byloNeco)*/ PosliJednoduchouZpravu(i->first, ZPRAVA_ZIJU_OK, 1);
}

void rozesliNezijuZpravy() {
  SCKlienti::iterator i;
   
  for (i = klienti.begin(); i != klienti.end(); i++) {
    posliZpravyKlientovi(i);
  }
}

void Vykosti(HoSocket *socket, bool lock) {
  holog.log(I2, "Vykosti %p\n", socket);
  if (lock) {
    holog.log(D1, "pred down\n");
    semaforKlienti.down();
    holog.log(D1, "proslo down\n");
  }
  semaforStolky.down();
  
  SCKlienti::iterator i = klienti.find(socket);
  if (i != klienti.end() && !i->second->vykostit) {
    i->second->vykostit = true;
    SCStolky::iterator j;
    znova:
    for (j = stolky.begin(); j != stolky.end(); j++) {
      if (stolky.odsedl(j->second->id, i->second->id)) goto znova;
    }
    if (!mamKoncit && i->second->id) RozesliLogout(i->second->id);
  }

  semaforStolky.up();
  if (lock) semaforKlienti.up();
}

/**
  Roze�le 1 nebo 2 zpr�vy hr���m. Semafor klient� neskl�p�, mus� u� b�t dole.
  Pokud je to zpr�va stolku, skl�p� semafor klient�. Zpr�vy pos�l� rovnou,
  pokud byloZiju, jinak do �ekac� fronty.
  @param zprava1 1. zpr�va se po�le v�dy
  @param zprava2 2. zpr�va, je-li NULL, nepo�le se
  @param stolek je-li 0, v�em hr���m, jinak jen hr���m u stolku
  @param kromeSoket tomu se nepos�l�, pokud NULL, ��dn� omezen�
*/
void Rozesli2ZpravyHracum(Zprava *zprava1, Zprava *zprava2, int stolek, HoSocket *kromeSoket, bool zamykatStolky) {
  if (zprava2) {
    zprava1->posledni = 0;
    zprava2->posledni = 1;
  } else {
    zprava1->posledni = 1;
  }

  SCKlienti::iterator i = klienti.begin();
  SCStolky::iterator si;
  
  if (stolek) {
    if (zamykatStolky) semaforStolky.down();
    si = stolky.find(stolek);
    if (si == stolky.end()) {
    if (zamykatStolky) semaforStolky.up();
      return;
    }
  }
  
  for (; i != klienti.end(); i++) {
    if (kromeSoket == i->second->socket) continue;
    if (i->second->vykostit) continue;
    if (stolek) {
      bool posli = 
        (si->second->bily == i->second->id ||
         si->second->cerny == i->second->id);
      if (!posli) {
        for (vector<int>::iterator vi = si->second->prihlizejici.begin();
          vi != si->second->prihlizejici.end(); vi++)
          if (*vi == i->second->id) {
            posli = true;
            break;
          }
      }
      if (!posli) continue;
    } // if (stolek)
    // else po�li to v�em
    if (i->second->byloZiju) {
//      holog.log(D4, "Primo odesilam %i\n", zprava1->typ);
      OdesliZpravuKeHraci(new Zprava(*zprava1), i->second->fronta);
      if (zprava2) {
        OdesliZpravuKeHraci(new Zprava(*zprava2), i->second->fronta);
      }
      i->second->byloZiju = false;
    } else {
//      holog.log(D4, "Pushuju do fronty %i\n", zprava1->typ);
      i->second->zpravy.push(new Zprava(*zprava1));
      if (zprava2) {
        i->second->zpravy.push(new Zprava(*zprava2));
      }
    }
  }
  if (stolek && zamykatStolky) semaforStolky.up();
  delete zprava1;
  if (zprava2) {
    delete zprava2;
  }
}

void RozesliZpravuHracum(Zprava *zprava, int stolek, HoSocket *kromeSoket,
  bool zamykatStolky) {
  Rozesli2ZpravyHracum(zprava, NULL, stolek, kromeSoket, zamykatStolky);
}

void OdesliZpravuKeHraci(Zprava *zprava, Fronta<Zprava *> *fronta) {
  fronta->push(zprava);
}

void OdesliZpravuKeHraciID(Zprava *zprava, int id) {
  SCKlienti::iterator i;

  for (i = klienti.begin(); i != klienti.end(); i++) {
    if (i->second->id == id && !i->second->vykostit) {
      OdesliZpravuKeHraci(zprava, i->second->fronta);
      return;
    }
  }
  delete zprava;
}

void PosliJednoduchouZpravu(HoSocket *socket, int zprava, char posledni) {
  holog.log(D3, "PosliJednoduchouZpravu %i %i\n", socket, zprava);
  SCKlienti::iterator i = klienti.find(socket);
  if (i == klienti.end()) {
    holog.log(E2, "Neznamy klient");
    return;
  }
  Zprava *z = new Zprava(zprava);
  z->posledni = posledni;
  holog.log(D3, "Jeste ziju\n");
  OdesliZpravuKeHraci(z, i->second->fronta);
}

void PosliIntZpravu(HoSocket *socket, int zprava, int param, char posledni)
{
  holog.log(D3, "PosliIntZpravu %p %i %i\n", socket, zprava, param);
  SCKlienti::iterator i = klienti.find(socket);
  Zprava *z = new Zprava(zprava, 4);
  z->posledni = posledni;
  param = htonl(param);
  memcpy(z->param, &param, 4);
  OdesliZpravuKeHraci(z, i->second->fronta);
}

void PosliOK(HoSocket *socket, char posledni) {
  PosliJednoduchouZpravu(socket, ZPRAVA_OK, posledni);
}

void RozesliLogout(int id) {
  Zprava *z = new Zprava(ZPRAVA_NEKDO_LOGOUT, 4);
  
  *(int *) z->param = htonl(id);
  RozesliZpravuHracum(z, 0, 0);
}



