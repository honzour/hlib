#ifndef semaforH
#define semaforH

#include "hoset.h"

#if hplatform==MW
#include <windows.h>
#endif

/**
  Tøída implementující semafor pro synchronizaci vláken.
*/
class HoSemaphore {
  /**
    Semafor - prostøedek OS.
  */
#if hplatform==XW
  int
#else
  HANDLE
#endif
    handle;
public:
  /**
    Konstruktor.
    @param nThreads kolik vláken najednou mù¾e do kritické sekce.
  */
  HoSemaphore(int nThreads = 1);
  /**
    Provede operaci samaforu.
    @param ops kolikrát up (-kolikrát down).
    @return ok?
  */
  bool ctl(int ops, int s = -1, int ms = -1);
  /**
    Vstoupí do kritické sekce.
    @return ok?
  */
  bool down();
  /**
    Vstoupí do kritické sekce. Èeká maximálnì zadaný èas.
    @param s vteøiny
    @param ms tisíciny vteøiny
    @return ok?
  */
  bool down(int s, int ms);

  /**
    Vystoupí z kritické sekce.
    @return ok?
  */
  bool up();
  /**
    Uvolní semafor.
  */
  ~HoSemaphore();
};
#endif

