#include <homsgbox.h>
#include <hothread.h>
#include <stdlib.h>
#include "zpravy.h"
#include "zprava.h"
#include "onevent.h"
#include "global.h"
#include "vlakna.h"
#include "stolkywindow.h"
#include "loginwindow.h"
#include "sachwindow.h"
#include "stolkylistbox.h"

void chyba(const char *str, bool deleteSocket = false) {
  HoMsgBox::msgBox(HoString("Chyba"), HoString(str, 0));
  if (deleteSocket) {
    semaforSocketu->down();
    nicimSocket = true;
    if (pSocket) {
      if (pSocket->connected) pSocket->close();
      if (!pisuSocket && !ctuSocket) {
        delete pSocket;
        pSocket = NULL;
      }
    }
    nicimSocket = false;
    semaforSocketu->up();
  }
  if (stav == STAV_PO_LOGIN) {
    stolkyOkno->exitIfLast = false;
    stolkyOkno->destroy();
    loginOkno = new LoginWindow;
  }
  stav = STAV_PRED_PUSH;
}

void ZpracujStolekNovy(Zprava *z) {
  if (z->delka < 24) return;
  int prihlizi = ntohl(*(int *)(z->param + 20));
  bool budeOkno = false;
  int bily, cerny;
  
  if (z->delka < 24 + prihlizi * 4) return;
  SCStolek *stolek = new SCStolek(
    ntohl(*(int *)z->param),
    (bily = ntohl(*(int *)(z->param + 4))),
    (cerny = ntohl(*(int *)(z->param + 8))),
    ntohl(*(int *)(z->param + 12)),
    ntohl(*(int *)(z->param + 16))
  );
  stolek->prihlizi.reserve(prihlizi);
  for (int i = 0; i < prihlizi; i++) {
    int hrac = ntohl(*(int *)(z->param + 24 + 4 * i));
    stolek->prihlizi.push_back(hrac);
    if (hrac == mojeID) {
      budeOkno = true;
    }
  }
  if (bily == mojeID || cerny == mojeID) {
    budeOkno = true;
  }
  
  stolky.insert(SCStolky::value_type(stolek->id, stolek));
  stolkyListbox->insert(stolek);
  if (budeOkno) {
    stolek->sw = new SachWindow(NULL, getListboxString(stolek), 400, 400, stolek->id,
      cerny != mojeID);
    stolek->sw->create();
  }
}

void ZpracujStolekPrisedl(Zprava *z) {
  if (z->delka != 9) return;
  
  bool budeOkno = false;
  int hrac = ntohl(*(int *) (z->param + 4));
  int stolek = ntohl(*(int *) z->param);
  bool pozorovatel = *(z->param + 8);

  if (!stolky.prisedl(stolek, hrac, pozorovatel)) {
    return;
  }
  

  printf("stolky.prisedl(stolek = %i , hrac = %i, pozorovatel = %i)\n",
    stolek, hrac, pozorovatel);

  if (hrac == mojeID) budeOkno = true;
  SCStolky::iterator i = stolky.find(stolek);

  if (budeOkno) {
    i->second->sw = new SachWindow(NULL, 
    getListboxString(i->second), 400, 400, stolek, i->second->bily == mojeID);
    i->second->sw->create();
  } else {
    i->second->sw->setText(getListboxString(i->second));
  }
  stolkyListbox->changed(stolek);
}

void ZpracujStolekOdsedl(Zprava *z) {
  puts("ZpracujStolekOdsedl");
  if (z->delka != 8) return;
  bool budeOkno = false;
  int hrac = ntohl(*(int *) (z->param + 4));
  int stolek = ntohl(*(int *) z->param);

  SCStolky::iterator i = stolky.find(stolek);
  if (i == stolky.end()) return;
  bool zmena = false;
  if (i->second->cerny == hrac || i->second->bily == hrac) {
    zmena = true;
  }
  if (i->second->bily == hrac) {
    i->second->bily = 0;
  }
  if (i->second->cerny == hrac) {
    i->second->cerny = 0;
  }
  if (!i->second->cerny && !i->second->bily) {
    stolkyListbox->deleted(stolek);
    stolky.erase(i);
  } else {
    if (zmena) {
      stolkyListbox->changed(stolek);
      if (i->second->sw) {
        i->second->sw->setText(getListboxString(i->second));
      }  // else odsedl jsem já
      
    }
  }


  if (hrac == mojeID) {
    if (stolkyOkno->programKonci) {
      stolkyOkno->pocetOken--;
      if (stolkyOkno->pocetOken <= 0) {
        stolkyOkno->destroy();
      }
    }
  }
  
}

void ZpracujStolekTah(Zprava *z) {
  puts("void ZpracujStolekTah(Zprava *z)");
  if (z->delka != 8) return;
  
  int tah = ntohl(*(int *) (z->param + 4));
  int stolek = ntohl(*(int *) z->param);
  
  SCStolky::iterator i = stolky.find(stolek);
  if (i == stolky.end() || !i->second->sw) {
    return;
  }
  i->second->sw->tah(tah);
}



