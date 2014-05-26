#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif
#include <stdlib.h>
#include "hoapp.h"
#include "howindow.h"
#include "hostring.h"
#include "holabel.h"


HoLabel::HoLabel(HoWindow *parent, const HoString &s,int x, int y, int w, int h)
 :HoWindow(parent, s, x, y, w, h)
{
  HoAssert(parent);
}

HoLabel::HoLabel(HoWindow *parent, const HoString &s, int w, int h)
 :HoWindow(parent, s, w, h)
{
  HoAssert(parent);
}

HoLabel::HoLabel(HoWindow *parent, const HoString &s)
 :HoWindow(parent, s,
   (s.strLen() + 5) * HoMainApp->getDefaultFontX(),
   HoMainApp->getDefaultFontY()
   )
{
  HoAssert(parent);
}

int HoLabel::create(){
  char *s = title->toString();
  created = 1;
#if hplatform==MW
  handle = CreateWindow(
    "STATIC",
    s,
    WS_CHILD | WS_VISIBLE,
    x,
    y,
    w,
    h,
    parent->handle,
    NULL,
    HoMainApp->hInstance,
    NULL
    );
#elif hplatform==XW
 int whiteColor = WhitePixel(HoMainApp->dpy, DefaultScreen(HoMainApp->dpy));
 handle = XCreateSimpleWindow(HoMainApp->dpy,parent->handle, x,y,w,h,0,whiteColor, whiteColor);
 XSetWMProtocols(HoMainApp->dpy, handle, & HoMainApp->xdel,1);
 XSelectInput(HoMainApp->dpy, handle, StructureNotifyMask);
 XMapWindow(HoMainApp->dpy, handle);
 xic = XCreateIC(HoMainApp->xim, XNInputStyle, HoMainApp->my_style, XNClientWindow,
    handle, NULL);

#endif

 HoMainApp->addWindow(this);
 free(s);
 return 1;
}

void HoLabel::paint(HoDC *dc){
#if hplatform==XW
 int fx, fy;
 HoMainApp->getDefaultFontSize(&fx, &fy);
 dc->setForeground(0, 0, 0);
 dc->drawString(fx, (h - fy) / 2, *title);
#endif
}
