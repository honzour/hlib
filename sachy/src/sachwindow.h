#ifndef sachwindowH
#define sachwindowH

#include <howindow.h>
#include <hostring.h>
#include <hoapp.h>
#include <hodc.h>

#include <sachy.h>

class SachWindow: public HoWindow {
  protected:
    HoImage * kameny[2][2][7];
    int oznaceno1, oznaceno2;
    bool bily;
    TUloha *uloha;
  public:
    /**
      Má metoda canClose poslat zprávu odsedl? Ne, pokud je okno zavøeno
      programem.
    */
    bool mamPoslatZpravu;
    int id;
    SachWindow(HoWindow *parent, const HoString &title, int w, int h, int id,
      bool bily);
    virtual int canClose();
    ~SachWindow();
    virtual void zobrazPole(HoDC *dc, int x, int y);
    virtual void paint(HoDC *dc);
    virtual void mouseDown(int x, int y, int button);
    virtual void tah(int t);
};

#endif

