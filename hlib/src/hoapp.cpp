#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "hoset.h"
#include "holog.h"
#include "hothread.h"

#if hplatform==XW

#ifndef XK_MISCELLANY
        #define XK_MISCELLANY
#endif

#ifndef XK_LATIN1
        #define XK_LATIN1
#endif
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>
#include <X11/keysymdef.h>
#include <unistd.h>
#elif hplatform==MW
#include <windows.h>
#endif

#include "hotree.h"
#include "hoapp.h"

HoApp *HoMainApp;

#if hplatform==MW
static HINSTANCE hhInstance;
#endif

int cmpWindowHandle(void *w1, void *w2){
#if hplatform==XW
  if ((*(Window *)w1) < (*(Window *)w2)) return -1;
  if ((*(Window *)w1) > (*(Window *)w2)) return 1;
#elif hplatform==MW
  if ((*(HWND *)w1) < (*(HWND  *)w2)) return -1;
  if ((*(HWND  *)w1) > (*(HWND  *)w2)) return 1;
#endif
  return 0;
}


HoApp::HoApp(int argc, char * argv[]) {

  HoAssert(!HoMainApp);
  HoMainApp = this;
  userEvent = NULL;
  parseArgs(argc, argv);

  windowTree = new HoTree(cmpWindowHandle, 0, 1);
	  mainWindowTree = new HoTree(cmpVoid, 0, 0);
#if hplatform==XW
  moznaKonec = false;
  lhandle = 0;
  posun = nic;
  borderWindowTree = new HoTree(cmpWindowHandle, 0, 1);
#endif
#if hplatform==MW
  threadId = GetCurrentThreadId();
  hInstance = hhInstance;
  char className[100];
  HWND h = _HoCreateWindow(className, "nic",
    0, 0, 0, 0, 0, 0);
  HDC dc = GetDC(h);
  TEXTMETRIC tm;
  GetTextMetrics(dc, &tm);
  fx = tm.tmAveCharWidth;
  fy = tm.tmHeight;
  ReleaseDC(h, dc);
  DestroyWindow(h);
  winOK = 1;

#elif hplatform==XW
 dpy = XOpenDisplay(0);
 if(!dpy){
   winOK = 0;
   if (!consoleContinue()) exit(1);
 }
 else {
    char strz[50];

    sprintf(strz, "_HONZOVA_ZPRAVA%i", getpid());
    zpravaAtom = XInternAtom(dpy, strz, False);

    /* Nastavit locale */
    char *locale;
    /*
    Doporuèované setlocale(LC_ALL, "") zpùsobí napøíklad zmìnu desetiné teèky
    za èárku pøi printf, co¾ je docela velký prù¹vih, pokud má program nìco
    generovat (tøeba nìjaký zdroják). Názorný pøíklad toho, jak se nemá dìlat
    lokalizace. Nezavolání setlocale zase zpùsobí, ¾e nepùjde napø. do
    edlajn psát 'ø'.
    */
    if(!(locale = setlocale(LC_CTYPE, ""))) {
        holog.log(F2, "Cannot set locale\n");
	setlocale(LC_CTYPE, "cs");
       // exit(1);
    }
    if(!XSupportsLocale()) {
        holog.log(F2, "X does not support locale\n");
        exit(1);
    }
    if(!(locale=XSetLocaleModifiers("")))
       holog.log(E2, "Cannot set locale modifiers\n");

  winOK = 1;
  fnt =
    XLoadQueryFont(
      dpy,
      "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-2"
    );
  if (!fnt) {
    int i;
    char **f =
      XListFonts(
        dpy,
        "-*-helvetica-bold-r-normal--12-*-*-*-*-*-iso8859-2",
        10000,
        &i
      );
  if (f) {
    fnt =
      XLoadQueryFont(dpy, f[0]);
      XFreeFontNames(f);
  }
  else {
    f = XListFonts(dpy, "-*-*-*-*-*--12-*-*-*-*-*-iso8859-2", 10000, &i);
    if (f) {
      fnt = XLoadQueryFont(dpy, f[0]);
      XFreeFontNames(f);
    }
    else {
      f = XListFonts(dpy, "-*-*-*-*-*--12-*-*-*-*-*-*", 10000, &i);
      if (f) {
        fnt = XLoadQueryFont(dpy, f[0]);
        XFreeFontNames(f);
      }
      else {
        f = XListFonts(dpy, "-*-*-*-*-*--*-*-*-*-*-*-*", 10000, &i);
        if (f) {
          fnt = XLoadQueryFont(dpy, f[0]);
          XFreeFontNames(f);
        } else {
          holog.log(F2, "V XWindows není vùbec ¾ádný font");
          exit(1);
	}
      }
   /* Else radeji nechat vychozi font...*/
    }
   }
  }
  xprot=XInternAtom(dpy,"WM_PROTOCOLS",false);
  xdel=XInternAtom(dpy,"WM_DELETE_WINDOW",false);
  if (!(xim = XOpenIM(dpy, NULL, NULL, NULL))) {
     holog.log(F2, "Cannot open input method\n");
     exit(1);
   }
   XFontSet fontset; /* Font set pobrazování textu */
   XIMStyles *im_styles; /* Styly vstupní metody */
   XIMStyle style; /* aktuálnì testovaný styl */
   char str_style[300], str_my_style[300]; /* Textový zápis stylu */
   int i;

   XGetIMValues(xim, XNQueryInputStyle, &im_styles, NULL);
   my_style = 0;
   str_my_style[0] = '\0';
    for (i = 0; i < im_styles->count_styles; i++) {
      style = im_styles->supported_styles[i];
      str_style[0] = '\0';
      if (style & XIMPreeditCallbacks)
        strcat(str_style, "XIMPreeditCallbacks | ");
      if (style & XIMPreeditPosition)
        strcat(str_style, "XIMPreeditPosition | ");
      if (style & XIMPreeditArea)
        strcat(str_style, "XIMPreeditArea | ");
      if (style & XIMPreeditNothing)
        strcat(str_style, "XIMPreeditNothing | ");
        if(style & XIMPreeditNone)
            strcat(str_style, "XIMPreeditNone | ");
        if(style & XIMStatusCallbacks)
            strcat(str_style, "XIMStatusCallbacks | ");
        if(style & XIMStatusArea)
            strcat(str_style, "XIMStatusArea | ");
        if(style & XIMStatusNothing)
            strcat(str_style, "XIMStatusNothing | ");
        if(style & XIMStatusNone)
            strcat(str_style, "XIMStatusNone | ");
        if(str_style[0] != '\0')
            str_style[strlen(str_style)-3] = '\0';
        if((style &
           (XIMPreeditNothing|XIMPreeditNone|XIMStatusNothing|XIMStatusNone))
	     == style) {
            /* Vybereme si ze stylù, které umíme */
            if(my_style == 0) {
                my_style = style;
            } else {
                if(my_style & XIMPreeditNone) {
                    if((style & XIMPreeditNothing) || (style & XIMStatusNothing))
                        my_style = style;
                } else {
                    if((style & XIMPreeditNothing) && (style & XIMStatusNothing))
                        my_style = style;
                }
            }
            if(style == my_style)
                strcpy(str_my_style, str_style);
        }
    }
    XFree(im_styles);
  }
#endif
}

