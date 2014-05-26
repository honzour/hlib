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
      Funkce, kter� se zavol� p�i obdr�en� u�ivetelsk� zpr�vy. Nebo NULL.
    */
    void (* userEvent)(void *);
    /**
      B�� okenn� prost�ed�? Pod MS Windows v�dy.
    */
    int winOK;
    HoTree *windowTree;
    HoTree *mainWindowTree;
#if hplatform==XW
    bool moznaKonec; 
    HoTree *borderWindowTree;
    int lx, ly, lw, lh;
    /**
    Handle dekorace okna, kter� my�� m�n�m (p�esun, zm�na velikosti)
    */
    Window lhandle;
    /**
    Co se d�je s oknem.
    */
    enum Posun {
      /**
      Nic.
      */
      nic,
      /**
      P�esouv� se za titulek.
      */
      sever,
      /**
      M�n� se y velikost posouv�n�m spodku.
      */
      jih,
      /**
      M�n� se x velikost posouv�n�m prav�ho okraje.
      */
      vychod,
      /**
      M�n� se x velikost a pozice posouv�n�m lev�ho okraje.
      */
      zapad,
      /**
      M�n� se x a y velikost posouv�n�m prav�ho doln�ho rohu.
      */
      jihovychod,
       /**
      M�n� se x a y velikost a x pozice posouv�n�m prav�ho doln�ho rohu.
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
    XIM xim; /* Vstupn� metoda */
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

