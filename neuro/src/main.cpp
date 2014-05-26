#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <time.h>

#include "howindow.h"
#include "hopushbutton.h"
#include "hoedit.h"
#include "hoapp.h"
#include "hodc.h"
#include "hostring.h"
#include "hothread.h"
#include "hosemaphore.h"

#include "ns.h"

std::vector<double> body;
HoSemaphore semafor;
HoSemaphore semaforKonceVlakna;
Sit *sit;

class OknoUceni: public HoWindow {
  public:
    OknoUceni(HoWindow *parent):
      HoWindow(parent, "Uèení",
        640, 480,
        0
/*        HoWindow::styleCaption |
        HoWindow::styleBorder |
        HoWindow::styleSizing*/
      ){};
    virtual void paint(HoDC *dc) {
      int wx = dc->getW();
      int wy = dc->getH();
      double min = 0, max = 0;
      int i;
      int faktor = 1;

      semafor.down();
      dc->setForeground(0xFFFF, 0xFFFF, 0xFFFF);
      dc->fillRectangle(0, 0, wx, wy);
      dc->setForeground(0, 0, 0);
      if (body.empty()) {
        dc->drawString(0, wy / 2, "®ádná data");
        goto konec;
      }
      char s[256];
      sprintf(s, "%i iterací", body.size());
      dc->drawString(0, wy / 2, s);



      if (body.size() == 1) {
        jedna:
        dc->drawLine(0, wy / 2, wx - 1, wy / 2);
        goto konec;
      }

      if (body.size() > 2048) {
        faktor = body.size() / 2048;
      }

      for (i = 0; i < body.size(); i += faktor) {
        if (min > body[i]) min = body[i];
        if (max < body[i]) max = body[i];
      }
      if (min == max) goto jedna;

      dc->drawLine(0, (int) (wy * (max - 0) / (max - min)) - 1, wx - 1, (int) (wy * (max - 0) / (max - min)) - 1);

      for (i = 0; i < body.size() - 1; i += faktor) {
        dc->drawLine(
          (int)(i / faktor * wx / (double)(body.size() / faktor - 1)),
          (int) (wy * (max - body[i]) / (max - min)), 
          (int)((i / faktor + 1) * wx / (double)(body.size() / faktor - 1)), 
          (int) (wy * (max - body[i + faktor]) / (max - min)) 
          );
      }

      konec:
      semafor.up();
    }
};

time_t cas = 0;
volatile bool konec = false;

void pushKonec(HoPushButton *b, void *param) {
  konec = true;
}

class HlavniOkno;
HlavniOkno *hlavni;
void pushZobrazit(HoPushButton *b, void *param);


class HlavniOkno : public HoWindow {
public:
  HoWindow *dite;
  HoPushButton *konecUceni;
  HoPushButton *zobrazit;
  HlavniOkno() : HoWindow(NULL, "Uèení",
        -1, -1, 800, 600,
        HoWindow::styleCaption |
        HoWindow::styleBorder |
        HoWindow::styleSizing
) {
    dite = new OknoUceni(this);
    konecUceni = new HoPushButton(this, "Nauceno", pushKonec);
    zobrazit = new HoPushButton(this, "Zobrazit", pushZobrazit);
    pack();
  };

  virtual int canClose() {
    if (!HoWindow::canClose()) return false;
    konec = true;
    semaforKonceVlakna.down();
    return true;
  }
};

void pushZobrazit(HoPushButton *b, void *param) {
  HoImage *img = new HoImage(200, 200);
  std::vector<double> vstup;
  vstup.reserve(2);
  vstup.push_back(0);
  vstup.push_back(0);
  std::vector<double> vystup;
  HoDC *dc = new HoDC(img);
 
  for (int i = 0; i < img->w; i++) {
    for (int j = 0; j < img->h; j++) {
      vstup[0] = -0.5 + 2 * i / (double)img->w;
      vstup[1] = -0.5 + 2 * j / (double)img->h;
      sit->vypocet(vstup, vystup);
      int r = (int)(0xFFFF * vystup[0] + 0.5);
      dc->setForeground(r, r, r);
      dc->putPixel(i, j);
    }
  }
  delete dc;
  HoDC dc2((hlavni->dite));
  dc2.putImage(0,0,img->w,img->h,img, 0,0);
  delete img;
}


bool krok(Sit *s) {
  if (konec) return false;
  semafor.down();
  body.push_back(s->sumChyba);
  bool ret = true;
  semafor.up();
  time_t t = time(NULL);
  if (t != cas) {
    cas = t;
    HoThread::sendEvent(NULL);
  }
  return ret;
}

void VlaknoVypoctu(void *) {
  std::vector<int> v;

#if 1
  v.push_back(2);
  v.push_back(3);
  v.push_back(1);

  std::vector<std::vector<std::vector<double> > > t;
  std::vector<double> in00;
  std::vector<double> in01;
  std::vector<double> in10;
  std::vector<double> in11;
  std::vector<double> out00;
  std::vector<double> out01;
  std::vector<double> out10;
  std::vector<double> out11;
  std::vector<std::vector<double> > xor00;
  std::vector<std::vector<double> > xor01;
  std::vector<std::vector<double> > xor10;
  std::vector<std::vector<double> > xor11;
 
  in00.push_back(0);
  in00.push_back(0);
  out00.push_back(0);
  xor00.push_back(in00);
  xor00.push_back(out00);

  in01.push_back(0);
  in01.push_back(1);
  out01.push_back(1);
  xor01.push_back(in01);
  xor01.push_back(out01);

  in10.push_back(1);
  in10.push_back(0);
  out10.push_back(1);
  xor10.push_back(in10);
  xor10.push_back(out10);

  in11.push_back(1);
  in11.push_back(1);
  out11.push_back(0);
  xor11.push_back(in11);
  xor11.push_back(out11);

  t.push_back(xor00);
  t.push_back(xor01);
  t.push_back(xor10);
  t.push_back(xor11);
#else

  v.push_back(1);
  v.push_back(1);

  std::vector<std::vector<std::vector<double> > > t;
  std::vector<double> in0;
  std::vector<double> in1;
  std::vector<double> out0;
  std::vector<double> out1;
  std::vector<std::vector<double> > id0;
  std::vector<std::vector<double> > id1;
 
  in0.push_back(0);
  out0.push_back(0);
  id0.push_back(in0);
  id0.push_back(out0);

  in1.push_back(1);
  out1.push_back(1);
  id1.push_back(in1);
  id1.push_back(out1);

  t.push_back(id0);
  t.push_back(id1);
#endif
  sit = new Sit(v);
  semaforKonceVlakna.down();
  sit->trenink(t, krok);
  semaforKonceVlakna.up();
}



void event(void *) {
  if (konec) return;
  hlavni->dite->rePaint();
}

int homain(int argc,char * argv[]){
  HoApp *a;

  a = new HoApp(argc, argv);
  a->userEvent = event;
  hlavni = new HlavniOkno;
  hlavni->create();
  HoThread vlakno(VlaknoVypoctu, NULL);
  vlakno.run();
  a->run();
  delete a;
  delete sit;
  return 0;
}

