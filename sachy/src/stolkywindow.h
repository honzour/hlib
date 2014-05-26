#ifndef stolkywindowH
#define stolkywindowH

#include <howindow.h>

class StolkyWindow: public HoWindow {
  public:
    int pocetOken;
    bool programKonci;
    StolkyWindow(HoWindow *parent);
    virtual int canClose();
};

#endif

