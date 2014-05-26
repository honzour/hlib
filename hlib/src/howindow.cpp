#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#elif hplatform==MW
#include <windows.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "hoapp.h"
#include "hopushbutton.h"
#include "holistbox.h"
#include "howindow.h"

static int canCloseCallback(void *key, void *data, void *param){
  return ((HoWindow *)key)->canClose();
}
static int createCallback(void *key, void *data, void *param){
  ((HoWindow *)key)->create();
  return 1;
}

static int createChildrenCallback(void *key, void *data, void *param){
  if (!((HoWindow *)key)->created) ((HoWindow *)key)->create();
  return 1;
}

void HoWindow::createChildren(){
  if (!created) return;
  windowTree->inOrder(NULL, createChildrenCallback);
}

#if hplatform==MW
static int classNameInt = 0;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
  RECT r;
  HoWindow *w = (HoWindow *) HoMainApp->windowTree->get((void *) &hwnd);
  if (!w) goto def; // minimálnì WM_CREATE pøíjde pøed zaøazením do stromu
    switch (uMsg) {
    case WM_LBUTTONDOWN:
      SetFocus(hwnd);
      w->mouseDown(LOWORD (lParam), HIWORD(lParam), 0);
      return 0;
    case WM_RBUTTONDOWN:
      SetFocus(hwnd);
      w->mouseDown(LOWORD(lParam), HIWORD(lParam), 1);
      return 0;
    case WM_LBUTTONUP:
      w->mouseUp(LOWORD(lParam), HIWORD(lParam), 0);
      return 0;
    case WM_RBUTTONUP:
      w->mouseUp(LOWORD(lParam), HIWORD(lParam), 1);
      return 0;
    case WM_SIZE:
      GetWindowRect(w->handle, &r);
      w->w = r.right - r.left;
      w->h = r.bottom - r.top;
      w->x = r.left;
      w->y = r.top;
      w->rePaint();
      return 0;
    case WM_MOVE:
      GetWindowRect(w->handle, &r);
      w->w = r.right - r.left;
      w->h = r.bottom - r.top;
      w->x = r.left;
      w->y = r.top;
      return 0;
    case WM_CHAR:
      w->keyDown(win[((TCHAR) wParam) & 255], -1, 0);
      return 0;
    case WM_PAINT:
      if (!w) return DefWindowProc(hwnd, uMsg, wParam, lParam);
      {
        PAINTSTRUCT ps;
        RECT r;
        GetClientRect(hwnd, &r);
        HDC wdc = BeginPaint(hwnd, &ps);
        HoDC dc(w, wdc, r.right, r.bottom);
        w->paint(&dc);
        EndPaint(hwnd,&ps);
        return 0;
      }
    case WM_CLOSE:
      if (w->canClose()) w->destroy();
      return 0;
    case WM_COMMAND:
      if (HIWORD(wParam) == BN_CLICKED) {
        HoPushButton *b = (HoPushButton *)
          HoMainApp->windowTree->get((void *) &(lParam));
        if (b->pushed != NULL) b->pushed();
        return 0;
      }
      if (HIWORD(wParam) == LBN_SELCHANGE) {
        HoListbox *b = (HoListbox *)
          HoMainApp->windowTree->get((void *) &(lParam));
        if (b->change) b->change(SendMessage(b->handle, LB_GETCURSEL, 0, 0) + 1);
        return 0;
      }
      goto def;
    case WM_DESTROY:
      if (
        w->exitIfLast &&
          (
          !HoMainApp->mainWindowTree->root ||
          HoMainApp->mainWindowTree->is((void *) w)  &&
          !HoMainApp->mainWindowTree->root->l &&
          !HoMainApp->mainWindowTree->root->r
          )
        )
        {
          PostQuitMessage(0);
        }
      return 0;
    case WM_CTLCOLORSTATIC: {
      HDC hdcStatic;
      HBRUSH hbr;
      hdcStatic = (HDC) wParam;
	    SelectObject(hdcStatic, hbr = (HBRUSH) GetStockObject(WHITE_BRUSH));
      return (LRESULT)hbr;
    }
    default:
      def:;
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
}
#endif

void HoWindow::addWindow(HoWindow *w) {
  windowTree->ins((void *)w, NULL);
}

void HoWindow::removeWindow(HoWindow *w) {
  windowTree->del((void *)w);
}

