#ifndef HoLabelH
#define HoLabelH
#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif
#include "howindow.h"
#include "hostring.h"

/**
Nápis v rodièovském oknì.
*/
class HoLabel : public HoWindow
{
public:
  /**
    Vytvoøí label v layoutu rodièe. Velikost labelu se spoèítá sama.
    @param parent rodiè
    @param s nápis
  */
  HoLabel(HoWindow *parent, const HoString &s);

  /**
    Vytvoøí label zadané velikosti v layoutu rodièe.
    @param parent rodiè
    @param s nápis
    @param w ¹íøka
    @param h vý¹ka
  */
  HoLabel(HoWindow *parent, const HoString &s, int w, int h);
  
   /**
    Vytvoøí label zadané velikosti mimo layout rodièe.
    @param parent rodiè
    @param s nápis
    @param x levý horní roh v souøadnicích otce
    @param y levý horní roh v souøadnicích otce
    @param w ¹íøka
    @param h vý¹ka
  */
  HoLabel(HoWindow *parent, const HoString &s, int x, int y, int w, int h);
  
  /**
    Vytvoøí GUI objekt.
  */
  virtual int create();
  
  /**
    Virtuální destruktor.
  */
  virtual ~HoLabel() {};
private:
  virtual void paint(HoDC *dc);
};
#endif

