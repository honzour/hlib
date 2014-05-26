#include <stdio.h>
#include "struktury.h"
#include "global.h"
#include "zpravy.h"
#include "rutiny.h"
#include <holog.h>

int SCStolky::firstFree() {
  int i;
  for (i = 1; ; i++) {
    if (find(i) == end()) {
      holog.log(D1, "SCStolky::firstFree() %i\n", i);
      return i;
    }
  }
}

int SCStolky::novy(int hrac, bool bily, int sCelkem, int sTah) {
  int i = firstFree();
  insert(SCStolky::value_type(i, 
   new SCStolek(i, hrac, bily, sCelkem, sTah)
  ));
  return i;
}

void SCStolky::print(FILE *f) {
  fprintf(f, "Výpis stavu stolkù\n");
  for (SCStolky::iterator i = begin(); i != end(); i++) {
    fprintf(f, "id = %i, ", i->first);
    i->second->print(f);
  }
}

SCStolek::SCStolek() {
  init();
};

SCStolek::SCStolek(int id, int hrac, bool bily, int sCelkem, int sTah) {
  if (bily) {
    this->bily = hrac; cerny = 0;
  }
  else {
    this->bily = 0; cerny = hrac;
  }
  this->id = id;
  this->sCelkem = sCelkem;
  this->sTah = sTah;
  init();
};

void SCStolek::init() {
  uloha = InitUlohu();
  msBCas = msCCas = sCelkem * 1000;
  bStart = cStart = false;
  bRemis = cRemis = false;
}

SCStolek::~SCStolek() {
  DoneUlohu(&uloha);
}

void SCStolek::print(FILE *f) {
  fprintf(f, "bily = %i, cerny = %i, prihlizi:", bily, cerny);
  for (vector<int>::iterator i = prihlizejici.begin(); i != prihlizejici.end(); i++) {
    fprintf(f, " %i", *i);
  }
  fputs("\n", f);
}

bool SCStolek::prisedl(int hrac, int kam /* -1 cerny, 0 prihl, 1 bily*/) {
  if (hrac == bily || hrac == cerny) return false;
  for (unsigned i = 0; i < prihlizejici.size(); i++)
    if (prihlizejici[i] == hrac) return false;

  switch (kam) {
    case -1:
      if (cerny) return false;
      cerny = hrac;
      break;
    case 0:
      prihlizejici.push_back(hrac);
      break;
    case 1:
      if (bily) return false;
      bily = hrac;
      break;
    default: return false;
  }
  return true;
}

bool SCStolek::odsedl(int hrac) {
  if (hrac == bily || hrac == cerny) {
    if (cStart && bStart && !mamKoncit) {
      novaPartie();
// TODO remis pokud neni material
      Zprava *zpravaKonec = new Zprava(ZPRAVA_STOLEK_KONEC_PARTIE, 12);
      *(int *) (zpravaKonec->param) = htonl(id);
      *(int *) (zpravaKonec->param + 4) = htonl(hrac == bily ? -1 : 1);
      *(int *) (zpravaKonec->param + 8) =
      htonl((hrac == bily) ? KONEC_SPADNUTI_BILEHO_PROHRA :
                             KONEC_SPADNUTI_CERNEHO_PROHRA);
      holog.log(I4, "STOLEK_KONEC_PARTIE %i\n", id);
      RozesliZpravuHracum(zpravaKonec, id, 0, false);
    }
 
    bStart = cStart = false;
    msBCas = msCCas = sCelkem * 1000; 

    if (hrac == bily) {
      bily = 0; return true;
    }
    cerny = 0; return true;
  }
  for (vector<int>::iterator i = prihlizejici.begin();
    i != prihlizejici.end(); i++) {
    if (*i == hrac) {
      prihlizejici.erase(i, i + 1);
      return true;
    }
  }
  return false;
}

bool SCStolek::presedli() {
  int tmp = bily;
  bily = cerny;
  cerny = tmp;
  return true;
}

bool SCStolky::prisedl(int stolek, int hrac, int kam /* -1 cerny, 0 prihl, 1 bily*/) {
  SCStolky::iterator i = find(stolek);
  if (i == end()) return false;
  return i->second->prisedl(hrac, kam);
}

bool SCStolky::presedli(int stolek) {
  SCStolky::iterator i = find(stolek);
  if (i == end()) return false;
  return i->second->presedli();
}

bool SCStolky::odsedl(int stolek, int hrac) {
  SCStolky::iterator i = find(stolek);
  if (i == end()) return false;
  if (!i->second->odsedl(hrac)) return false;
  if (!i->second->bily && !i->second->cerny) {
    delete i->second;
    erase(i);
  }
  return true;
}

void SCStolek::novaPartie() {
  bStart = cStart = bRemis = cRemis = false;
  DoneUlohu(&uloha);
  uloha = InitUlohu();
}

SCKlient::SCKlient(HoSocket * socket, string login) {
  this->socket = socket;
  aktiv = time(NULL);
  id = 0;
  vykostit = false;
  byloZiju = false;
  fronta = new FrontaZprav;
  this->login = login;
}

void SCKlienti::debugPrint(FILE *f) const {
  SCSimpleKlienti::const_iterator i;
  
  fprintf(f, "\nVýpis v¹ech klientù\n"
               "-------------------\n");
  for (i = begin(); i != end(); i++) {
    fprintf(f, "%i ", i->first);
    i->second->debugPrint(f);
  }
}