int HoWindow::canClose() {
  return windowTree->inOrder(NULL, canCloseCallback);
}

void HoWindow::destroy() {
  void *data;

  while (data = windowTree->getMinKey()) {
    ((HoWindow *)data)->destroy();
  }
  if (!created) {
    goto JenDelete;
  }
  created = 0;
#if hplatform==MW
  DestroyWindow(handle);
  if (className[0]) {
    UnregisterClass(className, HoMainApp->hInstance);
    className[0] = 0;
  }
#elif hplatform==XW
  XDestroyWindow(HoMainApp->dpy, handle);
  if (handleBorder) {
    // u editù, labelù atd. být border nemusí
    XDestroyWindow(HoMainApp->dpy, handleBorder);
  }
#endif
  HoMainApp->removeWindow(this); // Z ostatních stromù destruktor
JenDelete:
  delete this; // dost kontroverzní vìc, co kdy¾ ho budu chtít
 // zase vytvoøit => v budoucnu volitelnì
}

HoWindow::HoWindow(HoWindow *parent, const HoString &title, int style) {
  init(parent, title, -1, -1, -1, -1, 1, style);
}

void HoWindow::init(
  HoWindow *parent, const HoString &title,
  int x, int y, int w, int h, int inLayout, int style) {
 
  handle = 0;
  exitIfLast = true;
#if hplatform==MW
  className[0] = 0;  // Dulezite. Kdyby ne, blblo by PushButton::destroy.
#else
  handleBorder = 0; 
#endif

  if (style < 0)
    // implicitni hodnoty
    if (!parent)
      style = styleCaption | styleBorder;
    else
      style = 0;
  // Minimalni vlastnosti okna bez rodice
  if (!parent) {
    style |= styleCaption;
    style |= styleBorder;
  }
  this->style = style;
  this->parent = parent;
  getBorders();
  this->m_layout = new HoVBoxLayout(this);
  this->inLayout = (!parent ? 0 : inLayout);
  this->w = w;
  this->h = h;
  this->x = x;
  this->y = y;
  windowTree = new HoTree(cmpVoid, 0, 0);
  this->title = new HoString(title);
  created = 0;
  if(parent) {
    parent->addWindow(this);
    if (this->inLayout) parent->addLayoutWindow(this);
    // if (parent->created) create(); - blbì, tady ještì nefunguje polymorfismus
  }
  else
    HoMainApp->addMainWindow(this);
}

HoString &HoWindow::getText(){
#if hplatform==MW
  int i = GetWindowTextLength(handle);
  char *s = (char *) malloc(i + 1);
  GetWindowText(handle, s, i + 1);
  delete title;
  title = new HoString(s);
  free(s);
#elif hplatform==XW
#endif
  return *title;
}

void HoWindow::setText(const HoString &s){

  delete title;
  title = new HoString(s);
  char *str = s.toString();
#if hplatform==MW
  SendMessage(handle, WM_SETTEXT, 0, (LPARAM)(str));
#elif hplatform==XW
  if (parent) {
    rePaint();
  } else {
  Window w;
  if (handleBorder) {
    w = handleBorder;
  } else {
    w = handle;
  }
  XChangeProperty(
    HoMainApp->dpy, // display
    w,  // ID okna
    XA_WM_NAME, // atom vlastnosti
    XA_STRING, // typ vlastnosti
    8, // je osmibitová
    PropModeReplace, // budeme nahrazovat (Replace)
    (const unsigned char *) str, //data vlastnosti
    strlen(str) // poèet elementù ve vlastnosti
  ); }
#endif
  free(str);
}

HoWindow::HoWindow(
  HoWindow *parent,
  const HoString &title,
  int w,
  int h,
  int style
  ) {
  
  init(parent, title, -1, -1, w, h, 1, style);
}

HoWindow::HoWindow(HoWindow *parent, const HoString &title,
  int x, int y, int w, int h, int style) {
  
  init(parent, title, x, y, w, h, 0, style);
}

HoWindow::~HoWindow() {
  if(parent) {
    parent->removeWindow(this);
    if (inLayout) parent->removeLayoutWindow(this);
  }
  else
    HoMainApp->removeMainWindow(this);
  delete windowTree;
  delete title;
  delete m_layout;
}

