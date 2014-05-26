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
#include "hoedit.h"


HoEdit::HoEdit(HoWindow *parent, const HoString &s,int x, int y, int w, int h)
 :HoWindow(parent, s, x, y, w, h) {
  HoAssert(parent);
  pos=s.strLen();
}

HoEdit::HoEdit(HoWindow *parent, const HoString &s, int w, int h)
 :HoWindow(parent, s, w, h) {
  HoAssert(parent);
  pos=s.strLen();
}

HoEdit::HoEdit(HoWindow *parent, const HoString &s, int maxchar)
 :HoWindow(parent, s, HoMainApp->getDefaultFontX() * (maxchar + 2),
  HoMainApp->getDefaultFontY() * 2) {
  HoAssert(parent);
  pos = s.strLen();
}

int HoEdit::create() {
  char *s = title->toString();
  created = 1;
#if hplatform==MW
  handle = CreateWindow("EDIT", s, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y,
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


HoEdit::~HoEdit(){
}

void HoEdit::paint(HoDC *dc){
#if hplatform==XW
  dc->setForeground(255 << 8, 255 << 8, 255 << 8);
  dc->fillRectangle(1, 1, w - 2, h - 2);
  dc->setForeground(0, 0, 0);
  dc->fillRectangle(1, 1, 1, h - 1);
  dc->fillRectangle(w - 1, 1, w - 1, h - 1);
  dc->fillRectangle(2, 1, w - 2, 1);
  dc->fillRectangle(2, h - 1, w - 2, h - 1);
  int fx, fy;
  HoMainApp->getDefaultFontSize(&fx, &fy);
  dc->drawString(fx, (h - fy) / 2, *title);
  char *s = title->toString();
  int width = XTextWidth(HoMainApp->fnt, s, pos);
  free(s);
  dc->drawLine(
    fx + width, (h - fy) / 2 + fy, 2 * fx + width, (h - fy) / 2 + fy
  );
#endif
}

void HoEdit::setText(const HoString &s) {
  pos = 0;
  HoWindow::setText(s);
}

void HoEdit::mouseDown(int x, int y, int button){
}
void HoEdit::mouseUp(int x, int y, int button){
}
void HoEdit::keyDown(wchar_t c, int code, int mods){
#if hplatform==XW
 int i;
 switch(code){
 case Ho::KeyLeft: if(pos>0){pos--; rePaint();} break;
 case Ho::KeyRight: if(pos<title->strLen()){pos++; rePaint();} break;
 case Ho::KeyHome: if(pos>0){pos=0; rePaint();} break;
 case Ho::KeyEnd: i=title->strLen(); if(pos<i){ pos=i; rePaint();} break;
 case Ho::KeyDel:
   if(title->strLen()>pos){
     title->del(pos,1);
     rePaint();} break;
 case Ho::KeyBcksp:
   if(pos>0 && title->strLen()){
     title->del(--pos,1);
     rePaint();} break;
 default: if(c){
   title->insert(c, pos);
   pos++;
   rePaint();}
}
#endif
}
