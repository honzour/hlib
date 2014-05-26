#ifndef HoMsgboxH
#define HoMsgboxH
#include "hostring.h"
#include "howindow.h"

class HoMsgBox: public HoWindow {
  public:
    static void msgBox(HoString caption, HoString label,
      void (* push) (HoMsgBox *, void *) = NULL, void *param = NULL
      ) {
      HoMsgBox *b = new HoMsgBox(caption, label, push, param);
      b->create();
    }
    void (* push) (HoMsgBox *, void *);
    void *param;
  private:
    HoString label;
    HoMsgBox(HoString caption, HoString label,
      void (* push) (HoMsgBox *, void *), void *param
    ); 
    virtual ~HoMsgBox() {};
};

#endif
