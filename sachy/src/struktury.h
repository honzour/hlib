#ifndef strukturyH
#define strukturyH
#include <map>
#include <vector>

#include <hostring.h>

#include "sachwindow.h"
#include "zprava.h"

using namespace std;

struct SCUser {
  int id;
  HoString login;
 
  SCUser(int id, const HoString &login) {
    this->id = id;
    this->login = login;
  };
  
  void init(const SCUser &u) {
    this->id = u.id;
    this->login = login;
  };
  
  SCUser(const SCUser &u) {
    init(u);
  };
  
  SCUser & operator =(const SCUser &u) {
    init(u);
    return *this;
  };

  void print();
};

typedef map<int, SCUser *> SCSimpleUseri;

class SCUseri: public SCSimpleUseri {
  public:
    void print();
    void clearDelete();
};

struct SCStolek {
  int id;
  int bily;
  int cerny;
  int celkem;
  int tah;
  vector<int> prihlizi;
  SachWindow *sw;
  
  SCStolek(int id, int bily, int cerny, int celkem, int tah) {
    this->id = id;
    this->bily = bily;
    this->cerny = cerny;
    this->celkem = celkem;
    this->tah = tah;
    sw = NULL;
  }
  
  void init(const SCStolek &s) {
    this->id = s.id;
    this->bily = s.bily;
    this->cerny = s.cerny;   
    this->prihlizi = s.prihlizi;
    this->sw = s.sw;
  };
  
  SCStolek(const SCStolek &s) {
    init(s);
  };
  
  SCStolek & operator = (const SCStolek &s) {
    init(s);
    return *this;
  };

  bool prisedl(int hrac, bool pozorovatel);
};

typedef map<int, SCStolek *> SCSimpleStolky;

class SCStolky: public SCSimpleStolky {
  public:
    void clearDelete();
    bool prisedl(int stolek, int hrac, bool pozorovatel);
    bool odsedl(int stolek, int hrac);
};

bool parseLogin(const Zprava *z, SCStolky *s, SCUseri *u);

#endif

