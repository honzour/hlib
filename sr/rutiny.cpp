#include <string.h>
#include <errno.h>
#include "holog.h"
#include "rutiny.h"
#include "global.h"
#include "zpravy.h"
#include "vlakno4.h"

/**
Funkce po¹le klientovi (pokud byloZiju) v¹echny zprávy vèetnì odpovìdi na ¾iju.
Pokud nebylo ¾iju, nedìlá se nic.

Volá se pokud:
1. jednou za 10 vteøin (odpovìï na ¾iju)
2. klientovi do¹la zpráva (pokud !byloZiju, nestane se nic)
3. od klienta do¹lo ¾iju (napøíklad do¹la zpráva, kdy¾ nebylo ¾iju)

@param i ukazatel na klienta
@param iJenZiju má se poslat zpráva, i kdy¾ kromì ¾iju ¾ádná není
*/
void posliZpravyKlientovi(SCKlienti::iterator i, bool iJenZiju = false) {
  // neposílat mrtvole, neposílat, pokud není na co odpovìdìt
  if (i->second->vykostit || !i->second->byloZiju) return;
  // neposílat, pokud bych poslal jen ziju a nemám to dìlat
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
  Roze¹le 1 nebo 2 zprávy hráèùm. Semafor klientù nesklápí, musí u¾ být dole.
  Pokud je to zpráva stolku, sklápí semafor klientù. Zprávy posílá rovnou,
  pokud byloZiju, jinak do èekací fronty.
  @param zprava1 1. zpráva se po¹le v¾dy
  @param zprava2 2. zpráva, je-li NULL, nepo¹le se
  @param stolek je-li 0, v¹em hráèùm, jinak jen hráèùm u stolku
  @param kromeSoket tomu se neposílá, pokud NULL, ¾ádné omezení
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
    // else po¹li to v¹em
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



