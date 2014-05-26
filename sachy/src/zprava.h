#ifndef zpravaH
#define zpravaH

#include <hoedit.h>
#include <hosocket.h>
#include "zpravy.h"
#include "onevent.h"

class Zprava {
  public:
    char posledni;
    int typ;
    int delka;
    unsigned char *param;
    void initChybaSend() {
      init(CHYBA_SEND);
      posledni = 1;
    }
    void initChybaRecv() {
      init(CHYBA_RECV);
      posledni = 1;
    }
    void init(int typ = 0, int delka = 0, const unsigned char * param = NULL);
    Zprava(int typ = 0, int delka = 0, const unsigned char * param = NULL);
    bool send();
    bool recv();
    bool sendHTTP(HoSocket *s);
    bool recvHTTP(HoSocket *s);
    void free();
    ~Zprava();
    void initLogin(char *login, char *heslo);
    void initChat(int stolek, char *chat);
    void initTah(int stolek, int tah);
    void initNovyStolek(bool bily, int sCelkem, int sTah);
    void initPrisednout(int id, bool pozorovatel);
    void initZiju();
    void initOdsednout(int id);
};

#endif

