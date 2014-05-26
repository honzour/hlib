#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hoset.h"
#include "hodc.h"
#include "hostring.h"
#include "hoapp.h"
#include "hoimage.h"

HoDC::HoDC(HoImage *img) {
  window = NULL;
  this->img = img;
#if hplatform==XW
  client = true;
  handle = XCreateGC(HoMainApp->dpy, img->handle, 0, 0);
  if (HoMainApp->fnt)
    XSetFont(HoMainApp->dpy, handle, HoMainApp->fnt->fid);
#elif hplatform==MW
  handle = CreateCompatibleDC(NULL);
  SetBkMode(handle, TRANSPARENT);
  byloGetDC = 0;
  prvni = 0;
  bmp  = (HBITMAP) SelectObject(handle, img->handle);
#endif
  this->w = img->w;
  this->h = img->h;
  setForeground(0, 0, 0);
}

#if hplatform==XW
HoDC::HoDC(bool withBorder, HoWindow *w) {
  client = false;
  window = w;
  img = NULL;
  handle = XCreateGC(HoMainApp->dpy, w->handleBorder, 0, 0);
  int x, y;
  unsigned width, h, bw, d;
  Window win;

  XGetGeometry(HoMainApp->dpy, w->handleBorder, &win, &x, &y, &width, &h, &bw, &d);
  this->w = width;
  this->h = h;
  if (HoMainApp->fnt)
    XSetFont(HoMainApp->dpy, handle, HoMainApp->fnt->fid);
  setForeground(0, 0, 0);
}
#endif

HoDC::HoDC(HoWindow *w) {
  window = w;
  img = NULL;
#if hplatform==XW
  client = true;
  handle = XCreateGC(HoMainApp->dpy, w->handle, 0, 0);
  int x, y;
  unsigned width, h, bw, d;
  Window win;

  XGetGeometry(HoMainApp->dpy, w->handle, &win, &x, &y, &width, &h, &bw, &d);
  this->w = width - 2 * bw;
  this->h = h - 2 * bw;

  if (HoMainApp->fnt)
    XSetFont(HoMainApp->dpy, handle, HoMainApp->fnt->fid);
#elif hplatform==MW
  handle = GetDC(w->handle);
  SetBkMode(handle, TRANSPARENT);
  byloGetDC = 1;
  prvni = 0;
  RECT r;
  GetClientRect(w->handle, &r);
  this->w = r.right;
  this->h = r.bottom;
#endif
  setForeground(0, 0, 0);
}

HoDC::HoDC(HoWindow *w, // uz mam GC/DC - pozor jen na MS Windows
#if hplatform==XW
  GC
#elif hplatform==MW
  HDC
#endif
  handle, int width, int height) {
#if hplatform==MW
  prvni=0;
  byloGetDC=0;
  SetBkMode(handle,TRANSPARENT);
#endif
  this->window = w;
  this->handle = handle;
  setForeground(0, 0, 0);
  this->w = width;
  this->h = height;
}
void HoDC::getFontInfo(int *x, int *y){
#if hplatform==MW
  TEXTMETRIC tm;

  if(!x && !y) return;
  GetTextMetrics(handle, &tm);
  if (x) *x = tm.tmAveCharWidth;
  if (y) *y = tm.tmHeight;
#else
 if (x)
   *x = (HoMainApp->fnt->ascent + HoMainApp->fnt->descent) >> 1;
 if (y)
   *y = HoMainApp->fnt->ascent + HoMainApp->fnt->descent;
#endif
}

HoDC::~HoDC(){
#if hplatform==MW
  if (prvni) {
    br = (HBRUSH)SelectObject(handle, br);
    pe = (HPEN)SelectObject(handle, pe);
    DeleteObject(br);
    DeleteObject(pe);
  }
  if (byloGetDC) ReleaseDC(window->handle, handle);
  if (img) {
    SelectObject(handle, bmp);
    DeleteDC(handle);
  }
#elif hplatform==XW
  if (window) {
    XFlush(HoMainApp->dpy);
  }
  XFreeGC(HoMainApp->dpy, handle);
#endif
}

