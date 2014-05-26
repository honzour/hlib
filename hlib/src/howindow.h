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
Tøída reprezentující okno.
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
    XIC xic; /* Vstupní kontext */
#endif
    /**
       Ma se po ukonceni okna ukoncit i program, je-li okno jedine hlavni?
    */
    bool exitIfLast;
    /**
    ©íøka okna vèetnì dekorace, nastaví se ji¾ v konstruktoru. -1 znamená,
    ¾e ji má urèit layout, ale dosud neurèil.
    */
    int w;

    /**
    Vý¹ka okna vèetnì dekorace, nastaví se ji¾ v konstruktoru. -1 znamená,
    ¾e ji má urèit layout, ale dosud neurèil.
    */
    int h;

    /**
    Minimální w klientské oblasti.
    */
    static int minW;
    
    /**
    Minimální h klientské oblasti.
    */
    static int minH;
    
    /**
    X souøadnice levého horního rohu okna (resp. jeho dekorace) vùèi levému
    hornímu rohu klientské èásti otce, nastaví se ji¾ v konstruktoru.
    -1 znamená, ¾e ji má urèit layout otce, ale dosud neurèil.
    */
    int x;

    /**
    Y souøadnice levého horního rohu okna (resp. jeho dekorace) levému hornímu
    rohu klientské èásti otce, nastaví se ji¾ v konstruktoru. -1 znamená, ¾e ji
    má urèit layout otce, ale dosud neurèil.
    */
    int y;

    /**
    ©íøka levé dekorace okna.
    */
    int dx1;

    /**
    ©í¹ka pravé dekorace okna vèetnì rolovadla.
    */
    int dx2;

    /**
    Vý¹ka horní dekorace okna vcetnì titulku, menu.
    */
    int dy1;

    /**
    Vý¹ka dolní dekorace okna vèetne rolovadla.
    */
    int dy2;

    /**
    Hodnoty stylu okna
    */
    enum {
    /**
      Má okno titulek (s názvem)? U okna úrovnì 1 v¾dy.
    */
      styleCaption = 1,
      /**
      Má okno rámeèek? U okna urovnì 1 v¾dy.
      */
      styleBorder = 2,
      /**
      Má okno rámeèek pro zvìt¹ování u¾ivatelem?
      */
      styleSizing = 4
    };

    /**
    Styl okna
    */
    int style;

    /**
    Titulek nahoøe v dekoraci okna (je-li).
    */
    HoString *title; 

    /**
    Strom v¹ech dceøiných oken,
    */
    HoTree *windowTree;

    /**
    Layout dceøiných oken. Nemusí v nìm být v¹echna.
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
    Je vytvoøen pøíslu¹ný objekt objekt v gui ([X]CreateWindow)
    */
    int created; 

    /**
    Kostruktor defaultniho okna 1 urovne.
  */
    HoWindow();

  /**
    Kostruktor. Okno bud mít defaultní w a h, layout otce (je-li) urèí x, y.
    @param parent rodièovské okno nebo NULL
    @param title titulek
    @param style styl
  */
    HoWindow(HoWindow *parent, const HoString &title, int style = -1);

  /**
    Kostruktor. Okno bud mít stanovené w a h, layout otce (je-li) urèí x, y.
    @param parent rodièovské okno nebo NULL
    @param title titulek
    @param w ¹íøka
    @param h vý¹ka
    @param style styl okna
  */ 
    HoWindow(HoWindow *parent, const HoString &title, int w, int h,
             int style = -1);

    HoWindow(HoWindow *parent, const HoString &title,
      int x, int y, int w, int h, int style = -1); // nebude v layoutu otce

    /**
    Spoèítá velikost okrajù okna daného stylu.
    @param style styl okna
    @param topLevel je to okno na nejvy¹¹í úrovni?
    @param x1 levy okraj nebo NULL
    @param x2 pravy okraj nebo NULL
    @param y1 horni okraj nebo NULL
    @param y2 dolni okraj nebo NULL
    */
    static void getBorders(int topLevel, int style,
                           int *x1, int *x2, int *y1, int *y2);

    /**
    Spoèítá velikost okrajù okna do promìnných dx1, dx2, dy1, dy2
    */
    virtual void getBorders() {
      getBorders((parent ? 0 : 1), style, &dx1, &dx2, &dy1, &dy2);
    };

 /**
    Vrací po¾adovanou vý¹ku okna. Musí jít zodpovìdìt ji¾ po zavolání
    konstruktoru tj. pøed create.
    @return po¾adovaná vý¹ka okna
 */
    virtual int getW() {return w;};
    
 /**
    Vrací po¾adovanou ¹íøku okna. Musí jít zodpovìdìt ji¾ po zavolání
    konstruktoru tj. pøed create.
    @return po¾adovaná ¹íøka okna
 */
    virtual int getH(){return h;};
    //virtual void paintSystem();
    
 /**
    U¾ivatelova kreslící metoda.
    @param dc kreslítko
 */
    virtual void paint(HoDC *dc); 
    
 /**
    vyvolá paint
 */
    virtual void rePaint();
    
