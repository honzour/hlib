#ifndef HoWindowH
#define HoWindowH
#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif
#include "hotree.h"
#include "hodc.h"
#include "hostring.h"
#include "holayout.h"

/**
  "Handle" okna
*/
typedef 
#if hplatform==XW
  Window
#elif hplatform==MW
  HWND
#endif
  HoWindowHandle;

/**
T��da reprezentuj�c� okno.
*/
class HoWindow {
  public:
    /**
     "Handle" okna
    */
    HoWindowHandle handle;
#if hplatform==XW 
    HoWindowHandle handleBorder;
#endif
#if hplatform==MW
    char className[16];
#else
    XIC xic; /* Vstupn� kontext */
#endif
    /**
       Ma se po ukonceni okna ukoncit i program, je-li okno jedine hlavni?
    */
    bool exitIfLast;
    /**
    ���ka okna v�etn� dekorace, nastav� se ji� v konstruktoru. -1 znamen�,
    �e ji m� ur�it layout, ale dosud neur�il.
    */
    int w;

    /**
    V��ka okna v�etn� dekorace, nastav� se ji� v konstruktoru. -1 znamen�,
    �e ji m� ur�it layout, ale dosud neur�il.
    */
    int h;

    /**
    Minim�ln� w klientsk� oblasti.
    */
    static int minW;
    
    /**
    Minim�ln� h klientsk� oblasti.
    */
    static int minH;
    
    /**
    X sou�adnice lev�ho horn�ho rohu okna (resp. jeho dekorace) v��i lev�mu
    horn�mu rohu klientsk� ��sti otce, nastav� se ji� v konstruktoru.
    -1 znamen�, �e ji m� ur�it layout otce, ale dosud neur�il.
    */
    int x;

    /**
    Y sou�adnice lev�ho horn�ho rohu okna (resp. jeho dekorace) lev�mu horn�mu
    rohu klientsk� ��sti otce, nastav� se ji� v konstruktoru. -1 znamen�, �e ji
    m� ur�it layout otce, ale dosud neur�il.
    */
    int y;

    /**
    ���ka lev� dekorace okna.
    */
    int dx1;

    /**
    ��ka prav� dekorace okna v�etn� rolovadla.
    */
    int dx2;

    /**
    V��ka horn� dekorace okna vcetn� titulku, menu.
    */
    int dy1;

    /**
    V��ka doln� dekorace okna v�etne rolovadla.
    */
    int dy2;

    /**
    Hodnoty stylu okna
    */
    enum {
    /**
      M� okno titulek (s n�zvem)? U okna �rovn� 1 v�dy.
    */
      styleCaption = 1,
      /**
      M� okno r�me�ek? U okna urovn� 1 v�dy.
      */
      styleBorder = 2,
      /**
      M� okno r�me�ek pro zv�t�ov�n� u�ivatelem?
      */
      styleSizing = 4
    };

    /**
    Styl okna
    */
    int style;

    /**
    Titulek naho�e v dekoraci okna (je-li).
    */
    HoString *title; 

    /**
    Strom v�ech dce�in�ch oken,
    */
    HoTree *windowTree;

    /**
    Layout dce�in�ch oken. Nemus� v n�m b�t v�echna.
    */
    HoLayout *m_layout;

    /**
    Je v layoutu otce?
    */
    int inLayout;

    /**
    Otec nebo NULL
    */
    HoWindow *parent;

    /**
    Je vytvo�en p��slu�n� objekt objekt v gui ([X]CreateWindow)
    */
    int created; 

    /**
    Kostruktor defaultniho okna 1 urovne.
  */
    HoWindow();

  /**
    Kostruktor. Okno bud m�t defaultn� w a h, layout otce (je-li) ur�� x, y.
    @param parent rodi�ovsk� okno nebo NULL
    @param title titulek
    @param style styl
  */
    HoWindow(HoWindow *parent, const HoString &title, int style = -1);

  /**
    Kostruktor. Okno bud m�t stanoven� w a h, layout otce (je-li) ur�� x, y.
    @param parent rodi�ovsk� okno nebo NULL
    @param title titulek
    @param w ���ka
    @param h v��ka
    @param style styl okna
  */ 
    HoWindow(HoWindow *parent, const HoString &title, int w, int h,
             int style = -1);

    HoWindow(HoWindow *parent, const HoString &title,
      int x, int y, int w, int h, int style = -1); // nebude v layoutu otce

    /**
    Spo��t� velikost okraj� okna dan�ho stylu.
    @param style styl okna
    @param topLevel je to okno na nejvy��� �rovni?
    @param x1 levy okraj nebo NULL
    @param x2 pravy okraj nebo NULL
    @param y1 horni okraj nebo NULL
    @param y2 dolni okraj nebo NULL
    */
    static void getBorders(int topLevel, int style,
                           int *x1, int *x2, int *y1, int *y2);

