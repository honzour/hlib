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
Tøída - obal vlákna
*/
class HoThread {
  /**
  Systémový identrifikátor vlákna
  */
#if hplatform==XW
  pthread_t
#else
  HANDLE
#endif
  hThread;
public:
  /**
  Bì¾í právì teï vlákno?
  */
  bool running;
  /**
  Parametr nastavený v konstruktoru pøedaný funkci vlákna.
  */
  void *param;
  /**
  Funkce vlákna.
  */
  void (* threadFunc)(void *);
  /**
  Konstruktor nastavením promìnných.
  @param threadFunc funkce vlákna
  @param param parametr funkce vlákna
  */
  HoThread(void (* threadFunc)(void *), void *param = NULL) {
    running = false;
    this -> threadFunc = threadFunc;
    this -> param = param;
  }
  /**
  Spustí vlákno.
  @return true pokud OK
  */
  bool run();
  /**
  Zabije vlákno.
  */
  void kill9();
  /**
  Prázdný destruktor.
  */
  ~HoThread() {};
  /**
  Po¹le zprávu hlavnímu vláknu. Zaøadí zprávu do fronty, a¾ na ni dojde
  øada, zavolá
  if (HoMainApp->userEvent) HoMainApp->userEvent(data);
  Na XWindows pouze pokud existuje alespoò jedno okno
  @param data parametr pøedaný ve zprávì
  */
  static void sendEvent(void *data);
  /**
  Pocka x milisekund
  */
  static void sleep(unsigned ms);
};
#endif

