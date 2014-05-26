#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "howindow.h"
#include "hoapp.h"
#include "hodc.h"
#include "hostring.h"
#include "hopushbutton.h"

class MinyOkno: public HoWindow{
  public:
    int fx, fy, stav;
    int ct, ct2, ct3;
    char miny[10][10];
    MinyOkno();
    virtual void paint(HoDC *dc);
    virtual void paint(HoDC *dc, int i, int j);
    int kolikmin(int x, int y);
    void rekurze(int x, int y);
    void nova();
    virtual void mouseDown(int x, int y, int button);
};

MinyOkno *hlavni; 
HoPushButton *b;

void bpush(HoPushButton *, void *){
  hlavni->nova();
  hlavni->rePaint();
  hlavni->setText("Hledání min");
}

static int getFontX() {
  int x;
  HoMainApp->getDefaultFontSize(&x, NULL);
  return x;
}

static int getFontY() {
  int y;
  HoMainApp->getDefaultFontSize(NULL, &y);
  return y;
}

MinyOkno::MinyOkno():
  HoWindow(NULL, "Hledání min", 15 * getFontY(), 19 * getFontY()) {
  
  srand(time(NULL));
  ct = (int) (1.3 * getFontY());
  ct2 = (int)(0.9 * ct);
  ct3 = ct >> 1;
  b = new HoPushButton(this, "Nová hra", 10 * getFontX(), 14 * getFontY(),
    13 * getFontX(), 2 * getFontY(), bpush);
  nova();
}

void MinyOkno::nova(){
  int i;
  for (i = 0; i < 100; i++)
    miny[0][i] = ((rand() & 255) > 30 ? 0: 1);
  stav = 0;
}

void MinyOkno::rekurze(int x, int y) {
  int i, j;
  if (x < 0 || y < 0 || x > 9 || y > 9 || miny[x][y])
    return;
  miny[x][y] = 10;
  HoDC dc(this);
  paint(&dc, x, y);
  if (kolikmin(x,y)) return;
    for (i = x - 1; i < x + 2; i++)
      for (j = y - 1; j < y + 2; j++)
        rekurze(i, j);
}

int MinyOkno::kolikmin(int x, int y) {
  int i, j, sum;
  sum = 0;
  for (i = x - 1; i < x + 2; i++)
    for (j = y - 1; j < y + 2; j++) {
      if (i == x && j == y || i < 0 || i > 9 || j < 0 || j > 9)
        continue;
      if (miny[i][j] & 1 == 1)
        sum++;
    }
  return sum;
}

void MinyOkno::mouseDown(int x, int y, int button) {
  if (stav == 1) return;
  x-=ct3; y-=ct3;
  x/=ct; y/=ct;
  if (x < 0 || x > 9 || y < 0 || y > 9) return;
  HoDC dc(this);
  switch(button) {
    case 0:
      switch (miny[x][y]) {
        case 0: rekurze(x, y); break;
        case 1:
	  miny[x][y] = 11;
	  paint(&dc, x, y);
          stav = 1;
	  setText("Vybuchl jsi");
          break;
        default: break;
      }
      break;
    case 1:
      switch(miny[x][y]) {
        case 0:
	  miny[x][y] = 2;
	  paint(&dc, x, y);
	  break;
        case 1:
	  miny[x][y] = 3;
	  paint(&dc, x, y);
	  break;
        case 2:
	  miny[x][y] = 0;
	  paint(&dc, x, y);
	  break;
        case 3:
	  miny[x][y] = 1;
	  paint(&dc, x, y);
	  break;
        default: break;
      }
      break;
  }
}
void MinyOkno::paint(HoDC *dc, int i, int j) {
  char s[2];
  
  switch(miny[i][j]){
    case 0: // kryta prazdna
    case 1: // kryta mina 
      dc->setForeground(0xFFFF, 0xFFFF, 0);
      dc->fillRectangle(ct3 + ct * i, ct3 + ct * j, ct2, ct2);
      break;
    case 2: // kryta oznacena prazdna
    case 3: // kryta oznacena mina 
      dc->setForeground(0xFFFF, 0xFFFF, 0);
      dc->fillRectangle(ct3 + ct * i, ct3 + ct * j, ct2, ct2);
      dc->setForeground(0,0,0);
      dc->drawString(ct3 + (ct2 - fx) / 2 + ct * i, ct3 + (ct2 - fy) / 2 + ct * j, "M");
      break;
    case 10:
      dc->setForeground(0xFFFF, 0, 0xFFFF);
      dc->fillRectangle(ct3 + ct * i, ct3 + ct * j, ct2, ct2);
      dc->setForeground(0, 0, 0);
      sprintf(s, "%i", kolikmin(i, j));
      dc->drawString(ct3 + (ct2 - fx) / 2 + ct * i, ct3 + (ct2 - fy) / 2 + ct * j, s);
      break;
    case 11: // mina 
      dc->setForeground(0xFFFF, 0xFFFF, 0);
      dc->fillRectangle(ct3 + ct * i, ct3 + ct * j, ct2, ct2);
      dc->setForeground(0, 0, 0);
      dc->fillEllipse(
        ct3 + (ct - ct2) + ct * i,
        ct3 + (ct - ct2) + ct * j,
        ct3 + 2 * (ct - ct2),
        ct3 + 2 * (ct - ct2)
        );
      break;			
    default:
      dc->setForeground(0xFFFF, 0, 0xFFFF);
      dc->fillRectangle(ct3 + ct * i, ct3 + ct * j, ct2, ct2);
      break;
  }
}

void MinyOkno::paint(HoDC *dc) {
  int i,j;

  dc->getFontInfo(&fx, &fy);
  for(i=0; i<10; i++)
    for(j=0; j<10; j++)
      paint(dc, i, j);
}

int homain(int argc,char * argv[]) {
  HoApp *a;

  a = new HoApp(argc, argv);
  hlavni = new MinyOkno;
  hlavni->create();
  a->run();
  delete a;
  return 0;
}

