#ifndef HoListboxH
#define HoListboxH
#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif
#include "howindow.h"
#include "hostring.h"

/**
  T��da implementuj�c� listbox. Lze inicializovat i v re�imu bez mo�nosti
  ozna�en�.
*/
class HoListbox : public HoWindow
{
public:
  /**
    Ozna�en� prvek, od jedni�ky.
  */
  int selected;
  /**
    Funkce (callback), kter� se zavol� p�i zm�n� ozna�en�.
  */
  void (*change)(int);
  /**
    Vytvo�� listbox v kontajneru rodi�e.
    @param parent rodi�ovsk� okno
    @param w ���ka v bodech
    @param h v��ka v bodech
    @param change callback p�i zm�n� ozna�en� polo�ky
    @param enableSelect je povolen� ozna�ovat prvky?
  */
  HoListbox(HoWindow *parent, int w, int h, void (*change)(int),
    bool enableSelect = true);
  /**
    Vytvo�� listbox mimo kontajner rodi�e.
    @param parent rodi�ovsk� okno
    @param w ���ka v bodech
    @param h v��ka v bodech
    @param x x lev�ho horn�ho rohu v sou�adnic�ch rodi�e
    @param y y lev�ho horn�ho rohu v sou�adnic�ch rodi�e
    @param change callback p�i zm�n� ozna�en� polo�ky
    @param enableSelect je povolen� ozna�ovat prvky?
  */
  HoListbox(HoWindow *parent, int x, int y, int w, int h,
    void (*change)(int), bool enableSelect = true);
  /**
    P�id� �et�zec do listboxu na konec.
    @param s nov� �et�zec
  */
  virtual void addString(const HoString &s);
  /**
    Odstran� �et�zec z listboxu.
    @param index kter�
    @return true ok
  */
  virtual bool removeString(int index);
  /**
    Zm�n� �et�zec v listboxu.
    @param index kter�
    @param s nov� hodnota
    @return true ok
  */
  virtual bool changeString(int index, const HoString &s);
  /**
    Vytvo�� prvek gui.
    @return true ok
  */
  virtual int create();
  /**
    Virtu�ln� destruktor.
  */
  virtual ~HoListbox();
  /**
    Seznam �et�zc� listboxu.
  */
  HoTree *stringy;
  /**
    Je povolen� ozna�ovat prvky?
  */
  bool enableSelect;

private:
  virtual void initTree();
  virtual void paint(HoDC *dc);
  virtual void mouseDown(int x, int y, int button);
};
#endif

