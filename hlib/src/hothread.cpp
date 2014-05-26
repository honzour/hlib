#include <string.h>
#include "hoapp.h"
#include "hothread.h"

#if hplatform==XW

#include <X11/X.h>
#include <X11/Xlib.h>
#include <unistd.h>

#else

struct FuncParam {
  void (* threadFunc)(void *);
  void *param;
};

DWORD WINAPI hoThreadFunc(LPVOID lpParam ) 
{ 
  void (*threadFunc)(void *) = ((FuncParam *)lpParam) -> threadFunc;
  void *param = ((FuncParam *)lpParam) -> param;
  free(lpParam);

  threadFunc(param);

  return 0; 
}

#endif

bool HoThread::run() {
#if hplatform==MW
  DWORD dwThreadId;

  FuncParam *fp = (FuncParam *) malloc(sizeof(FuncParam));
  fp -> threadFunc =  threadFunc;
  fp -> param = param;
  hThread = CreateThread(NULL, 0, hoThreadFunc, (LPVOID) fp, 0, &dwThreadId);                
  if (!hThread) return false; 
  running = true;
  return true;
#else
  if (pthread_create(&hThread, NULL, (void * (*)(void *))threadFunc, param))
    return false;
  running = true;
  return true;
#endif
}

void HoThread::kill9() {
  if (running)
#if hplatform==MW
    TerminateThread(hThread, 0);
#else
    pthread_cancel(hThread);
#endif
}

void HoThread::sendEvent(void *data) {
#if hplatform==XW
  XEvent e;

  e.xclient.type = ClientMessage;
  e.xclient.display = HoMainApp->dpy;
  HoWindow *w = (HoWindow *) HoMainApp->mainWindowTree->getMinKey();
  if (!w) return;
  int i = w -> handle;
  e.xclient.window = i;
  e.xclient.send_event = True;
  e.xclient.message_type = HoMainApp->zpravaAtom;
  e.xclient.format = 32;
  e.xclient.data.l[0] = 0x87654321;
  memcpy(e.xclient.data.l + 1, &data, 4);
/*  HoMainApp->semaforZprav.down();
  frontaZprav.push(data);
  HoMainApp->semaforZprav.up();*/
  XSendEvent(HoMainApp->dpy, i, False, 0, &e);
  XFlush(HoMainApp->dpy);
#else
  PostThreadMessage(HoMainApp->threadId,  WM_USER, 0, (LPARAM) data);
#endif
}

void HoThread::sleep(unsigned ms) {
#if hplatform==XW
  usleep(1000 * ms);
#else
  Sleep(ms);
#endif
}


