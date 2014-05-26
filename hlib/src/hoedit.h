#ifndef HoEditH
#define HoEditH
#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif
#ifdef hwindowH
#error
#endif
#include "howindow.h"
#include "hostring.h"
/**
  Jednoduchý editaèní øádek.
*/
class HoEdit : public HoWindow
{
public:
/**
  Vytvoøí edlajnu obvyklé velikosti v kontajneru rodièe.
*/
 HoEdit(HoWindow *parent, const HoString &s, int nchars = 10);

/**
  Vytvoøí edlajnu zadané velikosti v kontajneru rodièe.
*/
 HoEdit(HoWindow *parent, const HoString &s, int w, int h);
/**
  Vytvoøí edlajnu zadané velikosti mimo kontajner rodièe.
*/
 HoEdit(HoWindow *parent, const HoString &s, int x, int y, int w, int h);
 virtual int create();
 /**
   nastaví titulek
   @param s nový titulek
 */
 virtual void setText(const HoString &s);

 virtual ~HoEdit();
private:
 int pos;
 virtual void paint(HoDC *dc);
 virtual void mouseDown(int x, int y, int button);
 virtual void mouseUp(int x, int y, int button);
 virtual void keyDown(wchar_t c, int code, int mods);
};
#endif

