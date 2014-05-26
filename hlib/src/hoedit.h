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
  Jednoduch� edita�n� ��dek.
*/
class HoEdit : public HoWindow
{
public:
/**
  Vytvo�� edlajnu obvykl� velikosti v kontajneru rodi�e.
*/
 HoEdit(HoWindow *parent, const HoString &s, int nchars = 10);

/**
  Vytvo�� edlajnu zadan� velikosti v kontajneru rodi�e.
*/
 HoEdit(HoWindow *parent, const HoString &s, int w, int h);
/**
  Vytvo�� edlajnu zadan� velikosti mimo kontajner rodi�e.
*/
 HoEdit(HoWindow *parent, const HoString &s, int x, int y, int w, int h);
 virtual int create();
 /**
   nastav� titulek
   @param s nov� titulek
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

