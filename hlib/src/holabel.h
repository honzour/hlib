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
N�pis v rodi�ovsk�m okn�.
*/
class HoLabel : public HoWindow
{
public:
  /**
    Vytvo�� label v layoutu rodi�e. Velikost labelu se spo��t� sama.
    @param parent rodi�
    @param s n�pis
  */
  HoLabel(HoWindow *parent, const HoString &s);

  /**
    Vytvo�� label zadan� velikosti v layoutu rodi�e.
    @param parent rodi�
    @param s n�pis
    @param w ���ka
    @param h v��ka
  */
  HoLabel(HoWindow *parent, const HoString &s, int w, int h);
  
   /**
    Vytvo�� label zadan� velikosti mimo layout rodi�e.
    @param parent rodi�
    @param s n�pis
    @param x lev� horn� roh v sou�adnic�ch otce
    @param y lev� horn� roh v sou�adnic�ch otce
    @param w ���ka
    @param h v��ka
  */
  HoLabel(HoWindow *parent, const HoString &s, int x, int y, int w, int h);
  
  /**
    Vytvo�� GUI objekt.
  */
  virtual int create();
  
  /**
    Virtu�ln� destruktor.
  */
  virtual ~HoLabel() {};
private:
  virtual void paint(HoDC *dc);
};
#endif