#if hplatform==XW
    virtual void rePaintBorder();
#endif
    
 /**
    vytvoøí objekt gui a objekty gui v¹ech dceøiných oken
    @return !0 pokud se povedlo
 */
    virtual int create();
    
 /**
    vrací titulek
    @return titulek
 */
    virtual HoString &getText();
    
 /**
   nastaví titulek
   @param s nový titulek
 */
    virtual void setText(const HoString &s); 

/**
  U¾ivatel zmáèknul my¹.
  @param x x souøadnice zmáèknutí 0 je vlevo klientské oblasti
  @param y y souøadnice zmáèknutí 0 je nahoøe klientské oblasti
  @param button 0 levý, 1 pravý
*/
    virtual void mouseDown(int x, int y, int button){};
    
/**
  U¾ivatel pustil my¹ zmáèknutou v u¾ivatelské oblasti okna.
  @param x x souøadnice zmáèknutí 0 je vlevo klientské oblasti
  @param y y souøadnice zmáèknutí 0 je nahoøe klientské oblasti
  @param button 0 levý, 1 pravý
*/   
    virtual void mouseUp(int x, int y, int button){};
    
    virtual void keyDown(wchar_t c, int code, int mods){};
 // Kdyz nic c==0, code==-1 (nikdy oboji najednou)
// mods zatim 0, bude tam Alt, Ctrl a Shift

/**
  U¾ivatelská funkce - smí se okno zavøít?
  @return 0 ne, 1 ano. Výchozí implementace vrací jedna, pokud
  v¹echny dìti vrací 1. (Kdy¾ 1. dítì vrátí 0, okam¾itì ukonèí vyhodnocování
  a vrátí 0.)
*/
    virtual int canClose();
    
 /**
   Zru¹í objekt gui a objekty gui v¹ech dceøiných oken, vyvolá destruktor.
   Toto je ta pravá metoda, kterou zavoláme v reakci na stisk tlaèítka konec.
 */
    virtual void destroy(); 
  /**
    Posune okno (u oken v layoutu nevolat pøímo).
    @param x x souøadnice levého horního rohu vùèi levému hornímu rohu
             klientské èásti otce.
    @param y y souøadnice levého horního rohu vùèi levému hornímu rohu
             klientské èásti otce.
    @param w ¹íøka okna vèetnì dekorace
    @param h vý¹ka okna vèetnì dekorace
  */
    virtual void move(int x, int y, int w, int h); 
    virtual void addWindow(HoWindow *w);
    virtual void removeWindow(HoWindow *w);
    virtual void addLayoutWindow(HoWindow *w);
    virtual void removeLayoutWindow(HoWindow *w);
    virtual void createChildren();
    virtual void pack();
/*
  Destruktor, nevolá se pøímo, ru¹í jen objekt, ale ne gui. Je volán metodou destroy.
*/
    virtual ~HoWindow();
  private:
    /**
      Pomocná funkce pro konstruktor.
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

