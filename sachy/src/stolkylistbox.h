#ifndef stolkylistboxH
#define stolkylistboxH
#include <vector>
#include <holistbox.h>
#include "struktury.h"

using namespace std;

extern HoString getListboxString(SCStolek *stolek);

class StolkyListbox: public HoListbox {
    public:
    vector<SCStolek *> stoly;

    StolkyListbox(HoWindow *w, int dx, int dy)
      : HoListbox(w, dx, dy, NULL) {};
    void insert(SCStolek *stolek);
    void changed(int stolek);
    void deleted(int stolek);
    SCStolek * getAktStolek() {
      if (selected > 0 && selected <= stoly.size()) return stoly[selected - 1];
     // printf("selected = %i size() = %i\n", selected, stoly.size());
      return NULL;
    }
    ~StolkyListbox() {stoly.clear();}
};
#endif

