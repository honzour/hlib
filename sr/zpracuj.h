#ifndef zpracujH
#define zpracujH
#include "zprava.h"
/**
  Reakce na ZPRAVA_ERROR. Po�le mu ihned OK a hr��e vykost�.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujError(Zprava *zprava);
/**
  Reakce na ZPRAVA_VERZE. Hnedka po�le ZPRAVA_VERZE_OK a nebo pokud chyba
  vykost�.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujVerze(Zprava *zprava);
/**
  Reakce na ZPRAVA_LOGIN. Hnedka po�le ZPRAVA_LOGIN_OK a nebo n�jakou
  chybovou zpr�vu (a vykost�). Ostat�m hr���m do fronty po�le
  ZPRAVA_NEKDO_LOGIN.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujLogin(Zprava *zprava);
/**
  Reakce na ZPRAVA_ZIJU.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujZiju(Zprava *zprava);
/**
  Reakce na ZPRAVA_CHAT.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujChat(Zprava *zprava);
/**
  Reakce na ZPRAVA_STOLEK.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujStolek(Zprava *zprava);
/**
  Reakce na ZPRAVA_PRISEDL.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujPrisedl(Zprava *zprava);
/**
  Reakce na ZPRAVA_PRESEDAVAM.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujPresedavam(Zprava *zprava);
/**
  Reakce na ZPRAVA_ODSEDL.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujOdsedl(Zprava *zprava);
/**
  Reakce na ZPRAVA_TAH.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujTah(Zprava *zprava);
/**
  Reakce na ZPRAVA_START.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujStart(Zprava *zprava);
/**
  Reakce na ZPRAVA_VZDAVAM:.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujVzdavam(Zprava *zprava);
/**
  Reakce na ZPRAVA_NAVRHUJI_REMIS.
  @param zprava hr��em zaslan� zpr�va
*/
void ZpracujRemis(Zprava *zprava);

#endif

