#include <hothread.h>
#include <holog.h>
#include <hosocket.h>
#include "vlakno2.h"
#include "vlaknok.h"
#include "rutiny.h"
#include "zpravy.h"
#include "zprava.h"
#include "global.h"

static bool NactiZpravuOdHrace(Zprava *zprava, HoSocket *s) {
  holog.log(I1, "NactiZpravuOdHrace %p\n", s);
  zprava->socket = s;
  if (
    !s->nactiInt(&(zprava->typ), true) ||
    !s->nactiInt(&(zprava->delka), true)
    ) {
    holog.log(E2, "Neproslo nactiInt %p\n", s);
    goto ZpravaError;
  }
  holog.log(I1, "delka = %i, typ = %i\n", zprava->delka, zprava->typ);
  if (zprava->delka <= 0) {
    zprava->delka = 0;
    zprava->param = NULL;
    return true;
  }
  zprava->param = (unsigned char *) malloc(zprava->delka);
  if (!zprava->param) {
    holog.log(E2, "Neproslo malloc %p %i\n", s, zprava->delka);
    goto ZpravaError;
  }
  if (s->recv(zprava->param, zprava->delka) != zprava->delka) {
    free(zprava->param);
    holog.log(E2, "Neproslo recv %p %i\n", s, zprava->delka);
    goto ZpravaError;
  }
   
  return true;
  ZpravaError:
  zprava->typ = ZPRAVA_ERROR;
  zprava->delka = 0;
  zprava->param = NULL;
  return false; 
}

static bool OdesliZpravuOdHrace(Zprava *zprava, HoSocket *s) {
  holog.log(D2, "OdesliZpravuOdHrace %p\n", s);
  hlavniFronta->push(zprava);
  return true;
}

static bool ZkontrolujZpravuOdHrace(Zprava *zprava) {
  return true;
}

void VlaknoOdHrace(void *p) {
  HoSocket *s = (HoSocket *) p;
  // Na zaèátku vlákna neznám ani ID hráèe
/*  int hrac = 0;*/
  bool pokracovat = true, odeslano; 
  Zprava *zprava;
  SCKlient *k = new SCKlient(s, "");
  FrontaSocket *fs = new FrontaSocket;
  fs->s = s;
  fs->f = k->fronta;
  holog.log(D1, "bude down");
  semaforKlienti.down();
  holog.log(D1, "proslo down");
  klienti.insert(SCKlienti::value_type(s, k)) ;
  semaforKlienti.up();
  HoThread vlakno(VlaknoVedlejsiPipy, (void *) fs);
  vlakno.run();
  
  while (1) {
    zprava = new Zprava;
    if (mamKoncit || !NactiZpravuOdHrace(zprava, s)) {
      holog.log(E2, "Chyba naètení zprávy na socketu %p\n", s);
      break;
    }
    if (mamKoncit || !ZkontrolujZpravuOdHrace(zprava)) {
      holog.log(E3, "Chyba kontroly zprávy na socketu %p\n", s);
      break;
    }
    if (mamKoncit || !OdesliZpravuOdHrace(zprava, s)) {
      holog.log(E3, "Chyba OdesliZpravuOdHrace, problém hlavní fronty?\n");
      break;
    }
  }
  delete zprava;
  Vykosti(s, true); 
  holog.log(I2, "Konèí vlákno na socketu %p\n", s);
}

