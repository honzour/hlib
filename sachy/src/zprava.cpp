#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "zpravy.h"
#include "zprava.h"

void Zprava::init(int typ, int delka, const unsigned char * param) {

  free();
  this->typ = typ;
  this->delka = delka;
  if (param && delka > 0) {
    this->param = (unsigned char *)malloc(delka);
    memcpy((void *)this->param, (const void *)param, delka);
    this->delka = delka;
  } else {
    this->param = NULL;
    this->delka = 0;
  }
}

Zprava::Zprava(int typ, int delka, const unsigned char * param) {
  
    this->param = NULL;
    init(typ, delka, param);
}

bool Zprava::send() {
  
  int typ = htonl(this->typ);
  int delka = htonl(this->delka);
  
  semaforPsaniSocketu->down();
  semaforSocketu->down();
  pisuSocket = true;
  if (!pSocket || nicimSocket) {
    semaforSocketu->up();
    goto sendError;
  }
  semaforSocketu->up();
  if (pSocket->send((void *)&typ, sizeof(int)) != sizeof(int)) goto sendError;
  if (pSocket->send((void *)&delka, sizeof(int)) != sizeof(int)) goto sendError;
  if (this->delka) {
    if (pSocket->send((void *)param, this->delka) != this->delka)
      goto sendError;
  }
  semaforSocketu->down();
  pisuSocket = false;
  semaforSocketu->up();
  semaforPsaniSocketu->up();
  return true;
  sendError:
  semaforSocketu->down();
  pisuSocket = false;
  if (pSocket) {
    if (pSocket->connected) pSocket->close();
    if (!pisuSocket && !ctuSocket && !nicimSocket) {
      delete pSocket;
      pSocket = NULL;
    }
  }
  semaforSocketu->up();
  semaforPsaniSocketu->up();
  initChybaSend();
  return false;
}

bool Zprava::recv() {
  free();
  int typ;
  int delka;
  semaforSocketu->down();
  ctuSocket = true;
  if (!pSocket || nicimSocket) {
    semaforSocketu->up();
    goto recvError;
  }
  semaforSocketu->up();

  if (pSocket->recv((void *)&posledni, 1) != 1) goto recvError;
  if (pSocket->recv((void *)&typ, sizeof(int)) != sizeof(int)) goto recvError;
  if (pSocket->recv((void *)&delka, sizeof(int)) != sizeof(int)) goto recvError;
  this->typ = ntohl(typ);
  this->delka = ntohl(delka);
  if (this->delka) {
    param = (unsigned char *) malloc(this->delka);
    if (pSocket->recv((void *)param, this->delka) != this->delka) goto recvError;
  }
  semaforSocketu->down();
  ctuSocket = false;
  semaforSocketu->up();
  return true;
  recvError:
  semaforSocketu->down();
  ctuSocket = false;
  if (pSocket) {
    if (pSocket->connected) pSocket->close();
    if (!pisuSocket && !ctuSocket && !nicimSocket) {
      delete pSocket;
      pSocket = NULL;
    }
  }
  semaforSocketu->up();
  initChybaRecv();
  return false;
}
    
bool Zprava::sendHTTP(HoSocket *s) {
  return false;
}
    
bool Zprava::recvHTTP(HoSocket *s) {
  return false;
}
    
void Zprava::free() {
  if (param) {
    ::free(param);
  }
  param = NULL;
}
    
Zprava::~Zprava() {
  free();
}
    
void Zprava::initLogin(char *login, char *heslo) {
  int dl, dh;
  char *str = (char *) malloc((dl = strlen(login)) + (dh =strlen(heslo)) + 2);

  memcpy(str, login, dl + 1);
  memcpy(str + dl + 1, heslo, dh + 1);
  ::free(login);
  ::free(heslo);
  init(ZPRAVA_LOGIN, dl + dh + 2, (const unsigned char *)str);
  ::free(str);
}

void Zprava::initChat(int stolek, char *chat) {
  int dc = strlen(chat) + 1;
      
  char *str = (char *) malloc(4 + dc);
  stolek = htonl(stolek);
  memcpy(str, &stolek, 4);
  memcpy(str + 4, chat, dc);

  ::free(chat);
  init(ZPRAVA_CHAT, 4 + dc, (const unsigned char *) str);
  ::free(str);
 }

    
void Zprava::initNovyStolek(bool bily, int sCelkem, int sTah) {
      
  sCelkem = htonl(sCelkem);
  sTah = htonl(sTah);
  char *str = (char *) malloc(9);
  str[0] = bily ? 1 : 0;
  memcpy(str + 1, &sCelkem, 4);
  memcpy(str + 5, &sTah, 4);
  init(ZPRAVA_STOLEK, 9, (const unsigned char *) str);
  ::free(str);
}

void Zprava::initPrisednout(int id, bool pozorovatel) {
      
  char *str = (char *) malloc(5);
  (*(int *)str) = htonl(id);
  str[4] = (char) pozorovatel;
  init(ZPRAVA_PRISEDL, 5, (const unsigned char *) str);
  ::free(str);
}

void Zprava::initTah(int id, int tah) {
  char *str = (char *) malloc(8);
  (*(int *)str) = htonl(id);
  (*(int *)(str + 4)) = htonl(tah);

  init(ZPRAVA_TAH, 8, (const unsigned char *) str);
  ::free(str);
}

void Zprava::initZiju() {
  init(ZPRAVA_ZIJU);
}

void Zprava::initOdsednout(int id) {
      
  char *str = (char *) malloc(4);
  (*(int *)str) = htonl(id);
  init(ZPRAVA_ODSEDL, 4, (const unsigned char *) str);
  ::free(str);
}

