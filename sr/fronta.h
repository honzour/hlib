#ifndef frontaH
#define frontaH

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <hosemaphore.h>
#include "zprava.h"

/**
Fronta, do které mù¾e X vláken psát (bez blokace) a právì jedno vlákno
z ní mù¾e èíst. Pokud ve frontì není ani jedna zpráva, ètení blokuje,
dokud zpráva nepøíjde.
*/

template <class T> class Fronta {
  protected:
    std::queue<T> fronta;
    HoSemaphore hSemafor;
    HoSemaphore hCekani;
    bool cekam;
  public:
    Fronta();
    ~Fronta();
    void push(T &t);
    T pop();
    size_t size();
    bool empty();
};

class FrontaZprav : public Fronta<Zprava *> {
  void clearNull();
};
#endif

