#include <stdio.h>
#include <stdlib.h>
#include "howindow.h"
#include "hopushbutton.h"
#include "hoapp.h"
HoPushButton::HoPushButton(
  HoWindow *parent,
  const HoString &title,
  int x, int y, int w, int h,
  void (* push)(HoPushButton *b, void *param),
  void *param
  ):
    HoWindow(parent, title, x, y, w, h)
{
 
  HoAssert(parent);
  this->push = push;
  this->param = param;
}

HoPushButton::HoPushButton(HoWindow *parent,const HoString &title,
  int w, int h, void (* push)(HoPushButton *b, void *param), void *param):
    HoWindow(parent, title, w, h)
{
  HoAssert(parent);
  this->push = push;
  this->param = param;
}

HoPushButton::HoPushButton(HoWindow *parent,const HoString &title,
  void (* push)(HoPushButton *b, void *param), void *param):
    HoWindow(parent, title,
    HoMainApp->getDefaultFontX() * (title.strLen() + 5),
    HoMainApp->getDefaultFontY() * 3)
{
  HoAssert(parent);
  this->push = push;
  this->param = param;
}

void HoPushButton::pushed(){
  if (push) push(this, param);
}

void HoPushButton::paint(HoDC *dc){
#if hplatform==XW
 dc->setForeground(214 << 8, 214 << 8, 206 << 8);
 dc->fillRectangle(1, 1, w - 3, h - 3);
 dc->setForeground(132 << 8, 132 << 8, 132 << 8);
 dc->fillRectangle(w - 2, 1, 1, h - 2);
 dc->fillRectangle(1, h - 2, w - 2, 1);
 dc->setForeground(66 << 8, 66 << 8, 66 << 8);
 dc->fillRectangle(w - 1, 0, 1, h);
 dc->fillRectangle(0, h - 1, w, 1);
 dc->drawString(20, 10, *title);
#endif
}

int HoPushButton::create(){
  char *s=title->toString();
   created=1;
#if hplatform==MW
   handle=CreateWindow("BUTTON", s, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,x, y,
    w,
    h,
    parent->handle,
    NULL,
    HoMainApp->hInstance,
    NULL);
 #elif hplatform==XW
 int whiteColor = WhitePixel(HoMainApp->dpy, DefaultScreen(HoMainApp->dpy));
 handle=XCreateSimpleWindow(HoMainApp->dpy,parent->handle, x,y,w,h,0,whiteColor, whiteColor);
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
void HoPushButton::mouseDown(int x, int y, int button){
#if hlpatform==XW
 if(button)return;
 HoDC dc(this);
 dc.setForeground(255<<8,255<<8,255<<8);
 dc.fillRectangle(1,h-2,w-1,h-1);
 dc.fillRectangle(w-2,0,w-1,h-2);
 dc.setForeground(66<<8,66<<8,66<<8);
 dc.fillRectangle(0,1,1,h-2);
 dc.fillRectangle(0,0,w-2,1);
 dc.setForeground(132<<8,132<<8,132<<8);
 dc.fillRectangle(1,2,2,h-3);
 dc.fillRectangle(1,1,w-3,2);

#endif
}
void HoPushButton::mouseUp(int x, int y, int button){
#if hlpatform==XW
 if(button)return;
 HoDC dc(this);
 paint(&dc);
 if(x>=0 && y>=0 && x<w && y<h)pushed();
#endif
}
HoPushButton::~HoPushButton(){
}

