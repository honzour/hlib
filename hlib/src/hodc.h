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
  Tøída odpovídající GC (XWindows) nebo DC (MS Windows). Má na starosti
  kreslení do oken nebo do obrázkù. Má souøadný systém, levý horní roh
  má [0,0].
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
  Okno, jeho¾ je DC nebo NULL.
*/
   HoWindow *window;
/**
  Image, její¾ je DC nebo NULL.
*/
   HoImage *img;
   
#if hplatform==XW
   /**
   Zkonstruuje HoDC z okna bez vytvoøeného GC/DC.
   @param withBorder ignorovaný perametr kvùli debilnímu C++ 
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
    Zkonstruuje HoDC pro kreslení do obrázku.
    @param img obrázek
    */
    HoDC(HoImage *img);
    /**
    Nakreslí bod aktuální barvou.
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
      Napí¹e øetìzec vybraným fontem.
    */
    void drawString(int x, int y, const HoString &s);
    void fillEllipse(int x, int y, int width, int height);
    void setForeground(int r, int g, int b);
	/**
	Vrati velikost aktualniho 
	*/
    void getFontInfo(int *x, int *y);
    /**
     Okopíruje do dc kus obrázku.
     @param x kam do dc x
     @param y kam do dc y
     @param w velikost kopírovaného x
     @param h velikost kopírovaného y
     @param img obrázek, ze kterého se kopíruje
     @param ix odkud v obrázku se kopíruje x
     @param iy odkud v obrázku se kopíruje y
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
