#ifndef HoImageH
#define HoImageH
#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif
class HoImage {
public:
#if hplatform==XW
  Pixmap handle;
#elif hplatform==MW
  HBITMAP handle;
#endif
  int w, h;

  HoImage(int w, int h);
  /**
     @param rgbData rgb, 3 byty na bod, zleva doprava, shora dolu, velikost w * h * 3
  */
  HoImage(int w, int h, void *rgbData);
  /**
    Udela novy obrazek zvetsenim stareho.
  */
  HoImage(HoImage &image, int zvetseni);
  ~HoImage();
protected:
  void init(int w, int h, void *rgbData);
};
#endif

