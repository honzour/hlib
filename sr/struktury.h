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
  Struktura odpov�daj�c� jednomu klientovi.
*/
struct SCKlient {
public:
  /**
  Fronta zpr�v. Push zp�sob� okam�it� odesl�n� klientovi, pokud je toho klient
  schopen tj. pokud mu pr�v� nen� pos�l�na n�kter� ze zpr�v p�edchoz�ch.
  */
  FrontaZprav *fronta;
  /**
  Socket, kter�m je hr�� p�ipojen.
  */
  HoSocket *socket;
  /**
  Id hr��e (odpov�d� si 1 k 1 loginu).
  */
  int id;
  time_t aktiv;
  /** Maj� se zahazovat v�echny zpr�vy? Nap��klad pokud je �patn� verze.
   Nakonec ho to vykost� na timeout.*/
  bool vykostit;
  /**
   Jednozna�n� login hr��e. To, co vypl�uje p�i p�ihl�en�. 
  */
  string login;
  bool byloZiju;
  /**
  Zpr�vy, kter� vzhledem k n�vrhu dotaz-odpov�� nemohou b�t posl�ny ihned.
  �ekaj� na �iju nebo na jinou zpr�vu tohoto u�ivatele.
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
    fprintf(f, "id %i, login %s, socket %p, �as %i,"
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
Struktura reprezetuj�c� jeden stolek.
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
   Zru�� st�vaj�c� partii a vytvo�� novou.
  */
  void novaPartie();
  /**
  Pokud je partie a hrac je b�l� nebo �ern�, vzd� ho, po�le o tom zpr�vu a
  p�esedne soupe�e.
  */
  void moznaVzdej(int hrac);
  /**
   Provede odsednut� hr��e. Pokud je t�m hr��em b�l� nebo �ern�, sma�e bStart
   a cStart a nastav� �as na po��tek. Pokud je to v pr�b�hu partie,
   vzd� ho.
   @param hrac kdo si odsed�v�
   @return true pokud hr�� u stolku sed�
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

