#ifndef frontaH
#define frontaH

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <hosemaphore.h>
#include "zprava.h"

/**
Fronta, do kter� m��e X vl�ken ps�t (bez blokace) a pr�v� jedno vl�kno
z n� m��e ��st. Pokud ve front� nen� ani jedna zpr�va, �ten� blokuje,
dokud zpr�va nep��jde.
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

