#ifndef HoLayoutH
#define HoLayoutH
#include "hotree.h"

class HoWindow;

/**
Layout um� pracovat s potomky HoWindow. Ur�uje direktivn� velikost
rodi�ovsk�ho okna. HoLayout je jen abstraktn� p�edek stromu layoutu.
*/
class HoLayout{
 public:
 /**
 Otcovsk� okno
 */
 HoWindow *w; 
 
 /**
 Konstruktor p�i�azuj�c� rodi�ovsk� okno
 @param w rodi�ovsk� okno
 */
 HoLayout(HoWindow *w){this->w = w;};
 /**
 P�id�n� dce�in�ho okna
 @param w dce�inn� okno
 */
 virtual void add(HoWindow *w) = 0;
 /**
 Odstran�n� dce�in�ho okna zn�m-li ptr.
 @param w ukazatel na okno
 */
 virtual void remove(HoWindow *w) = 0;
 /**
 Po�et oken
 @return po�et oken
 */
 virtual int count() = 0;
/**
 Po�et oken je nula?
 @return po�et oken je nula?
 */
 virtual int empty() = 0;
 virtual int getX() = 0; // ���ka klientsk�ho prostoru rodi�e
 virtual int getY() = 0; // V��ka klientsk�ho prostoru rodi�e
 virtual void pack() = 0; // Nastav� velikosti
 virtual ~HoLayout(){};
};

/**
Abstraktn� p�edek implementac� obd�ln�kov�ch layout�.
*/
class HoBoxLayout: public HoLayout{ // Abstraktni predek
  public:
  /** 
   Vzd�lenost mezi dce�in�mi okny navz�jem a mezi nimi a okraji
  */
    int d,
  /**
  Po�et dce�in�ch oken te� v layouty.
  */
    cnt,
  /**
  Po�et v�ech oken, kter� kdy v layoutu byly.*/
   cntAll,
  /**
   pohybliv� sou�adnice nov�ho prvku
  */
    n,
   /**
    nejv�t�� rozm�r prvku v nepohybliv� sou�adnici (zde x)
   */
    maxm;
/**
Strom dce�in�ch oken
*/
 HoTree *t;
 HoBoxLayout(HoWindow *w, int d=5);
 virtual void add(HoWindow *w);
 virtual void remove(HoWindow *w);
 virtual int count() {return cnt;};
   virtual int empty() {return !cnt;};
 virtual ~HoBoxLayout();
};

/**
Trida implementujici obdelnikovy layout s okny pod sebou.
*/
class HoVBoxLayout: public HoBoxLayout{
  public:
    HoVBoxLayout(HoWindow *w, int d = 5);
    virtual void add(HoWindow *w);
    virtual int getX() {return maxm + 2 * d;};
    virtual int getY() {return n;};
    virtual void pack();
};

/**
Trida implementujici obdelnikovy layout s okny vedle sebe.
*/
class HoHBoxLayout: public HoBoxLayout{
  public:
    HoHBoxLayout(HoWindow *w, int d = 5);
    virtual void add(HoWindow *w);
    virtual int getX(){return n;};
    virtual int getY(){return maxm + 2 * d;};
    virtual void  pack();
};
#endif
