#include "fronta.h"
#include "zprava.h"

template<class T> Fronta<T>::Fronta() {
  hCekani.down();
  cekam = false;
}

template<class T> Fronta<T>::~Fronta() {
}

template<class T> void Fronta<T>::push(T &t) {
  hSemafor.down();
  fronta.push(t);
  if (cekam) {
    cekam = false;
    hCekani.up();
  }
  hSemafor.up();
}

    
template<class T> T Fronta<T>::pop() {
  hSemafor.down();
  while (fronta.empty()) {
    cekam = true;
    hSemafor.up();
    hCekani.down();
    hSemafor.down();
  }
  T t = fronta.front();
  fronta.pop();
  hSemafor.up();
  return t;
}

template<class T> size_t Fronta<T>::size() {
  hSemafor.down();
  size_t tmp = fronta.size();
  hSemafor.up();
  return tmp;
}

template<class T> bool Fronta<T>::empty() {
  hSemafor.down();
  bool tmp = fronta.empty();
  hSemafor.up();
  return tmp;
}

template class Fronta<Zprava *>;

void FrontaZprav::clearNull() {
  hSemafor.down();
  while (!fronta.empty()) {
    delete fronta.front();
    fronta.pop();
  }
  fronta.push(NULL);
  if (cekam) {
    cekam = false;
    hCekani.up();
  }
  hSemafor.up();
}

/*


#define KLIENTU 3

Fronta<unsigned> fronta;

//std::queue<unsigned> writeFronty[KLIENTU];


DWORD WINAPI KlientRead(LPVOID iValue) {
  unsigned kod = *(unsigned *)iValue;

  printf("klient read %i\n", kod);

  while (1) {
    Sleep(rand() % 10000);
    printf("push %i\n", kod);
    fronta.push(kod);
  }


  return 0;
}


DWORD WINAPI Vlakno2(LPVOID iValue) {
  
  puts("Vlakno 2");

  while (1) {
    unsigned kod = fronta.pop();
    printf("pop %i\n", kod);
  }


  return 0;
}



DWORD WINAPI KlientWrite(LPVOID iValue) {
  int kod = *(int *)iValue;

  printf("klient write %i\n", kod);
  return 0;
}



int main(void) {
  DWORD dwGenericThread;





  int klienti[KLIENTU];
  int i;

  for (i = 0; i < KLIENTU; i++) {
    klienti[i] = i;
  }
  

  for (i = 0; i < KLIENTU; i++) {
    CreateThread(NULL,0,KlientRead,klienti + i,0,&dwGenericThread);
  }


  CreateThread(NULL,0,Vlakno2,NULL,0,&dwGenericThread);
 
  Sleep(30000);

  return 0;
}

*/