void HoDC::drawString(int x, int y, const HoString &s){
  char *str=s.toString();
#if hplatform==XW
  XDrawString(
    HoMainApp->dpy,
    (img ?  img->handle :
      (client ? window->handle : window->handleBorder)
    ),
    handle,
    x,
    y + HoMainApp->fnt->ascent,
    str,
    strlen(str)
  );
#elif hplatform==MW
  TextOut(handle, x, y, str, strlen(str));
#endif
  free(str);
}

void HoDC::setForeground(int r, int g, int b){
#if hplatform==XW
  f.pixel = 1;
  f.red = (unsigned short) r;
  f.green = (unsigned short) g;
  f.blue = (unsigned short) b;

  XAllocColor(
    HoMainApp->dpy,
    DefaultColormap(HoMainApp->dpy, DefaultScreen(HoMainApp->dpy)),
    &f
  );
  XSetForeground(HoMainApp->dpy, handle , f.pixel);
#elif hplatform==MW
  SetTextColor(handle, RGB(this->r = r >> 8, this->g = g >> 8,
   this->b = b >> 8));
  HBRUSH br = CreateSolidBrush(RGB(r >> 8, g >> 8, b >> 8));
  this->br = (HBRUSH)SelectObject(handle, br);
  HPEN pe = CreatePen(PS_SOLID,1, RGB(r >> 8, g >> 8, b >> 8));
  this->pe = (HPEN)SelectObject(handle, pe);
  if (prvni) {
    DeleteObject(this->br);
    DeleteObject(this->pe);
  }
  else prvni = 1;
#endif
}

void HoDC::fillRectangle(int x, int y, int w, int h){
#if hplatform==XW
  XFillRectangle(
    HoMainApp->dpy,
    (img ?  img->handle :
      (client ? window->handle : window->handleBorder)
    ),
    handle, x, y, w, h
  );
#elif hplatform==MW
  Rectangle(handle, x, y, x+w, y+h);
#endif
}

void HoDC::fillEllipse( int x, int y, int width, int height){
#if hplatform==XW
  XFillArc(
    HoMainApp->dpy,
    (img ?  img->handle :
      (client ? window->handle : window->handleBorder)
    ),
    handle, x, y, width, height, 0, 23040
  );
#elif hplatform==MW
  Ellipse(handle, x, y, x+width, y+height);
#endif
}

void HoDC::drawLine(int x1, int y1, int x2, int y2){
#if hplatform==XW
  XDrawLine(
    HoMainApp->dpy,
    (img ?  img->handle :
      (client ? window->handle : window->handleBorder)
    ),

    handle, x1, y1, x2, y2
  );
#elif hplatform==MW
  MoveToEx(handle, x1, y1, NULL);
  LineTo(handle, x2, y2);
#endif
}

void HoDC::putPixel(int x, int y) {
#if hplatform==XW
  XDrawPoint(HoMainApp->dpy,
    (img ?  img->handle :
      (client ? window->handle : window->handleBorder)
    ),
   handle, x, y);

#else
  SetPixel(handle, x, y, RGB(r, g, b));
#endif
}

bool HoDC::putImage(int x, int y, int w, int h, HoImage *img, int ix, int iy) {
#if hplatform==XW
  XCopyArea(HoMainApp->dpy, img->handle, 
   (this->img ?  this->img->handle :
      (client ? window->handle : window->handleBorder)
    ),
  handle, ix, iy, w, h, x, y);
#elif hplatform==MW
  HDC compDC = CreateCompatibleDC(handle);
  HBITMAP staraBmp = (HBITMAP) SelectObject(compDC, img->handle);
  BitBlt(handle, x, y, w, h, compDC, ix, iy, SRCCOPY);
  SelectObject(compDC, staraBmp);
  DeleteDC(compDC);
#endif
  return true;
}

int HoDC::getW() {
  return w;
}

int HoDC::getH() {
  return h;
}
