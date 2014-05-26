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

std::vector<int> body;


class Okno : public HoWindow {
public:
  Okno() : HoWindow(NULL, "Hlavní okno", 50, 50, 0x600, 0x300, HoWindow::styleSizing) {
  }

  ~Okno() {
  }

  bool getCenter(int r, int ax, int ay, int bx, int by, int cx, int cy, int &rx, int &ry) {

    int bax = bx - ax;
    int bay = by - ay;
    int bcx = bx - cx;
    int bcy = by - cy;

    double dcx, dcy, dax, day, Ax, Ay, Cx, Cy, la, lc;

    dax = bay;
    day = -bax;
    la = 2 * sqrt(dax * dax + day * day) / r;
    dax /= la;
    day /= la;
    
    dcx = -bcy;
    dcy = bcx;
    lc = 2 * sqrt(dcx * dcx + dcy * dcy) / r;
    dcx /= lc;
    dcy /= lc;

    Ax = ax + dax;
    Ay = ay + day;
    Cx = cx + dcx;
    Cy = cy + dcy;

    double u = (bay * (Ax - Cx) + bax * (Cy - Ay)) / (bcx * bay - bcy * bax);

//    printf("%f %f %f %f %f\n", Ax, Ay, (double)ax, (double)ay, u);
//    u = 0;
 
    rx = (int) (Cx + u * bcx + 0.5);
    ry = (int) (Cy + u * bcy + 0.5);
  }

#define polomer 50 
  virtual void paint(HoDC *dc) {
    int i;
    dc->setForeground(0xFFFF, 0xFFFF, 0xFFFF);
    dc->fillRectangle(0, 0, getW(), getH());
    dc->setForeground(0, 0, 0);

    for (i = 0; i < ((int)body.size()) - 3; i += 2)
    dc->drawLine(body[i], body[i + 1], body[i + 2], body[i + 3]);
    dc->setForeground(0, 0xFFFF, 0);
    for (i = 0; i < ((int) body.size()) - 5; i += 2) {
      int ax, bx, cx, ay, by, cy;
      int rx, ry;

      ax = body[i];
      ay = body[i + 1];
      bx = body[i + 2];
      by = body[i + 3];
      cx = body[i + 4];
      cy = body[i + 5];
      getCenter(2*polomer, ax, ay, bx, by, cx, cy, rx, ry);
      dc->fillEllipse(rx - polomer, ry - polomer, 2*polomer, 2*polomer);
    }
  }
  virtual void mouseDown(int x, int y, int button) {
    if (button) {
      body.clear();
      rePaint();
    } else {
      if (body.empty() || x != body[body.size() - 2] || x != body[body.size() - 1]) {
        body.push_back(x);
        body.push_back(y);
        rePaint();
      }
    }
  }
	
};

int homain(int argc,char * argv[]){
  HoApp *a;
  HoWindow *hlavni;
 
  a = new HoApp(argc, argv);
  hlavni = new Okno;
  hlavni->create();
  a->run();
  delete a;
  return 0;
}

