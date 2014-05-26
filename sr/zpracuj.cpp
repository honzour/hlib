#include <holog.h>
#include <globruti.h>

#include "zpravy.h"
#include "zprava.h"
#include "zpracuj.h"
#include "struktury.h"
#include "global.h"
#include "rutiny.h"
#include "db.h"
#include "vlakno4.h"

void ZpracujError(Zprava *zprava) {
  holog.log(I2, "ZpracujError %i\n", zprava->socket);
  PosliOK(zprava->socket, 1);
  Vykosti(zprava->socket, false);
}

void ZpracujVerze(Zprava *zprava) {
  holog.log(I2, "ZpracujVerze %p\n", zprava->socket);
  if (zprava->delka != 4) { 
    holog.log(E2, "ZpracujVerze - chyba\n");
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
    Vykosti(zprava->socket, false);
    return;
  }
  if (*((int *)zprava->param) != VerzeServeru) {
    holog.log(W3, "ZpracujVerze - chyba, klient verze %i\n",
      *((int *)zprava->param));
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_SPATNA_VERZE, 1);
    Vykosti(zprava->socket, false);
    return;
  }
  PosliJednoduchouZpravu(zprava->socket, ZPRAVA_VERZE_OK, 1);
}

void ZpracujLogin(Zprava *zprava) {
  holog.log(I1, "ZpracujLogin %i\n", zprava->socket);
  int i, nul = 0;
  char *login = (char *) zprava->param;
  char *heslo;
  int id;
  bool duplicita;
  SCKlienti::iterator it;
  SCStolky::iterator is;
  int velikostStolku = 0;
  int velikostHracu = 0;
  int n = 0;
  int m = 0;

  unsigned char *pos;
 
  if (zprava->delka < 2) goto ErrorLogin;
  for (i = 0; i < zprava->delka; i++)
    if (!zprava->param[i]) nul++;

  if (nul != 2) goto ErrorLogin;
  
  for (i = 0; i < zprava->delka; i++)
    if (!zprava->param[i]) {
      heslo = (char *) zprava->param + i + 1;
      break;
    }
  if (i + 1 > zprava->delka) goto ErrorLogin;
  id = getID(login, heslo);
  if (!id) {
    holog.log(E1, "Spatny uzivatel %i, '%s':'%s', posilam LOGIN_ERROR\n",
      zprava->socket, login, heslo);
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_LOGIN_ERROR, 1);
    Vykosti(zprava->socket, false);
    return;
  }
  duplicita = false;
  for (it = klienti.begin(); it != klienti.end(); it++) {
    if (!it->second->vykostit && it->second->id == id) {
      duplicita = true;
      break;
    }
  }
  if (!duplicita) {
    holog.log(I2, "LOGIN, %i '%s':'%s' NEKDO_LOGIN\n",
      zprava->socket, login, heslo);
    Zprava *z = new Zprava(ZPRAVA_NEKDO_LOGIN, 4 + 1 + strlen(login));
    i = htonl(id);
    *(int *)z->param = i;
    strcpy((char *) z->param + 4, login);
    RozesliZpravuHracum(z, 0, zprava->socket);
        it = klienti.find(zprava->socket);
    it->second->id = id;
    it->second->login = login;
    Fronta<Zprava *> *fronta = it->second->fronta;
    for (it = klienti.begin(); it != klienti.end(); it++) {
      if (it->second->id && !it->second->vykostit) {
        velikostHracu += (4 + 1 + it->second->login.size());
        n++;
      }
    }
    
    for (is = stolky.begin(); is != stolky.end(); is++) {
      velikostStolku += (24 + is->second->prihlizejici.size() * 4);
      m++;
    }
    z = new Zprava(ZPRAVA_LOGIN_OK, 12 + velikostHracu + velikostStolku);
    pos = z->param;
 
 #define PridejInt(k) i = htonl(k);\
                      memcpy(pos, &i, 4);\
                      pos += 4;

    PridejInt(id);
    PridejInt(n);
    PridejInt(m);
  
    for (it = klienti.begin(); it != klienti.end(); it++) {
      if (it->second->id && !it->second->vykostit) {
        PridejInt(it->second->id);
        memcpy(pos, it->second->login.c_str(), it->second->login.size() + 1);
        pos += it->second->login.size() + 1;
      }
    }
    for (is = stolky.begin(); is != stolky.end(); is++) {
      PridejInt(is->second->id);
      PridejInt(is->second->bily);
      PridejInt(is->second->cerny);
      PridejInt(is->second->sCelkem);
      PridejInt(is->second->sTah);
      PridejInt(is->second->prihlizejici.size());
      for (vector<int>::iterator iv = is->second->prihlizejici.begin();
        iv != is->second->prihlizejici.end(); iv++) {
        PridejInt(*iv);
      }
    }
    holog.log(D3, "Nepøeteklo ? %i == %i\n", z->delka, pos - z->param);
    OdesliZpravuKeHraci(z, fronta);
  } else {
    holog.log(W2, "Duplicitni prihlaseni %i '%s':'%s', LOGIN_DUPLICITA\n",
      zprava->socket, login, heslo);
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_LOGIN_DUPLICITA, 1);
    Vykosti(zprava->socket, false);
  }
  return;
  ErrorLogin:
  PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
  Vykosti(zprava->socket, false);
}