#if hplatform==XW
void HoApp::keyEvent(XKeyEvent *e, HoWindow *w){

  KeySym ks;
  static char str[16];
  wchar_t buf[16];
/*int table=x_input_encoding<0?drv->codepage:x_input_encoding;*/
  int len, key;
  Status status;

  len=XwcLookupString(w->xic, e, buf, 32, &ks, &status);
  if (status == XLookupKeySym || status == XLookupBoth) {
    switch (ks) {
      case NoSymbol:
      case XK_Return:
      case XK_BackSpace: w->keyDown(0,Ho::KeyBcksp,0); return;
      case XK_KP_Tab:
      case XK_Tab:
      case XK_Escape: w->keyDown(0,Ho::KeyEsc,0); return;
      case XK_KP_Left:
      case XK_Left: w->keyDown(0,Ho::KeyLeft,0); return;
      case XK_KP_Right:
      case XK_Right: w->keyDown(0,Ho::KeyRight,0); return;
      case XK_KP_Up:
      case XK_Up: w->keyDown(0,Ho::KeyUp,0); return;
      case XK_KP_Down:
      case XK_Down: w->keyDown(0,Ho::KeyDown,0); return;
      case XK_KP_Insert:
      case XK_Insert: w->keyDown(0,Ho::KeyIns,0); return;
      case XK_KP_Delete:
      case XK_Delete: w->keyDown(0,Ho::KeyDel,0); return;
      case XK_KP_Home:
      case XK_Home: w->keyDown(0,Ho::KeyHome,0); return;
      case XK_KP_End:
      case XK_End: w->keyDown(0,Ho::KeyEnd,0); return;
      case XK_KP_Page_Up:
      case XK_Page_Up: w->keyDown(0,Ho::KeyPgUp,0); return;
      case XK_KP_Page_Down:
      case XK_Page_Down: w->keyDown(0,Ho::KeyPgDown,0); return;
      case XK_KP_F1:
      case XK_F1:
      case XK_KP_F2:
      case XK_F2:
      case XK_KP_F3:
      case XK_F3:
      case XK_KP_F4:
      case XK_F4:
      case XK_F5:
      case XK_F6:
      case XK_F7:
      case XK_F8:
      case XK_F9:
      case XK_F10:
      case XK_F11:
      case XK_F12:return;
      case XK_KP_Subtract:key='-';break;
      case XK_KP_Decimal:key='.';break;
      case XK_KP_Divide:key='/';break;
      case XK_KP_Space:key=' ';break;
      case XK_KP_Enter:return;
      case XK_KP_Equal:key='=';break;
      case XK_KP_Multiply:key='*';break;
      case XK_KP_Add:key='+';break;
      case XK_KP_0:key='0';break;
      case XK_KP_1:key='1';break;
      case XK_KP_2:key='2';break;
      case XK_KP_3:key='3';break;
      case XK_KP_4:key='4';break;
      case XK_KP_5:key='5';break;
      case XK_KP_6:key='6';break;
      case XK_KP_7:key='7';break;
      case XK_KP_8:key='8';break;
      case XK_KP_9:key='9';break;
      default: break;
    }
  }
  if (status == XLookupChars || status == XLookupBoth) {
    w->keyDown(/*iso[key]*/ buf[0], -1, 0);
  }
}
#endif

