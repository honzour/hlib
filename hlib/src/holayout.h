#ifndef HoLayoutH
#define HoLayoutH
#include "hotree.h"

class HoWindow;

/**
Layout umí pracovat s potomky HoWindow. Urèuje direktivnì velikost
rodièovského okna. HoLayout je jen abstraktní pøedek stromu layoutu.
*/
class HoLayout{
 public:
 /**
 Otcovské okno
 */
 HoWindow *w; 
 
 /**
 Konstruktor pøiøazující rodièovské okno
 @param w rodièovské okno
 */
 HoLayout(HoWindow *w){this->w = w;};
 /**
 Pøidání dceøiného okna
 @param w dceøinné okno
 */
 virtual void add(HoWindow *w) = 0;
 /**
 Odstranìní dceøiného okna znám-li ptr.
 @param w ukazatel na okno
 */
 virtual void remove(HoWindow *w) = 0;
 /**
 Poèet oken
 @return poèet oken
 */
 virtual int count() = 0;
/**
 Poèet oken je nula?
 @return poèet oken je nula?
 */
 virtual int empty() = 0;
 virtual int getX() = 0; // Šíøka klientského prostoru rodièe
 virtual int getY() = 0; // Výška klientského prostoru rodièe
 virtual void pack() = 0; // Nastaví velikosti
 virtual ~HoLayout(){};
};

/**
Abstraktní pøedek implementací obdélníkových layoutù.
*/
class HoBoxLayout: public HoLayout{ // Abstraktni predek
  public:
  /** 
   Vzdálenost mezi dceøinými okny navzájem a mezi nimi a okraji
  */
    int d,
  /**
  Poèet dceøiných oken teï v layouty.
  */
    cnt,
  /**
  Poèet v¹ech oken, které kdy v layoutu byly.*/
   cntAll,
  /**
   pohyblivá souøadnice nového prvku
  */
    n,
   /**
    nejvìt¹í rozmìr prvku v nepohyblivé souøadnici (zde x)
   */
    maxm;
/**
Strom dceøiných oken
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
