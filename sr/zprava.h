#ifndef zpravaH
#define zpravaH

#include <stdlib.h>
#include <string.h>
#include <hosocket.h>

struct Zprava {
  HoSocket *socket;
  /**
    1 pokud nenásledují bezprostøednì dal¹í zprávy, 0 jindy
  */
  char posledni;
  int typ;
  int delka;
  unsigned char *param;


  Zprava(int typ) {
    socket = NULL;
    posledni = 1;
    this->typ = typ;
    delka = 0;
    param = NULL;
  };

  Zprava(int typ, int delka) {
    socket = NULL;
    posledni = 1;
    this->typ = typ;
    this->delka = delka;
    param = (unsigned char *) malloc(delka);
  };

  Zprava() {
    Zprava(0);
  };


  void init(const Zprava &z) {
    socket = z.socket;
    posledni = z.posledni;
    typ = z.typ;
    delka = z.delka;
    param = (unsigned char *) malloc(delka);
    memcpy(param, z.param, delka);
  }
  
  Zprava(const Zprava &z) {
    init(z);
  }

  Zprava & operator=(const Zprava &z) {
    init(z);
    return *this;
  }
  
  void free() {
    if (param) {
      ::free(param);
      param = NULL;
    }
  };
  
  ~Zprava() {
    free();
  };
  
};

#endif

