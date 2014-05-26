#ifndef HoDcH
#define HoDcH
#include "hoset.h"
#include "hostring.h"
#include "hoimage.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif
class HoWindow;
/**
  T��da odpov�daj�c� GC (XWindows) nebo DC (MS Windows). M� na starosti
  kreslen� do oken nebo do obr�zk�. M� sou�adn� syst�m, lev� horn� roh
  m� [0,0].
*/
class HoDC {
  public:
#if hplatform==XW
  bool client;
  XColor f, b;
  GC
#elif hplatform==MW
  HDC
#endif
  handle;
  int w, h;

#if hplatform==MW
  int r, g, b;
  HBRUSH br;
  HPEN pe;
  HBITMAP bmp;
  int prvni, byloGetDC;
#endif

/**
  Okno, jeho� je DC nebo NULL.
*/
   HoWindow *window;
/**
  Image, jej� je DC nebo NULL.
*/
   HoImage *img;
   
#if hplatform==XW
   /**
   Zkonstruuje HoDC z okna bez vytvo�en�ho GC/DC.
   @param withBorder ignorovan� perametr kv�li debiln�mu C++ 
   @param w okno
   */
   HoDC(bool withBorder, HoWindow *w);
#endif

   HoDC(HoWindow *w);
   HoDC(HoWindow *w, // uz mam GC/DC
#if hplatform==XW
     GC
#elif hplatform==MW
     HDC
#endif
     handle, int width, int height);
    /**
    Zkonstruuje HoDC pro kreslen� do obr�zku.
    @param img obr�zek
    */
    HoDC(HoImage *img);
    /**
    Nakresl� bod aktu�ln� barvou.
    @param x kam se bude kreslit.
    @param y kam se bude kreslit.
    */
    void putPixel(int x, int y);
	/**
    Nakresli obdelnik barvou popredi.
	@param x souradnice leveho horniho rohu
	@param y souradnice leveho horniho rohu
	@param w sirka v bodech
	@param h vyska v bodech
	*/
    void fillRectangle(int x, int y, int w, int h);
	/**
	Nakresli caru sirky jednoho pixelu barvou popredi.
	@param x souradnice jednoho konce.
	@param y souradnice jednoho konce.
	@param x2 souradnice druheho konce.
	@param y2 souradnice druheho konce.
	*/
    void drawLine(int x, int y, int x2, int y2);
    /**
      Nap�e �et�zec vybran�m fontem.
    */
    void drawString(int x, int y, const HoString &s);
    void fillEllipse(int x, int y, int width, int height);
    void setForeground(int r, int g, int b);
	/**
	Vrati velikost aktualniho 
	*/
    void getFontInfo(int *x, int *y);
    /**
     Okop�ruje do dc kus obr�zku.
     @param x kam do dc x
     @param y kam do dc y
     @param w velikost kop�rovan�ho x
     @param h velikost kop�rovan�ho y
     @param img obr�zek, ze kter�ho se kop�ruje
     @param ix odkud v obr�zku se kop�ruje x
     @param iy odkud v obr�zku se kop�ruje y
    */
    bool putImage(int x, int y, int w, int h, HoImage *img, int ix, int iy);
    /**
      @return sirka DC v bodech
    */
    int getW();
    /**
      @return vyska DC v bodech
    */
    int getH();
   ~HoDC();
};
#endif