void ZpracujZiju(Zprava *zprava) {
  holog.log(D4, "ZpracujZiju %i\n", zprava->socket);
  SCKlienti::iterator i = klienti.find(zprava->socket);
  if (i == klienti.end()) return;
  i->second->byloZiju = true;
  posliZpravyKlientovi(i, false);
}

void ZpracujChat(Zprava *zprava) {

  int id, stolek;
 
  holog.log(I1, "ZpracujChat %i\n", zprava->socket);
  SCKlienti::iterator i = klienti.find(zprava->socket);
  if (i == klienti.end()) return;
  if (!i->second->id) {
    holog.log(W3, "Neoprávnìný pokus o chat!\n");
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
    Vykosti(zprava->socket, false);   
    return;
  }

  if (zprava->delka < 5 || zprava->param[zprava->delka - 1]) {
    holog.log(E2, "©patná chat zpráva délky %i\n", zprava->delka);
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
    Vykosti(zprava->socket, false);
    return;
  }
  id = i->second->id;
  memcpy(&stolek, zprava->param, 4);
  stolek = ntohl(stolek);
  Zprava *z = new Zprava(ZPRAVA_CHAT_OK, zprava->delka + 4);
  int id2 = htonl(id);
  memcpy(z->param, &id2, 4);
  memcpy(z->param + 4, zprava->param, zprava->delka);
  z->posledni = 1;
  RozesliZpravuHracum(z, stolek, NULL);
}

void ZpracujStolek(Zprava *zprava) {
  holog.log(I1, "ZpracujStolek %p\n", zprava->socket);
  SCKlienti::iterator it;
  int id, sCelkem, sTah;
  bool bily;

  it = klienti.find(zprava->socket);
  if (it == klienti.end()) return;
 
  if (zprava->delka != 9) {
    holog.log(W3, "©patná délka zprávy ZPRAVA_STOLEK!\n");
    goto ErrorStolek;
  }

  if (!it->second->id) {
    holog.log(W3, "Neoprávnìný pokus o zalo¾ení stolku!\n");
    ErrorStolek:
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
    Vykosti(zprava->socket, false);   
    return;
  }
  id = it->second->id;
  it->second->byloZiju = true;
  bily = zprava->param[0];
  memcpy(&sCelkem, zprava->param + 1, 4);
  memcpy(&sTah, zprava->param + 5, 4);
  sCelkem = ntohl(sCelkem);
  sTah = ntohl(sTah);
  holog.log(I1, "Zakládám stolek hráè = %i celkem = %i tah = %i\n", id, sCelkem,
    sTah);
// int id, int bily, int cerny, int sCelkem, int sTah,
//     int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]
  semaforStolky.down();
  int sid = stolky.novy(id, bily, sCelkem, sTah);
  holog.log(D4, "Tady jsem\n");
  stolky.print(stdout);
  Zprava *z = new Zprava(ZPRAVA_STOLEK_NOVY, 24);
  *(int *) (z->param) = htonl(sid);
  *(int *) (z->param + 4) = htonl(bily ? id : 0);
  *(int *) (z->param + 8) = htonl(!bily ? id : 0);
  *(int *) (z->param + 12) = htonl(sCelkem);
  *(int *) (z->param + 16) = htonl(sTah);
  *(int *) (z->param + 20) = htonl(0);
  RozesliZpravuHracum(z, 0, 0);
  semaforStolky.up();
}

