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
  Tøída implementující listbox. Lze inicializovat i v re¾imu bez mo¾nosti
  oznaèení.
*/
class HoListbox : public HoWindow
{
public:
  /**
    Oznaèený prvek, od jednièky.
  */
  int selected;
  /**
    Funkce (callback), která se zavolá pøi zmìnì oznaèení.
  */
  void (*change)(int);
  /**
    Vytvoøí listbox v kontajneru rodièe.
    @param parent rodièovské okno
    @param w ¹íøka v bodech
    @param h vý¹ka v bodech
    @param change callback pøi zmìnì oznaèené polo¾ky
    @param enableSelect je povolené oznaèovat prvky?
  */
  HoListbox(HoWindow *parent, int w, int h, void (*change)(int),
    bool enableSelect = true);
  /**
    Vytvoøí listbox mimo kontajner rodièe.
    @param parent rodièovské okno
    @param w ¹íøka v bodech
    @param h vý¹ka v bodech
    @param x x levého horního rohu v souøadnicích rodièe
    @param y y levého horního rohu v souøadnicích rodièe
    @param change callback pøi zmìnì oznaèené polo¾ky
    @param enableSelect je povolené oznaèovat prvky?
  */
  HoListbox(HoWindow *parent, int x, int y, int w, int h,
    void (*change)(int), bool enableSelect = true);
  /**
    Pøidá øetìzec do listboxu na konec.
    @param s nový øetìzec
  */
  virtual void addString(const HoString &s);
  /**
    Odstraní øetìzec z listboxu.
    @param index který
    @return true ok
  */
  virtual bool removeString(int index);
  /**
    Zmìní øetìzec v listboxu.
    @param index který
    @param s nová hodnota
    @return true ok
  */
  virtual bool changeString(int index, const HoString &s);
  /**
    Vytvoøí prvek gui.
    @return true ok
  */
  virtual int create();
  /**
    Virtuální destruktor.
  */
  virtual ~HoListbox();
  /**
    Seznam øetìzcù listboxu.
  */
  HoTree *stringy;
  /**
    Je povolené oznaèovat prvky?
  */
  bool enableSelect;

private:
  virtual void initTree();
  virtual void paint(HoDC *dc);
  virtual void mouseDown(int x, int y, int button);
};
#endif

