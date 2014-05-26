#include <stdlib.h>

#include <sachy.h>
#include <generato.h>
#include <globruti.h>

#include "sachwindow.h"
#include "struktury.h"
#include "global.h"
#include "push.h"

#include <pole.h>


static char nazev(int i) {
  switch (i) {
    case 1: return 'P';
    case 2: return 'J';
    case 3: return 'S';
    case 4: return 'V';
    case 5: return 'D';
    case 6: return 'K';
    case -1: return 'p';
    case -2: return 'j';
    case -3: return 's';
    case -4: return 'v';
    case -5: return 'd';
    case -6: return 'k';
    default: return ' ';
  }
}

void nakresliFiguru(HoDC &dc, int typ, int fx, int fy) {
  dc.setForeground(0, 0, 0);
  switch (typ) {
    case -5:
      dc.drawLine(7, 37, 3, 10);
      dc.drawLine(33, 37, 37, 10);
      dc.drawLine(33, 37, 7, 37);
      dc.drawLine(3, 10, 15, 20);
      dc.drawLine(37, 10, 25, 20);
      dc.drawLine(15, 20, 20, 7);
      break;
    default:
     dc.drawString((40 - fx) / 2, (40 - fy ) / 2, nazev(typ));
  }
}

SachWindow::SachWindow(HoWindow *parent, const HoString &title, int w, int h,
  int id, bool bily)
  :HoWindow(parent, title, 0, 0, w, h,
    HoWindow::styleSizing | HoWindow::styleCaption) {
  int i, j, k;
  int fx, fy;

  uloha = InitUlohu();
  this->bily = bily;
  oznaceno1 = -1;
  oznaceno2 = -1;
  mamPoslatZpravu = true;
  this->id = id;
  HoMainApp->getDefaultFontSize(&fx, &fy);
  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      for (k = 0; k < 7; k++) {
        kameny[i][j][k] = new HoImage(40, 40);
        HoDC idc(kameny[i][j][k]);
        if (i) idc.setForeground(200 << 8, 200 << 8, 200 << 8);
        else idc.setForeground(255 << 8, 255 << 8, 255 << 8);
        idc.fillRectangle(0, 0, 40, 40);
        nakresliFiguru(idc, (1 - 2 * j) * k, fx, fy);
     }
}
    
SachWindow::~SachWindow() {
  DoneUlohu(&uloha);
  int i, j, k;
  for (i = 0; i < 2; i++)
    for (j = 0; j < 2; j++)
      for (k = 0; k < 7; k++)
        delete kameny[i][j][k];
}

void SachWindow::zobrazPole(HoDC *dc, int x, int y) {
  int ox = -1, oy = -1;
  if (oznaceno1 >= 0) {
    ox = oznaceno1 % 10 - 1;
    oy = oznaceno1 / 10 - 2;
  } else if (oznaceno2 >= 0) {
    ox = oznaceno2 % 10 - 1;
    oy = oznaceno2 / 10 - 2;
  } 
  int xz = x * 40;
  int yz = 280 - y * 40;
  int p = uloha->pozice.sch[a1 + x + 10 * y];
  dc->putImage(xz, yz, kameny[0][0][0]->w,
  kameny[0][0][0]->w,
  kameny[1 - ((x + y) & 1)][p < 0 ? 1 : 0][abs(p)], 0, 0);
  if (x == ox && y == oy) {
    dc->setForeground(0, 0, 0);
    dc->drawLine(xz + 3, yz + 3, xz + 3, yz + 36);
    dc->drawLine(xz + 36, yz + 3, xz + 36, yz + 36);
    dc->drawLine(xz + 3, yz + 3, xz + 36, yz + 3);
    dc->drawLine(xz + 3, yz + 36, xz + 36, yz + 36);
  }
}

void SachWindow::paint(HoDC *dc) {
  
  int i, j;
  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++) {
      zobrazPole(dc, i, j);
    }
}

int SachWindow::canClose() {

  puts("SachWindow::canClose()");
  if (!HoWindow::canClose()) return 0;
  if (!mamPoslatZpravu) return 1;
  SCStolky::iterator i = stolky.find(id);
  if (i == stolky.end()) return 1;
  if (i->second->id <= 0) return 1;
  pushZavrelStolek(i->second->id);
  i->second->sw = NULL;
  return 1;
}

void SachWindow::mouseDown(int x, int y, int button) {
 
  if (bily && !uloha->pozice.bily ||
      !bily && uloha->pozice.bily) return;
  int i = x / 40;
  int j = y / 40;
  if (i > 7 || j > 7) {
    return;
  }
  j = 7 - j;
  int p = uloha->pozice.sch[a1 + i + 10 * j];
  if (oznaceno1 < 0) {
    if (!p) return;
    if (p < 0 && bily) return;
    if (!p > 0 && !bily) return;
    oznaceno1 = a1 + i + j * 10;
  } else {
    if (p > 0 && bily || p < 0 && !bily) {
      oznaceno1 = a1 + i + j * 10;
    } else {
      int o2 = a1 + i + j * 10;
      NalezTahy(uloha);
      JenPripustne(uloha);
      if (JeTam2((u8)oznaceno1, (u8)o2, uloha)) {
        int tah = DoplnTah(oznaceno1, o2, uloha);
        pushTah(id, tah);
      } else {
        printf("Nepøípustný tah\n");
      }
    }
  }
  rePaint();
}

void SachWindow::tah(int t) {
  if (!KorektniTah((u16) t, uloha)) {
    puts("Nekorektní tah");
  }
  GlobTahni(1, uloha, 1, t);
  oznaceno1 = -1;
  rePaint();
}

