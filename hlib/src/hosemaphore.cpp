#include "hoset.h"

#if hplatform==XW
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#else
#include <windows.h>
#endif
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "hosemaphore.h"
#include "holog.h"

HoSemaphore::HoSemaphore(int nThreads) {
  
  if (nThreads <= 0 || nThreads > 1024) {
#if hplatform==XW
    handle = -1;
#else
    handle = NULL;
#endif
    return;
  }
  handle = 
#if hplatform==XW
    semget(IPC_PRIVATE, 1, 0700);
  ctl(nThreads);
#else
  CreateSemaphore(NULL, 1, 1, NULL);
#endif
}

bool HoSemaphore::ctl(int ops, int s, int ms) {
  bool timeout = (s >= 0 && ms >= 0 && (s > 0 || ms > 0));
#if hplatform==XW
  struct sembuf op;
  memset(&op, 0, sizeof(op));
  op.sem_num = 0;
  op.sem_op = ops;
  op.sem_flg = 0;
  struct timespec ts;
  ts.tv_sec = s;
  ts.tv_nsec = 1000000 * ms;
  return !semtimedop(handle, &op, 1, timeout ? &ts : NULL);
#else
  if (!ops) return true;
  if (ops < 0) {
    return WaitForSingleObject(handle, timeout ? ms + s * 1000 : INFINITE)
      == WAIT_OBJECT_0; 
  } else return (bool) ReleaseSemaphore(handle, 1, NULL);
#endif
}

bool HoSemaphore::down(int s, int ms) {
  return ctl(-1, s, ms);
}

bool HoSemaphore::down() {
  return ctl(-1);
}

bool HoSemaphore::up() {
  return ctl(1);
}

HoSemaphore::~HoSemaphore() {
  if (handle > 0) {
#if hplatform==XW
    semctl(handle, 0, IPC_RMID, 0);
#else
    CloseHandle(handle);
#endif
  }
}