int HoApp::run() {

  if (!winOK) return 0;

#if hplatform==XW
  XEvent e;
  bool client;
  
  while (windowTree->root || !moznaKonec) {
    
    client = true;
    HoWindow *w = NULL;
    XNextEvent(dpy, &e);
 

   if (
     e.type == ClientMessage &&
     zpravaAtom == e.xclient.message_type &&
     e.xclient.data.l[0] == 0x87654321) {

      int i;
      void *v = (void *) e.xclient.data.l[1];
      if (userEvent) userEvent(v);
      continue;
    }
   
    if (XFilterEvent(&e, None)) /* Filtrace událostí vstupní metodou */
      continue;
//    printf(" e.xany.window je 0x%x\n", e.xany.window);
    w = (HoWindow *) windowTree->get((void *) &e.xany.window);
    if (!w || w->handle != e.xany.window) {
      w = (HoWindow *) borderWindowTree->get((void *) &e.xany.window);
      if (!w || w->handleBorder != e.xany.window) {
        continue;
      }
      else
        client = false;
    }
    switch (e.type) {
      case MapNotify:
        if (client) {
          XSelectInput(
            dpy,
            w->handle, 
              ButtonPressMask |
              ButtonReleaseMask |
              ExposureMask |
              SubstructureNotifyMask |
              KeyPressMask |
	      ResizeRedirectMask 
          );
          w->rePaint();
	}
	else {
          XSelectInput(
            dpy,
            w->handleBorder,
              ButtonPressMask |
              ButtonReleaseMask |
              ExposureMask |
              SubstructureNotifyMask |
              KeyPressMask
          );
	  w->rePaintBorder();
	}
        break;
      case Expose:
      // asi lépe zmìna velikosti jinde!
        if (!client) {
          XWindowAttributes a;
          XGetWindowAttributes(dpy, w->handleBorder, &a);
          if (w->w != a.width || w->h != a.height) {
            w->w = a.width;
            w->h = a.height;
            XMoveResizeWindow(
              HoMainApp->dpy,
              w->handle,
              w->dx1, w->dy1,
              w->w - w->dx1 - w->dx2,
              w->h - w->dy1 - w->dy2
            );
          }
          if (!e.xexpose.count) w->rePaintBorder();
        }
        else
          if (!e.xexpose.count) w->rePaint();
        break;
      case ButtonPress:
        XSetInputFocus(HoMainApp->dpy, w->handle, RevertToPointerRoot, CurrentTime);
        // pokud má okno titulek, pøesuneme ho do popøedí
        if (w->style & HoWindow::styleCaption)
	  XRaiseWindow(HoMainApp->dpy, w->handleBorder);
        
        if (!client) {
          // sever
          if ((w->style & HoWindow::styleCaption) && e.xbutton.y < w->dy1) {
            if (e.xbutton.x > w->w - w->dy1) {
              if (w->canClose()) {
                w->destroy();
              }
            }
            else {
	      lhandle = w->handleBorder;
              posun = sever;
	      lx = e.xbutton.x;
	      ly = e.xbutton.y;
            }
            break;
          }
          // jih
          if ((w->style & HoWindow::styleSizing) && e.xbutton.y > w->h - w->dy2) {
	    lhandle = w->handleBorder;
            posun = jih;
	    lx = e.xbutton.x;
	    ly = e.xbutton.y;
            break;
          }
          // vychod 
          if ((w->style & HoWindow::styleSizing) && e.xbutton.x > w->w - w->dx2) {
	    lhandle = w->handleBorder;
            posun = vychod;
	    lx = e.xbutton.x;
	    ly = e.xbutton.y;
            break;
          }
          // zapad 
          if ((w->style & HoWindow::styleSizing) && e.xbutton.x < w->dx1) {
	    lhandle = w->handleBorder;
            posun = zapad;
	    lx = e.xbutton.x;
	    ly = e.xbutton.y;
            break;
          }
            
        }
        
        else {
          if (e.xbutton.button == 1)
            w->mouseDown(e.xbutton.x, e.xbutton.y, 0);
          if (e.xbutton.button == 3)
            w->mouseDown(e.xbutton.x, e.xbutton.y, 1);
        }
        break;
      case ButtonRelease:
        if (!client) {
	  if (lhandle == w->handleBorder) {
            if (posun == sever) {
              w->move(
                w->x + e.xbutton.x - lx,
                w->y + e.xbutton.y - ly,
                w->w,
                w->h
              );
            }
            if (posun == jih) {
              int noveH = w->h + e.xbutton.y - ly;
              if (noveH < w->dy1 + w->dy2 + HoWindow::minH)
              noveH = w->dy1 + w->dy2 + HoWindow::minH;
              w->move(
                w->x,
                w->y,
                w->w,
                noveH
              );
            }
            if (posun == vychod) {
	      int noveW = w->w + e.xbutton.x - lx;
	      if (noveW < w->dx1 + w->dx2 + HoWindow::minW)
	         noveW = w->dx1 + w->dx2 + HoWindow::minW;
              w->move(
                w->x,
                w->y,
                noveW,
                w->h
              );
            }
            if (posun == zapad) {
	      int noveW = w->w - e.xbutton.x + lx;
	      if (noveW < w->dx1 + w->dx2 + HoWindow::minW)
	         noveW = w->dx1 + w->dx2 + HoWindow::minW;
              w->move(
                w->x - noveW + w->w,
                w->y,
                noveW,
                w->h
              );
            }
          }
          lhandle = 0;
        }
        else {
          if (e.xbutton.button == 1)
            w->mouseUp(e.xbutton.x, e.xbutton.y, 0);
          if (e.xbutton.button == 3)
            w->mouseUp(e.xbutton.x, e.xbutton.y, 1);
          break;
        }
      case KeyPress: keyEvent(&e.xkey, w); break;
      case ClientMessage:
        if (
             e.xclient.format == 32 &&
             e.xclient.message_type == xprot &&
             e.xclient.data.l[0] == xdel &&
             w->canClose()
           )
          w->destroy();
        break;
      case ResizeRequest:
        w->w = e.xresizerequest.width;
        w->h = e.xresizerequest.height;
        w->rePaint();
        break;
      default:
        break;
    }
  }

#elif hplatform==MW

  MSG msg;
  
  while (GetMessage(&msg, NULL, 0, 0) == 1) {
    if (msg.message == WM_USER) {
      if (userEvent) userEvent((void *) msg.lParam);
      continue;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
#endif
  return 0;
}

int HoApp::consoleContinue() {
  puts("Tento program vyzaduje X nebo MS Windows.");
  return 0;
}

HoApp::~HoApp() {
#if hplatform==XW
  delete borderWindowTree;
#endif
  delete windowTree;
  delete mainWindowTree;
#if hplatform==XW
  if (fnt) {
    //XUnloadFont(dpy, fnt->fid);
    XFreeFont(dpy, fnt);
  }
  XCloseDisplay(dpy);
#endif
}

void HoApp::removeWindow(HoWindow *w) {
#if hplatform==XW
  if (w->handleBorder) borderWindowTree->del((void *)&(w->handleBorder));
#endif
  windowTree->del((void *)&(w->handle));
}

void HoApp::addWindow(HoWindow *w){
#if hplatform==MW
  HWND *pw = (HWND *) malloc(sizeof(HWND));
#elif hplatform==XW
  Window *pw = (Window *) malloc(sizeof(Window));

#endif
  *pw = w->handle;
  windowTree->ins((void *) pw, (void *) w);
#if hplatform==XW
  if (w->handleBorder) {
    Window *pbw = (Window *) malloc(sizeof(Window));
    *pbw = w->handleBorder;
     borderWindowTree->ins((void *) pbw, (void *) w);
  }
#endif
}

void HoApp::addMainWindow(HoWindow *w) {
  mainWindowTree->ins((void *) w, NULL);
}
void HoApp::removeMainWindow(HoWindow *w) {
#if hplatform==XW
  moznaKonec = w->exitIfLast;
#endif
  mainWindowTree->del((void *) w);
}

void HoApp::getDefaultFontSize(int *x, int *y) {
#if hplatform==MW
  if (x)
    *x = fx;
  if (y)
    *y = fy;
#elif hplatform==XW
  if (x)
    *x = (HoMainApp->fnt->ascent + HoMainApp->fnt->descent) >> 1;
  if (y)
    *y = HoMainApp->fnt->ascent + HoMainApp->fnt->descent;
#endif
}

int homain(int argc,char * argv[]);

#if hplatform==XW
int main(int argc,char * argv[]){
  init_encoding();
  argc=homain(argc, argv);
  done_encoding();
  return argc;
}
#elif hplatform==MW
int PASCAL WinMain(HINSTANCE hCurInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
 
   wVersionRequested = MAKEWORD( 1, 1 );
 
err = WSAStartup( wVersionRequested, &wsaData );
if ( err != 0 ) {
    /* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */
    return 0;
}

  hhInstance=hCurInstance;
  init_encoding();
  char *arg = GetCommandLine();

  //MessageBox(NULL, arg, arg, MB_OK);

  int argc = 0;
  char **argv = NULL;
  int i;
  if (arg && arg[0]) {
    if (arg[0] != '"') {
chyba:
    //  MessageBox(NULL,"Chyba GetCommandLine" , "Chyba", MB_OK);
    // Divná pøíkazová øádka, nebudu se s tim parsovat...
      argc = 1;
      argv = (char **) malloc(sizeof(char *));
      argv[0] = (char *) malloc(i);
      goto bezim;
    }
    for (i = 1; ; i++) {
      if (arg[i] == 0) {
        goto chyba;
      }
      if (arg[i] == '"') {
        i++;
        argc++;
        break;
      }
    }
    while (arg[i]) {
      while (arg[i] && arg[i] <= ' ') i++;
      if (!arg[i]) break;
      argc++;
      while (arg[i] > ' ') i++;
    }
    argv = (char **) malloc(sizeof(char *) * argc);
    i = 1;
    while (arg[i] != '"') i++;
    argv[0] = (char *) malloc(i);
    memcpy((void *) argv[0], (const void *) (arg + 1), i - 1);
    argv[0][i - 1] = 0;
    i++;
    int k, j;
    for (k = 1; k < argc; k++) {
      while (arg[i] <= ' ') i++;
      j = i;
      while (arg[i] > ' ') i++;
      argv[k] = (char *) malloc(i - j + 1);
      memcpy((void *) argv[k], (const void *) (arg + j), i - j);
      argv[k][i - j] = 0;
    }
  }
bezim:
  nCmdShow = homain(argc, argv);

  if (argc) {
    for (i = 0; i < argc; i++) free(argv[i]);
    free(argv);
  }
  done_encoding();

  return nCmdShow;
}
#endif

