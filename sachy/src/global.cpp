#include "global.h"
#include <hosemaphore.h>

int stav = STAV_PRED_PUSH;
int mojeID = 0;

HoSocket *pSocket = NULL;
bool ctuSocket = false;
bool pisuSocket = false;
bool nicimSocket = false;

HoSemaphore *semaforSocketu;
HoSemaphore *semaforPsaniSocketu;

HoEdit *loginEdit;
HoEdit *hesloEdit;
HoEdit *chatEdit;
HoListbox *chatListbox;

HoWindow *loginOkno;
StolkyWindow *stolkyOkno;

SCStolky stolky;
SCUseri useri;

StolkyListbox *stolkyListbox;