void ZpracujPrisedl(Zprava *zprava) {
  holog.log(I1, "ZpracujPrisedl %i\n", zprava->socket);
  SCKlienti::iterator it;
  int id, idStolku, kam;
  bool pozorovatel;
  vector<int>::iterator pi;
  Zprava *z;

  if (zprava->delka != 5) {
    holog.log(W3, "©patná délka zprávy ZPRAVA_PRISEDL!\n");
    goto ErrorStolek;
  }

  it = klienti.find(zprava->socket);
  if (!it->second->id) {
    holog.log(W3, "Neoprávnìný pokus o pøisednutí ke stolku!\n");
    ErrorStolek:
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
    Vykosti(zprava->socket, false);   
    return;
  }

  id = it->second->id;
  pozorovatel = zprava->param[4];
  memcpy(&idStolku, zprava->param, 4);
  idStolku = ntohl(idStolku);
  holog.log(I1, "Hráè = %i pøisedává ke stolku %i, pozorovatel = %i\n",
   id, idStolku, pozorovatel);
  
  semaforStolky.down();
  SCSimpleStolky::iterator si = stolky.find(idStolku);
  if (si == stolky.end()) {
    holog.log(W3, "Prisednuti k neexistujicimu stolku\n");
    goto nic;
  }
  if (pozorovatel) kam = 0; else {
    if (!si->second->bily) kam = 1; else
      if (!si->second->cerny) kam = -1; else {
        holog.log(W3, "Prisednuti k obsazenemu stolku\n");
        goto nic;
      }
  }
  if (id == si->second->bily || id == si->second->cerny) {
    holog.log(W3, "Prisednuti ke stolku, kde uz sedi jako hrac\n");
    goto nic;
  }

  
  for (pi = si->second->prihlizejici.begin();
    pi != si->second->prihlizejici.end(); pi++) {
    if (id == *pi) {
      holog.log(W3, "Prisednuti ke stolku, kde uz sedi jako pozorovatel\n");
      goto nic;
    }
  }
  
  holog.log(D2, "Prisedl vraci %i\n", stolky.prisedl(idStolku, id, kam));
  z = new Zprava(ZPRAVA_STOLEK_PRISEDL, 17);
  *(int *) (z->param) = htonl(idStolku);
  *(int *) (z->param + 4) = htonl(id);
  *(int *) (z->param + 8) = htonl(si->second->msBCas);
  *(int *) (z->param + 12) = htonl(si->second->msCCas);
  z->param[16] = pozorovatel;
  
  RozesliZpravuHracum(z, 0, NULL);
  if (pozorovatel) {
    TPartie *prt = si->second->uloha->prt;
    while (prt->p) {
      prt = prt->p;
    }
    int pocet = 0;
    while (prt->l) {
      pocet++;
      prt = prt->l;
    }
    if (pocet) {
      z = new Zprava(ZPRAVA_STOLEK_TAHY_PRIHLIZEJICIMU, 8 + pocet * 4);
      *(int *) (z->param) = htonl(idStolku);
      *(int *) (z->param + 4) = htonl(pocet);
      prt = prt->p;
      int *p = (int *) (z->param + 8);
      while (prt) {
        *p++ = htonl(prt->data.t); 
        prt = prt->p;
      }
      OdesliZpravuKeHraci(z, it->second->fronta);
    }
  }
  nic:;
  semaforStolky.up();
}

