#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "howindow.h"
#include "hopushbutton.h"
#include "hoedit.h"
#include "hoapp.h"
#include "hodc.h"
#include "hostring.h"

struct TZvire {
  double teplota;
  double dedicnost;
};

class MojeOkno: public HoWindow {
  public:
    MojeOkno(HoWindow *parent, int x, int y):

      HoWindow(parent, "Evoluce",
        x, y,
        640, 480,
        HoWindow::styleCaption |
        HoWindow::styleBorder |
        HoWindow::styleSizing
      ){
      };
    virtual void paint(HoDC *dc) {
      dc->setForeground(0, 0xFFFF, 0);
      std::vector<TZvire> v;
      TZvire z;
      int i;

      for (i = 0; i < 100; i++) {
        z.teplota = -1.0 + (rand() % 200) / 100.0;
	z.dedicnost = 0.0 + (rand() % 100) / 100.0;
	v.push_back(z);
      }
      
      int y = 240, y2;
      for (i = 0; i < 640; i++) {
        y2 = (int) (240 * sin(i / 50.0) + 240.5);
	dc->drawLine(i, y, i + 1, y2);
	y = y2;
      }
    }
};

int homain(int argc,char * argv[]){
  HoApp *a;
  MojeOkno *hlavni;

  a = new HoApp(argc, argv);
  hlavni = new MojeOkno(NULL, 50, 50);
  hlavni->create();
  a->run();
  delete a;
  return 0;
}