    /**
    Spo��t� velikost okraj� okna do prom�nn�ch dx1, dx2, dy1, dy2
    */
    virtual void getBorders() {
      getBorders((parent ? 0 : 1), style, &dx1, &dx2, &dy1, &dy2);
    };

 /**
    Vrac� po�adovanou v��ku okna. Mus� j�t zodpov�d�t ji� po zavol�n�
    konstruktoru tj. p�ed create.
    @return po�adovan� v��ka okna
 */
    virtual int getW() {return w;};
    
 /**
    Vrac� po�adovanou ���ku okna. Mus� j�t zodpov�d�t ji� po zavol�n�
    konstruktoru tj. p�ed create.
    @return po�adovan� ���ka okna
 */
    virtual int getH(){return h;};
    //virtual void paintSystem();
    
 /**
    U�ivatelova kresl�c� metoda.
    @param dc kresl�tko
 */
    virtual void paint(HoDC *dc); 
    
 /**
    vyvol� paint
 */
    virtual void rePaint();
    
#if hplatform==XW
    virtual void rePaintBorder();
#endif
    
 /**
    vytvo�� objekt gui a objekty gui v�ech dce�in�ch oken
    @return !0 pokud se povedlo
 */
    virtual int create();
    
 /**
    vrac� titulek
    @return titulek
 */
    virtual HoString &getText();
    
 /**
   nastav� titulek
   @param s nov� titulek
 */
    virtual void setText(const HoString &s); 

/**
  U�ivatel zm��knul my�.
  @param x x sou�adnice zm��knut� 0 je vlevo klientsk� oblasti
  @param y y sou�adnice zm��knut� 0 je naho�e klientsk� oblasti
  @param button 0 lev�, 1 prav�
*/
    virtual void mouseDown(int x, int y, int button){};
    
/**
  U�ivatel pustil my� zm��knutou v u�ivatelsk� oblasti okna.
  @param x x sou�adnice zm��knut� 0 je vlevo klientsk� oblasti
  @param y y sou�adnice zm��knut� 0 je naho�e klientsk� oblasti
  @param button 0 lev�, 1 prav�
*/   
    virtual void mouseUp(int x, int y, int button){};
    
    virtual void keyDown(wchar_t c, int code, int mods){};
 // Kdyz nic c==0, code==-1 (nikdy oboji najednou)
// mods zatim 0, bude tam Alt, Ctrl a Shift

/**
  U�ivatelsk� funkce - sm� se okno zav��t?
  @return 0 ne, 1 ano. V�choz� implementace vrac� jedna, pokud
  v�echny d�ti vrac� 1. (Kdy� 1. d�t� vr�t� 0, okam�it� ukon�� vyhodnocov�n�
  a vr�t� 0.)
*/
    virtual int canClose();
    
 /**
   Zru�� objekt gui a objekty gui v�ech dce�in�ch oken, vyvol� destruktor.
   Toto je ta prav� metoda, kterou zavol�me v reakci na stisk tla��tka konec.
 */
    virtual void destroy(); 
  /**
    Posune okno (u oken v layoutu nevolat p��mo).
    @param x x sou�adnice lev�ho horn�ho rohu v��i lev�mu horn�mu rohu
             klientsk� ��sti otce.
    @param y y sou�adnice lev�ho horn�ho rohu v��i lev�mu horn�mu rohu
             klientsk� ��sti otce.
    @param w ���ka okna v�etn� dekorace
    @param h v��ka okna v�etn� dekorace
  */
    virtual void move(int x, int y, int w, int h); 
    virtual void addWindow(HoWindow *w);
    virtual void removeWindow(HoWindow *w);
    virtual void addLayoutWindow(HoWindow *w);
    virtual void removeLayoutWindow(HoWindow *w);
    virtual void createChildren();
    virtual void pack();
/*
  Destruktor, nevol� se p��mo, ru�� jen objekt, ale ne gui. Je vol�n metodou destroy.
*/
    virtual ~HoWindow();
  private:
    /**
      Pomocn� funkce pro konstruktor.
      @param parent otec nebo NULL
      @param title titulek
      @param x
      @param y
      @param w
      @param h
      @param inLayout
      @param style
     */
    void init(HoWindow *parent, const HoString &title, int x, int y,
      int w, int h, int inLayout, int style);
};

class Ho {
  public:
    enum Key {
      KeyHome = 0,
      KeyEnd = 1,
      KeyIns = 2,
      KeyDel = 3,
      KeyPgUp = 4,
      KeyPgDown = 5,
      KeyLeft = 6,
      KeyRight = 7,
      KeyUp = 8,
      KeyDown = 9,
      KeyEsc = 10,
      KeyBcksp = 11
    };
};
#if hplatform==MW
HWND _HoCreateWindow(char *className, const char *titulek, HWND parent,
						   int x, int y, int w, int h, DWORD style);
#endif
#endif