/**
  Pøedzpracuje zprávu u nìjakého stolku. Pokud v¹e OK, budou stolky
  zamèené, jinak odemèené.
  @return true, pokud OK
*/
static bool zpravaStolku(Zprava *zprava, SCKlienti::iterator &it,
  SCSimpleStolky::iterator &si, int &id, int &idStolku) {

  if (zprava->delka < 4) {
    holog.log(E3, "©patná délka zprávy u stolku!\n");
    goto ErrorStolek;
  }

  it = klienti.find(zprava->socket);
  if (!it->second->id) {
    holog.log(E3, "Nemohu identifikovat klienta!\n");
    ErrorStolek:
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
    Vykosti(zprava->socket, false);   
    return false;
  }
  
  id = it->second->id;
  memcpy(&idStolku, zprava->param, 4);
  idStolku = ntohl(idStolku);
  holog.log(I1, "Hráè = %i nìco dìlá u stolku %i\n", id, idStolku);
  
  semaforStolky.down();
  si = stolky.find(idStolku);
  if (si == stolky.end()) {
    holog.log(E3, " Akce u neexistujícího stolku\n");
    semaforStolky.up();
    goto ErrorStolek;
  }
  return true; 
}

static bool overDelku(const Zprava *zprava, int delka) {
  if (zprava->delka != delka) {
    semaforStolky.up();
    holog.log(E3, "©patná délka zprávy!\n");
    PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
    Vykosti(zprava->socket, false);   
    return false;
  }
  return true;
}

void ZpracujPresedavam(Zprava *zprava) {
  SCKlienti::iterator it;
  SCSimpleStolky::iterator si;
  int id, idStolku;
  Zprava *z;

  holog.log(I1, "ZpracujPresedavam %i\n", zprava->socket);
  if (!zpravaStolku(zprava, it, si, id, idStolku)) return;
  if (!overDelku(zprava, 4)) return;
  if (si->second->bily != id && si->second->cerny != id ||
      si->second->bily && si->second->cerny) {
    goto nic;
  }
  if (!stolky.presedli(idStolku)) {
    holog.log(W2, "stolky.presedli selhalo\n");
    goto nic;
  }

  z = new Zprava(ZPRAVA_STOLEK_PRESEDNUTI, 4);
  *(int *) (z->param) = htonl(idStolku);
  holog.log(I1, "STOLEK_PRESEDNUTI %i\n", idStolku); 
  RozesliZpravuHracum(z, 0, 0);
  nic:;
  semaforStolky.up();
}


void ZpracujOdsedl(Zprava *zprava) {
  holog.log(I1, "ZpracujOdsedl %i\n", zprava->socket);
  SCKlienti::iterator it;
  SCSimpleStolky::iterator si;
  int id, idStolku;
  Zprava *z;

  if (!zpravaStolku(zprava, it, si, id, idStolku)) return;
  if (!overDelku(zprava, 4)) return;
 
  if (!stolky.odsedl(idStolku, id)) {
    holog.log(W2, "stolky.odsedl selhalo\n");
    goto nic;
  }
  z = new Zprava(ZPRAVA_STOLEK_ODSEDL, 8);
  *(int *) (z->param) = htonl(idStolku);
  *(int *) (z->param + 4) = htonl(id);
  holog.log(I1, "STOLEK_ODSEDL %i %i\n", idStolku, id); 
  RozesliZpravuHracum(z, 0, 0);
  nic:;
  semaforStolky.up();
}

