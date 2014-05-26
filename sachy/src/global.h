#ifndef globalH
#define globalH

#include <hosocket.h>
#include <hoedit.h>
#include <howindow.h>
#include <hosemaphore.h>
#include <holistbox.h>
#include "stolkylistbox.h"
#include "stolkywindow.h"
#include "struktury.h"

#define STAV_PRED_PUSH 0
#define STAV_PRED_VERZE 10
#define STAV_PRED_LOGIN 20
#define STAV_PO_LOGIN 30
#define STAV_LOGOUT 40

extern int stav;
extern int mojeID;

extern HoSocket *pSocket;
extern bool ctuSocket;
extern bool pisuSocket;
extern bool nicimSocket;
extern HoSemaphore *semaforSocketu;
extern HoSemaphore *semaforPsaniSocketu;

extern HoEdit *loginEdit;
extern HoEdit *hesloEdit;
extern HoEdit *chatEdit;
extern HoListbox *chatListbox;

extern HoWindow *loginOkno;
extern StolkyWindow *stolkyOkno;

extern SCStolky stolky;
extern SCUseri useri;

extern StolkyListbox *stolkyListbox;

#endif