#if hplatform==MW
HWND _HoCreateWindow(char *className, const char *titulek, HWND parent,
						   int x, int y, int w, int h, DWORD style) {
  WNDCLASS wc;
  
  sprintf(className, "Honza%i", classNameInt++); /* Semafor !!*/
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HoMainApp->hInstance;
  wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = className;
  RegisterClass(&wc);
  return
    CreateWindow(
      className, titulek, style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      x, y, w, h,  
      parent, NULL, HoMainApp->hInstance, NULL
    );
}
#endif

int HoWindow::create() {

  HoAssert(!created);
  char * titulek = title->toString();
#if hplatform==XW
  int whiteColor = WhitePixel(HoMainApp->dpy, DefaultScreen(HoMainApp->dpy));
 
  if (h <= 0) h = 400;
  if (w <= 0) w = 400;
  if (x < 0) x = 0;
  if (y < 0) y = 0;

  handleBorder = XCreateSimpleWindow(
    HoMainApp->dpy,
    (parent ? parent->handle : DefaultRootWindow(HoMainApp->dpy)),
    x,
    y,
    w,
    h,
    0,
    whiteColor,
    whiteColor
   );
   XMoveResizeWindow(HoMainApp->dpy, handleBorder, x, y, w, h);
// Aby pøi zavírání okna poslalo místo zru¹ení ClientMessage s parametrem 1
   XSetWMProtocols(HoMainApp->dpy, handleBorder, & HoMainApp->xdel, 1);
   XChangeProperty(
     HoMainApp->dpy, // display
     handleBorder,  // ID okna
     XA_WM_NAME, // atom vlastnosti
     XA_STRING, // typ vlastnosti
     8, // je osmibitová
     PropModeReplace, // budeme nahrazovat (Replace)
     (const unsigned char *) titulek, //data vlastnosti
     strlen(titulek) // poèet elementù ve vlastnosti
   );
   XSelectInput(HoMainApp->dpy, handleBorder, StructureNotifyMask);
   XMapWindow(HoMainApp->dpy, handleBorder);
   xic = XCreateIC(
     HoMainApp->xim,
     XNInputStyle,
     HoMainApp->my_style,
     XNClientWindow,
     handleBorder, NULL
   );

   handle = XCreateSimpleWindow(
    HoMainApp->dpy,
    handleBorder,
    dx1,
    dy1,
    w - dx1 - dx2,
    h - dy1 - dy2,
    0,
    whiteColor,
    whiteColor
   );  
   XSetWMProtocols(HoMainApp->dpy, handle, & HoMainApp->xdel, 1);
   XChangeProperty(
     HoMainApp->dpy, // display
     handle,  // ID okna
     XA_WM_NAME, // atom vlastnosti
     XA_STRING, // typ vlastnosti
     8, // je osmibitová
     PropModeReplace, // budeme nahrazovat (Replace)
     (const unsigned char *) titulek, //data vlastnosti
     strlen(titulek) // poèet elementù ve vlastnosti
   );
   XSelectInput(HoMainApp->dpy, handle, StructureNotifyMask);
   XMapWindow(HoMainApp->dpy, handle);
   xic = XCreateIC(
     HoMainApp->xim,
     XNInputStyle,
     HoMainApp->my_style,
     XNClientWindow,
     handle, NULL
   );

#elif hplatform==MW
  DWORD styl = 0;
  if (style & styleCaption) styl |= (WS_CAPTION | WS_SYSMENU);
  if (style & styleSizing) styl |= WS_THICKFRAME;
  if (style & styleBorder) styl |= WS_DLGFRAME;
  if (parent) styl |= WS_CHILD;

  handle = _HoCreateWindow(className, titulek,
    (parent ? parent->handle :0),
    (x>=0 ? x : CW_USEDEFAULT),
    (y>=0 ? y : CW_USEDEFAULT),
    (w>=0 ? w : CW_USEDEFAULT),
    (h>=0 ? h : CW_USEDEFAULT),
    styl
   );
#endif
  HoMainApp->addWindow(this);
#if hplatform==MW
  ShowWindow(handle,SW_SHOWNORMAL);
  UpdateWindow(handle);
#endif
  free(titulek);
  created = 1;
  move(x, y, w, h);
  createChildren();
  return 1;
}


void HoWindow::paint(HoDC *dc){
}