void ZpracujTah(Zprava *zprava) {
  holog.log(I1, "ZpracujTah %p\n", zprava->socket);
  SCKlienti::iterator it;
  int id, idStolku, tah, cas;
//  bool pozorovatel;
  Zprava *zpravaKonec = NULL; // = NULL nutné, aby s pøípadnì zahodila
  SCSimpleStolky::iterator si;

  if (!zpravaStolku(zprava, it, si, id, idStolku)) return;
  if (!overDelku(zprava, 12)) return;

  memcpy(&tah, ((char *)(zprava->param)) + 4, 4);
  tah = ntohl(tah);
  memcpy(&cas, ((char *)(zprava->param)) + 8, 4);
  cas = ntohl(cas);
  holog.log(I1, "Hráè = %i hraje u stolku %i\n tah %i\n", id, idStolku, tah);
  
  {
    if (si->second->uloha->pozice.bily && si->second->bily != id ||
        !si->second->uloha->pozice.bily && si->second->cerny != id ||
        !si->second->bStart ||
        !si->second->cStart) {
      holog.log(E2,
        "Hráè = %i hraje u stolku %i\n tah %i, ale není na tahu.i\n",
        id, idStolku, tah);
      semaforStolky.up();
      ErrorStolek:
      PosliJednoduchouZpravu(zprava->socket, ZPRAVA_KLIENT_ERROR, 1);
      Vykosti(zprava->socket, false);   
      return;
    }
    u16 tah16 = (u16) tah;
    if (tah16 != tah || !KorektniTah(tah16, si->second->uloha)) {
       holog.log(E2, "Hráè = %i hraje u stolku %i\n nepøípustný tah %i",
       id, idStolku, tah);
       semaforStolky.up();
       goto ErrorStolek;
    }
  }

  if (si->second->bily == id) {
    si->second->cRemis = false;
    si->second->msBCas = cas;
  } else {
    si->second->bRemis = false;
    si->second->msCCas = cas;
  }

  
  Zprava *z = new Zprava(ZPRAVA_STOLEK_TAH, 12);
  *(int *) (z->param) = htonl(idStolku);
  *(int *) (z->param + 4) = htonl(tah);
  *(int *) (z->param + 8) = htonl(cas);
  holog.log(I3, "STOLEK_TAH %i %i\n", idStolku, tah);
  GlobTahni(1, si->second->uloha, 1, tah);

  if (si->second->uloha->KonecPartie) {
    int konec;
    switch (si->second->uloha->KonecPartie) {
      case KONEC_BILY_V_MATU: konec = -1; break;
      case KONEC_CERNY_V_MATU: konec = 1; break;
      default: konec = 0;
    }
    zpravaKonec = new Zprava(ZPRAVA_STOLEK_KONEC_PARTIE, 12);
    *(int *) (zpravaKonec->param) = htonl(idStolku);
    *(int *) (zpravaKonec->param + 4) = htonl(konec);
    *(int *) (zpravaKonec->param + 8) = htonl(si->second->uloha->KonecPartie);
    holog.log(I4, "STOLEK_KONEC_PARTIE %i\n", idStolku);
    si->second->novaPartie();
  }
  semaforStolky.up();
  Rozesli2ZpravyHracum(z, zpravaKonec, idStolku, 0);
}

void ZpracujStart(Zprava *zprava) {
  SCKlienti::iterator it;
  int id, idStolku;
  SCSimpleStolky::iterator si;
  
  holog.log(I1, "ZpracujStart %p\n", zprava->socket);
  if (!zpravaStolku(zprava, it, si, id, idStolku)) return;
  if (!overDelku(zprava, 4)) return;

  bool problem = false;

  if (si->second->bily == id) {
    if (!si->second->bStart && si->second->cerny) {
      si->second->bStart = true;
    }
  } else
  if (si->second->cerny == id) {
    if (!si->second->cStart && si->second->bily) {
      si->second->cStart = true;
    }
  } else {
    problem = true;
  }
  semaforStolky.up();
  if (problem) {
    holog.log(E2, "Ignoruji chybnou zpravu start %i\n", id);
    return;
  }
  Zprava *z = new Zprava(ZPRAVA_STOLEK_START, 8);
  *(int *) (z->param) = htonl(idStolku);
  *(int *) (z->param + 4) = htonl(id);
  RozesliZpravuHracum(z, idStolku, NULL);
}

