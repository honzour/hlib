#ifndef strukturyH
#define strukturyH
#include <stdio.h>
#include <time.h>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <hosocket.h>
#include "sachy.h"
#include "zprava.h"
#include "fronta.h"

using namespace std;
/**
  Struktura odpovídající jednomu klientovi.
*/
struct SCKlient {
public:
  /**
  Fronta zpráv. Push zpùsobí okam¾ité odeslání klientovi, pokud je toho klient
  schopen tj. pokud mu právì není posílána nìkterá ze zpráv pøedchozích.
  */
  FrontaZprav *fronta;
  /**
  Socket, kterým je hráè pøipojen.
  */
  HoSocket *socket;
  /**
  Id hráèe (odpovídá si 1 k 1 loginu).
  */
  int id;
  time_t aktiv;
  /** Mají se zahazovat v¹echny zprávy? Napøíklad pokud je ¹patná verze.
   Nakonec ho to vykostí na timeout.*/
  bool vykostit;
  /**
   Jednoznaèný login hráèe. To, co vyplòuje pøi pøihlá¹ení. 
  */
  string login;
  bool byloZiju;
  /**
  Zprávy, které vzhledem k návrhu dotaz-odpovìï nemohou být poslány ihned.
  Èekají na ¾iju nebo na jinou zprávu tohoto u¾ivatele.
  */
  queue<Zprava *> zpravy;


  SCKlient() {
    aktiv = time(NULL);
    socket = NULL;
    fronta = NULL;
    id = 0;
    vykostit = false;
    byloZiju = false;
  };
  
  SCKlient(HoSocket *socket, string login);

  void init(const SCKlient &k) {
    fronta = k.fronta;
    socket = k.socket;
    aktiv = k.aktiv;
    id = k.id;
    vykostit = k.vykostit;
    login = k.login;
    byloZiju = k.byloZiju;
  };
  
  SCKlient(const SCKlient &k) {
    init(k);
  };

  SCKlient & operator =(const SCKlient &k) {
    init(k);
    return *this;
  };

  ~SCKlient() {
    if (fronta) delete fronta;
    if (socket) delete socket;
  }

  
  void smazFrontu() {
    while (!zpravy.empty()) {
      delete zpravy.front();
      zpravy.pop();
    }
  }

  void debugPrint(FILE *f) const {
    fprintf(f, "id %i, login %s, socket %p, èas %i,"
               " kostit %i\n",
      id, login.c_str(), socket,
      (int) (time(NULL) - aktiv), (int) vykostit);
  }
};

typedef map<HoSocket *, SCKlient *> SCSimpleKlienti;

struct SCKlienti: public SCSimpleKlienti {
  void debugPrint(FILE *f) const;
};

/**
Struktura reprezetující jeden stolek.
*/
struct SCStolek {
  int bily, cerny; // 0 = nikdo
  int id;
  int sCelkem, sTah;
  bool bStart, cStart;
  bool bRemis, cRemis; // TODO
  int msBCas, msCCas;
  TUloha *uloha;

  SCStolek();
  SCStolek(int id, int hrac, bool bily, int sCelkem, int sTah);
  ~SCStolek();
  void init();
  bool prisedl(int hrac, int kam /* -1 cerny, 0 prihl, 1 bily*/);
  /**
   Zru¹í stávající partii a vytvoøí novou.
  */
  void novaPartie();
  /**
  Pokud je partie a hrac je bílý nebo èerný, vzdá ho, po¹le o tom zprávu a
  pøesedne soupeøe.
  */
  void moznaVzdej(int hrac);
  /**
   Provede odsednutí hráèe. Pokud je tím hráèem bílý nebo èerný, sma¾e bStart
   a cStart a nastaví èas na poèátek. Pokud je to v prùbìhu partie,
   vzdá ho.
   @param hrac kdo si odsedává
   @return true pokud hráè u stolku sedí
  */
  bool odsedl(int hrac);
  bool presedli();
  vector<int> prihlizejici;
  void print(FILE *f);
};

typedef map <int, SCStolek *> SCSimpleStolky;

struct SCStolky: public SCSimpleStolky {
  int firstFree();
  int novy(int hrac, bool bily, int sCelkem, int sTah);
  bool prisedl(int stolek, int hrac, int kam /* -1 cerny, 0 prihl, 1 bily*/);
  bool odsedl(int stolek, int hrac);
  bool presedli(int stolek);
  void print(FILE *f);
};

struct FrontaSocket {
  FrontaZprav *f;
  HoSocket *s;
};
#endif

