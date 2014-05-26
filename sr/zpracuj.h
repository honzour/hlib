#ifndef zpracujH
#define zpracujH
#include "zprava.h"
/**
  Reakce na ZPRAVA_ERROR. Poπle mu ihned OK a hr·Ëe vykostÌ.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujError(Zprava *zprava);
/**
  Reakce na ZPRAVA_VERZE. Hnedka poπle ZPRAVA_VERZE_OK a nebo pokud chyba
  vykostÌ.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujVerze(Zprava *zprava);
/**
  Reakce na ZPRAVA_LOGIN. Hnedka poπle ZPRAVA_LOGIN_OK a nebo nÏjakou
  chybovou zpr·vu (a vykostÌ). OstatÌm hr·Ë˘m do fronty poπle
  ZPRAVA_NEKDO_LOGIN.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujLogin(Zprava *zprava);
/**
  Reakce na ZPRAVA_ZIJU.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujZiju(Zprava *zprava);
/**
  Reakce na ZPRAVA_CHAT.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujChat(Zprava *zprava);
/**
  Reakce na ZPRAVA_STOLEK.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujStolek(Zprava *zprava);
/**
  Reakce na ZPRAVA_PRISEDL.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujPrisedl(Zprava *zprava);
/**
  Reakce na ZPRAVA_PRESEDAVAM.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujPresedavam(Zprava *zprava);
/**
  Reakce na ZPRAVA_ODSEDL.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujOdsedl(Zprava *zprava);
/**
  Reakce na ZPRAVA_TAH.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujTah(Zprava *zprava);
/**
  Reakce na ZPRAVA_START.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujStart(Zprava *zprava);
/**
  Reakce na ZPRAVA_VZDAVAM:.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujVzdavam(Zprava *zprava);
/**
  Reakce na ZPRAVA_NAVRHUJI_REMIS.
  @param zprava hr·Ëem zaslan· zpr·va
*/
void ZpracujRemis(Zprava *zprava);

#endif