void ZpracujVzdavam(Zprava *zprava) {
  SCKlienti::iterator it;
  int id, idStolku;
  SCSimpleStolky::iterator si;
  holog.log(I1, "ZpracujVzdavam %p\n", zprava->socket);
  if (!zpravaStolku(zprava, it, si, id, idStolku)) return;
  if (!overDelku(zprava, 4)) return;
  if (si->second->bily != id && si->second->cerny != id ||
      !si->second->cStart || !si->second->bStart) {
    holog.log(W2, "Ignoruji chybnou zpravu vzdavam %p\n", zprava->socket);
    semaforStolky.up();
    return;
  }
  si->second->novaPartie();
  Zprava *zpravaKonec = new Zprava(ZPRAVA_STOLEK_KONEC_PARTIE, 12);
  *(int *) (zpravaKonec->param) = htonl(idStolku);
  *(int *) (zpravaKonec->param + 4) = htonl(si->second->bily ? -1 : 1);
  *(int *) (zpravaKonec->param + 8) =
    htonl((si->second->bily == id) ? KONEC_BILY_VZDAL : KONEC_CERNY_VZDAL);
  holog.log(I4, "STOLEK_KONEC_PARTIE %i\n", idStolku);
  semaforStolky.up();
  RozesliZpravuHracum(zpravaKonec, idStolku, 0);
}

void ZpracujRemis(Zprava *zprava) {
  SCKlienti::iterator it;
  int id, idStolku;
  SCSimpleStolky::iterator si;
  holog.log(I1, "ZpracujRemis %p\n", zprava->socket);
  if (!zpravaStolku(zprava, it, si, id, idStolku)) return;
  if (!overDelku(zprava, 4)) return;
  if (si->second->bily != id && si->second->cerny != id ||
      !si->second->cStart || !si->second->bStart) {
    holog.log(W2, "Ignoruji chybnou zpravu remis %p\n", zprava->socket);
    nic:
    semaforStolky.up();
    return;
  }
  if (si->second->bily == id && si->second->bRemis ||
      si->second->cerny == id && si->second->cRemis ) {
    // nabizi podruhe
    goto nic;
  }
  if (si->second->bRemis || si->second->cRemis ) {
    Zprava *zpravaKonec = new Zprava(ZPRAVA_STOLEK_KONEC_PARTIE, 12);
    *(int *) (zpravaKonec->param) = htonl(idStolku);
    *(int *) (zpravaKonec->param + 4) = htonl(0);
    *(int *) (zpravaKonec->param + 8) =
    htonl(si->second->bily == id ? KONEC_REMIS_BILY_PRIJAL :
      KONEC_REMIS_CERNY_PRIJAL);
    holog.log(I4, "STOLEK_KONEC_PARTIE %i\n", idStolku);
    si->second->novaPartie();
    semaforStolky.up();
    RozesliZpravuHracum(zpravaKonec, idStolku, 0);
    return;
  }
  if (si->second->bily == id) {
    si->second->bRemis = true;
  } else {
    si->second->cRemis = true;
  }
  Zprava *z = new Zprava(ZPRAVA_STOLEK_NAVRHUJE_REMIS, 8);
  *(int *) (z->param) = htonl(idStolku);
  *(int *) (z->param + 4) = htonl(id);
  semaforStolky.up();
  RozesliZpravuHracum(z, idStolku, 0);
  return;
}