void HoWindow::addLayoutWindow(HoWindow *w){
  m_layout->add(w);
}
void HoWindow::removeLayoutWindow(HoWindow *w){
  m_layout->remove(w);
}
/*
void HoWindow:paintSystem() {
#if hplatform==XW
  if (mamKreslit)
  HoDC dc(this);

  paint(&dc);
  XFlush(HoMainApp->dpy);
#endif
}
*/
void HoWindow::rePaint() {
#if hplatform==XW
  HoDC dc(this);

  paint(&dc);
  XFlush(HoMainApp->dpy);
#elif hplatform==MW
  InvalidateRect(handle, NULL, TRUE);
#endif
}

#if hplatform==XW
void HoWindow::rePaintBorder() {
  
  if (parent && style) {
    HoDC dc(true, this);
  
    if (style & (styleBorder | styleSizing)) {
      dc.fillRectangle(0, 0, w, dy1);
      dc.fillRectangle(0, dy1, dx1, h);
      dc.fillRectangle(w - dx2, dy1, w, h);
      dc.fillRectangle(dx1, h - dy2, w - dx2, h);
    }

    if (style & styleCaption) {
      dc.setForeground(0x5555, 0x5555, 0xEEEE);
      dc.fillRectangle(0, 0, w, dy1);
      dc.setForeground(0xffff, 0xffff, 0xffff);
      dc.drawString(dy1, dy1 >> 3, *title);
      dc.setForeground(0xC0C0, 0xC0C0, 0xC0C0);
      dc.fillRectangle(w - dy1, 0, w, dy1);
      dc.setForeground(0, 0, 0);
      dc.drawLine(w - dy1, 0, w, dy1);
      dc.drawLine(w, 0, w - dy1, dy1);
    }
    
    XFlush(HoMainApp->dpy);
  }
}
#endif

void HoWindow::move(int x, int y, int w, int h) {
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  if (created) {
#if hplatform==MW
    MoveWindow(handle, x, y, w, h, TRUE);
#else
    if (handleBorder) {
      if (parent)
        XMoveResizeWindow(HoMainApp->dpy, handleBorder, x, y, w, h);
      else 
        XResizeWindow(HoMainApp->dpy, handleBorder, w, h);
      XMoveResizeWindow(
        HoMainApp->dpy,
        handle,
        dx1, dy1,
        w - dx1 - dx2, h - dy1 - dy2
      );
    }
    else {
      if (parent)
        XMoveResizeWindow(HoMainApp->dpy, handle, x, y, w, h);
      else
        XResizeWindow(HoMainApp->dpy, handle, w, h);
    }
    XFlush(HoMainApp->dpy);
#endif
  }
}

void HoWindow::pack(){
  // prázdná okna se nepackují
  if (m_layout->empty()) return;
  m_layout->pack();
  if (created) {
    move(x, y, w, h);
  }
}

void HoWindow::getBorders(int topLevel, int style,
                          int *x1, int *x2, int *y1, int *y2) {
#if hplatform==MW
  int dx = 0;
  int dy = 0;
    
  if (style & styleBorder) {
    dx += GetSystemMetrics(SM_CXBORDER);
    dy += GetSystemMetrics(SM_CYBORDER);
  }
  if (style & styleSizing) {
    dx += GetSystemMetrics(SM_CXFRAME);
    dy += GetSystemMetrics(SM_CYFRAME);
  }
  if (x1) *x1 = dx;
  if (x2) *x2 = dx;
  if (y2) *y2 = dy;
  if (y1) {
    if (style & styleCaption)
      dy += GetSystemMetrics(SM_CYSIZE);
    *y1 = dy;
  }

#else

  if (topLevel) {
    bezDekorace:
    if (x1) *x1 = 0;
    if (x2) *x2 = 0;
    if (y2) *y2 = 0;
    if (y1) *y1 = 0;
  }
  else {
    if (!style) goto bezDekorace;
    int borderWidth =
     ((style & (styleBorder | styleSizing)) ? 3 : 0);
    int captionWidth;
    
    if (style & styleCaption) {
      HoMainApp->getDefaultFontSize(NULL, &captionWidth);
      captionWidth += (captionWidth >> 2);
    }
    else {
      captionWidth = 0;
    }
    
    if (x1) *x1 = borderWidth;
    if (x2) *x2 = borderWidth;
    if (y2) *y2 = borderWidth;
    if (y1) *y1 = captionWidth; 
  }
#endif
}

int HoWindow::minW = 1;
int HoWindow::minH = 1;

