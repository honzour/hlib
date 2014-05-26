#ifndef HoPushbuttonH
#define HoPushbuttonH
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

//class HWindow;
class HoPushButton : public HoWindow
{
public:
 void (* push)(HoPushButton *, void *);
 void *param;

 HoPushButton(HoWindow *parent, const HoString &title,
   int x, int y, int w, int h, void (* push)(HoPushButton *, void *) = NULL,
   void *param = NULL);
 HoPushButton(HoWindow *parent, const HoString &title, int w, int h,
   void (* push)(HoPushButton *, void *) = NULL, void *param = NULL); // layout
 HoPushButton(HoWindow *parent, const HoString &title,
   void (* push)(HoPushButton *, void *) = NULL, void *param = NULL); // layout

 virtual int create();
 virtual void pushed();
 virtual ~HoPushButton();
private:
 virtual void paint(HoDC *dc);
 virtual void mouseDown(int x, int y, int button);
 virtual void mouseUp(int x, int y, int button);
};
#endif
