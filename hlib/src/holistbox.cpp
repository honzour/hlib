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
#include "hotree.h"
#include "holistbox.h"

int cmpPint(void *a, void *b) {
  if (*(int *) a > *(int *) b) return 1;
  if (*(int *) a < *(int *) b) return -1;
  return 0;
}

HoListbox::HoListbox(HoWindow *parent, int x, int y, int w, int h,
  void (*change)(int), bool enableSelect)
 :HoWindow(parent, "", x, y, w, h)
{
  HoAssert(parent);
  this->change = change;
  initTree();
  selected = 0;
  this->enableSelect = enableSelect;
}

HoListbox::HoListbox(HoWindow *parent, int w, int h, void (*change)(int),
  bool enableSelect)
 :HoWindow(parent, "", w, h)
{
  this->change = change;
  HoAssert(parent);
  initTree();
  selected = 0;
  this->enableSelect = enableSelect;
}

#if hplatform==MW
int insCalback(void * key, void * data, void * param) {
  HoListbox *h = (HoListbox *) param;
  HoString *s = (HoString *) data;
  char *str = s->toString();
  SendMessage(h->handle, LB_ADDSTRING, 0, (LPARAM) str);
  free(str);
  return 1;
}
#endif

int g(void * key, void * data, void * param) {
  delete (HoString *) data;
  return 1;
}

HoListbox::~HoListbox() {
  stringy->inOrder(NULL, g);
  delete stringy;
}

void HoListbox::initTree() {
  stringy = new HoTree(cmpPint, 0, 1);
}

int HoListbox::create() {
  char *s = title->toString();
  created = 1;
#if hplatform==MW
  handle = CreateWindow(
    "LISTBOX",
    s,
    WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
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
  handle = XCreateSimpleWindow(
    HoMainApp->dpy,
    parent->handle,
    x,
    y,
    w,
    h,
    0,
    whiteColor,
    whiteColor
  );
  XSetWMProtocols(HoMainApp->dpy, handle, & HoMainApp->xdel, 1);
  XSelectInput(HoMainApp->dpy, handle, StructureNotifyMask);
  XMapWindow(HoMainApp->dpy, handle);
  xic = XCreateIC(HoMainApp->xim, XNInputStyle, HoMainApp->my_style,
    XNClientWindow, handle, NULL);
#endif

  HoMainApp->addWindow(this);
  free(s);
#if hplatform==MW
  stringy->inOrder((void *) this, insCalback);
#endif
  return 1;
}

struct PaintInfo {
  int dy;
  int up;
  int left;
  int selected;
  HoDC *dc;
};

int f(void * key, void * data, void * param) {
  PaintInfo *p = (PaintInfo *) param;
  HoListbox *lb = (HoListbox *) p->dc->window;
  if (p->selected == *(int *) key) {
    p->dc->setForeground(0, 0, 220 << 8);
    p->dc->fillRectangle(1, p->up + ((*(int *) key) - 1) * p->dy, lb->w - 2, (p->dy * 4) / 5);
    p->dc->setForeground(255 << 8, 255 << 8, 255 << 8);
    p->dc->drawString(p->left, p->up + ((*(int *) key) - 1) * p->dy, *(HoString *) data);
  }
  else
    p->dc->setForeground(0, 0, 0);
  p->dc->drawString(p->left, p->up + ((*(int *) key) - 1) * p->dy, *(HoString *) data);
  return 1;
}

void HoListbox::paint(HoDC *dc){
#if hplatform==XW
  int fx, fy;
  dc->setForeground(0xFFFF, 0xFFFF, 0xFFFF);
  dc->fillRectangle(0, 0, w, h);
  HoMainApp->getDefaultFontSize(&fx, &fy);
  dc->setForeground(0, 0, 0);
  dc->drawLine(0, 0, 0, h - 1);
  dc->drawLine(0, 0, w - 1, 0);
  dc->drawLine(w - 1, h - 1, 0, h - 1);
  dc->drawLine(w - 1, h - 1, w - 1, 0);
  PaintInfo p;
  p.dy = (fy * 3)/2;
  p.up = fy / 2;
  p.left = fx;
  p.selected = selected;
  p.dc = dc;
  stringy->inOrder((void *) &p, f);
#endif
}

void HoListbox::mouseDown(int x, int y, int button) {
  
  if (!enableSelect) return;
  int fx, fy;
  HoMainApp->getDefaultFontSize(&fx, &fy);

  int sel = (y - fy / 2) / ((fy * 3)/2) + 1;
  if (sel < 0 || sel > stringy->size() || sel == selected)
    return;
  selected = sel;
  rePaint();
  if (change) change(sel - 1);
}

void HoListbox::addString(const HoString &s) {
  
  HoString *str = new HoString(s);
  int *i = (int *) malloc(sizeof(int));
  *i = stringy->size() + 1;
  stringy->ins((void *) i, (void *) str);
#if hplatform == MW
  if (created) {
    char *cstr = s.toString();
    SendMessage(handle, LB_ADDSTRING, 0, (LPARAM) cstr);
    free(cstr);
  }
#endif
  if (created) rePaint();
}

bool HoListbox::changeString(int index, const HoString &s) {
  
  HoString *str = (HoString *) stringy->get((void *) &index);
  if (!str) return false;
  *str = s;
#if hplatform == MW
  if (created) {
  // TODO
  }
#else
  if (created) rePaint();
#endif
  return true;
}

int delF(void * key, void * data, void * param) {
//  printf("delF %i %i\n", *(int *) key, (int) param);
  if (*(int *) key > (int) param) (*((int *) key))--;
  return 1;
}

bool HoListbox::removeString(int index) {

//  puts("HoListbox::removeString");
  void *v = stringy->get((void *) &index);
  if (!v) return false;
  delete (HoString *) v;
  stringy->del((void *) &index);
  stringy->inOrder((void *) index, delF);
  if (selected) selected = stringy->size();
#if hplatform == MW
  if (created) {
    SendMessage(handle, LB_DELETESTRING, (WPARAM) (index - 1), 0);
  }
#endif
  if (created) rePaint();
  return true;
}

