#ifndef HoThreadH
#define HoThreadH
#include "hoset.h"
#if hplatform==XW
#include <pthread.h>
#elif hplatform==MW
#include <windows.h>
#endif
#if hplatform==XW
#include <queue>
extern std::queue<void *> frontaZprav;
#endif


/**
T��da - obal vl�kna
*/
class HoThread {
  /**
  Syst�mov� identrifik�tor vl�kna
  */
#if hplatform==XW
  pthread_t
#else
  HANDLE
#endif
  hThread;
public:
  /**
  B�� pr�v� te� vl�kno?
  */
  bool running;
  /**
  Parametr nastaven� v konstruktoru p�edan� funkci vl�kna.
  */
  void *param;
  /**
  Funkce vl�kna.
  */
  void (* threadFunc)(void *);
  /**
  Konstruktor nastaven�m prom�nn�ch.
  @param threadFunc funkce vl�kna
  @param param parametr funkce vl�kna
  */
  HoThread(void (* threadFunc)(void *), void *param = NULL) {
    running = false;
    this -> threadFunc = threadFunc;
    this -> param = param;
  }
  /**
  Spust� vl�kno.
  @return true pokud OK
  */
  bool run();
  /**
  Zabije vl�kno.
  */
  void kill9();
  /**
  Pr�zdn� destruktor.
  */
  ~HoThread() {};
  /**
  Po�le zpr�vu hlavn�mu vl�knu. Za�ad� zpr�vu do fronty, a� na ni dojde
  �ada, zavol�
  if (HoMainApp->userEvent) HoMainApp->userEvent(data);
  Na XWindows pouze pokud existuje alespo� jedno okno
  @param data parametr p�edan� ve zpr�v�
  */
  static void sendEvent(void *data);
  /**
  Pocka x milisekund
  */
  static void sleep(unsigned ms);
};
#endif