void ZpracujChat(Zprava *z) {
  if (z->delka < 9 || z->param[z->delka - 1]) return;
  int id = ntohl(*(int *) z->param);
  HoString sid;
  SCUseri::iterator i = useri.find(id);
  if (i == useri.end())
    sid = HoString("???: ");
  else
    sid = i->second->login + HoString(": ");
  if (chatListbox->stringy->size() > 4)
    chatListbox->removeString(1); 
  chatListbox->addString(sid + HoString((char *)(z->param + 8))); 
}

void ZpracujNekdoLogin(Zprava *z) {
  int id;
  if (z->delka < 5 || z->param[z->delka - 1]) return;
  id = ntohl(*(int *) z->param);
  useri.insert(SCUseri::value_type(id,
    new SCUser(id, HoString((char *) z->param + 4)))
    );
  HoString sid = HoString((char *) z->param + 4) + HoString(" pøi¹el", 0);
  if (chatListbox->stringy->size() > 4)
    chatListbox->removeString(1); 
  chatListbox->addString(sid);
}

void ZpracujNekdoLogout(Zprava *z) {
  int id;
  if (z->delka != 4) return;
  id = ntohl(*(int *) z->param);
  SCUseri::iterator i = useri.find(id);
  if (i == useri.end()) return;

  SCStolky::iterator si;
  bool zmena = true;
  while (zmena) {
    zmena = false;
    for (si = stolky.begin(); si != stolky.end(); si++) {
      if (si->second->cerny != id && si->second->cerny != id) {
        continue;
      }
      zmena = true;
      if (si->second->bily == id) {
        si->second->bily = 0;
      }
      if (si->second->cerny == id) {
        si->second->cerny = 0;
      }
      if (!si->second->cerny && !si->second->bily) {
        stolkyListbox->deleted(si->second->id);
        stolky.erase(si);
      } else {
        stolkyListbox->changed(si->second->id);
      }
    }
  }

  HoString sid = i->second->login + HoString(" ode¹el", 0);
  if (chatListbox->stringy->size() > 4)
    chatListbox->removeString(1); 
  chatListbox->addString(sid); 
  useri.erase(id);
}

void onEvent(void *param) {

  Zprava *z = (Zprava *) param;
  switch (z->typ) {
    case CHYBA_SEND:
      chyba("Odeslání na server", true);
      break; 
    case CHYBA_RECV:
      chyba("Pøijetí ze serveru", true);
      break;  
    case ZPRAVA_SPATNA_VERZE:
      chyba("©patná verze klienta", true);
      break;   
    case ZPRAVA_LOGIN_ERROR:
      chyba("©patné login nebo heslo", true);
      break;
   case ZPRAVA_LOGIN_DUPLICITA:
      chyba("U¾ jste pøihlá¹en(a)", true);
      break;     
    case ZPRAVA_KLIENT_ERROR:
      chyba("Nerozumím si se serverem", true);
      break;
    case ZPRAVA_INTERNAL_ERROR:
      chyba("Vnitøní chyba serveru", true);
      break;     
    case ZPRAVA_VERZE_OK: {
      stav = STAV_PRED_LOGIN;
      char **s = (char **) malloc(sizeof(char *) *2);
      s[0] = loginEdit->getText().toUtf8();
      s[1] = hesloEdit->getText().toUtf8();
      HoThread t(vlaknoLogin, (void *) s);
      t.run();
      break;
    }
    case ZPRAVA_CHAT_OK:
      ZpracujChat(z);
      break;
    case ZPRAVA_ZIJU_OK:
      break;
    case ZPRAVA_NEKDO_LOGIN:
      ZpracujNekdoLogin(z);
      break;
    case ZPRAVA_LOGIN_OK:
      if (!parseLogin(z, &stolky, &useri)) {
        chyba("Nerozumím si se serverem", true);
	break;
      }
      mojeID = ntohl(*(int *)z->param);
      loginOkno->exitIfLast = false;
      loginOkno->destroy(); 
      {
        stolkyOkno = new StolkyWindow(NULL);
        stolkyOkno->pack();
        stolkyOkno->create();
      }
      stav = STAV_PO_LOGIN;
      {
        HoThread t(vlaknoZiju, NULL);
        t.run();
      }
      break;
    case ZPRAVA_NEKDO_LOGOUT:
      ZpracujNekdoLogout(z);
      break;
    case ZPRAVA_STOLEK_NOVY:
      ZpracujStolekNovy(z);
      break;
    case ZPRAVA_STOLEK_PRISEDL:
      ZpracujStolekPrisedl(z);
      break;
    case ZPRAVA_STOLEK_ODSEDL:
      ZpracujStolekOdsedl(z);
      break;
    case ZPRAVA_STOLEK_TAH:
      ZpracujStolekTah(z);
      break;

    default:
      HoMsgBox::msgBox(HoString("Chyba"), HoString("Neznámá zpráva", 0));
      printf("%i\n", z->typ);
      break;
  }
  delete z;
}

