#ifndef HoAppH
#define HoAppH
#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#include "hosemaphore.h"
#elif hplatform==MW
#include <windows.h>
#endif
#include "hotree.h"
#include "howindow.h"

int cmpWindowHandle(void *w1, void *w2);

class HoApp{
  public:
    /**
      Funkce, která se zavolá pøi obdr¾ení u¾ivetelské zprávy. Nebo NULL.
    */
    void (* userEvent)(void *);
    /**
      Bì¾í okenní prostøedí? Pod MS Windows v¾dy.
    */
    int winOK;
    HoTree *windowTree;
    HoTree *mainWindowTree;
#if hplatform==XW
    bool moznaKonec; 
    HoTree *borderWindowTree;
    int lx, ly, lw, lh;
    /**
    Handle dekorace okna, které my¹í mìním (pøesun, zmìna velikosti)
    */
    Window lhandle;
    /**
    Co se dìje s oknem.
    */
    enum Posun {
      /**
      Nic.
      */
      nic,
      /**
      Pøesouvá se za titulek.
      */
      sever,
      /**
      Mìní se y velikost posouváním spodku.
      */
      jih,
      /**
      Mìní se x velikost posouváním pravého okraje.
      */
      vychod,
      /**
      Mìní se x velikost a pozice posouváním levého okraje.
      */
      zapad,
      /**
      Mìní se x a y velikost posouváním pravého dolního rohu.
      */
      jihovychod,
       /**
      Mìní se x a y velikost a x pozice posouváním pravého dolního rohu.
      */
      jihozapad
    };
    
    Posun posun;
 #endif
  private:
    int fx, fy;
  public:
#if hplatform==MW
    DWORD threadId;
    HINSTANCE hInstance;
#elif hplatform==XW
    Display *dpy;
    XFontStruct *fnt;
    Atom xdel, xprot, zpravaAtom;
    XIM xim; /* Vstupní metoda */
    XIMStyle my_style;
#endif
   HoApp(int argc, char * argv[]);
   virtual void addWindow(HoWindow *w);
   virtual void removeWindow(HoWindow *w);
   virtual void removeMainWindow(HoWindow *w);
   virtual void addMainWindow(HoWindow *w);
   virtual int parseArgs(int argc,char * argv[]){return 1;};
   virtual int consoleContinue();
   virtual ~HoApp();
   virtual int run();
   virtual void getDefaultFontSize(int *x, int *y);
   virtual int getDefaultFontX() {
     int x;
     getDefaultFontSize(&x, NULL);
     return x;
   }
   virtual int getDefaultFontY() {
     int y;
     getDefaultFontSize(NULL, &y);
     return y;
   }

#if hplatform==XW
   void keyEvent(XKeyEvent *e, HoWindow *w);
#endif
};
extern HoApp *HoMainApp;
#endif

