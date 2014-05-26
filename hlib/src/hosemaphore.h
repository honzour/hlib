#ifndef semaforH
#define semaforH

#include "hoset.h"

#if hplatform==MW
#include <windows.h>
#endif

/**
  T��da implementuj�c� semafor pro synchronizaci vl�ken.
*/
class HoSemaphore {
  /**
    Semafor - prost�edek OS.
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
    @param nThreads kolik vl�ken najednou m��e do kritick� sekce.
  */
  HoSemaphore(int nThreads = 1);
  /**
    Provede operaci samaforu.
    @param ops kolikr�t up (-kolikr�t down).
    @return ok?
  */
  bool ctl(int ops, int s = -1, int ms = -1);
  /**
    Vstoup� do kritick� sekce.
    @return ok?
  */
  bool down();
  /**
    Vstoup� do kritick� sekce. �ek� maxim�ln� zadan� �as.
    @param s vte�iny
    @param ms tis�ciny vte�iny
    @return ok?
  */
  bool down(int s, int ms);

  /**
    Vystoup� z kritick� sekce.
    @return ok?
  */
  bool up();
  /**
    Uvoln� semafor.
  */
  ~HoSemaphore();
};
#endif

