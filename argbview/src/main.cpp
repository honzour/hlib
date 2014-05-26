#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "howindow.h"
#include "hopushbutton.h"
#include "hoedit.h"
#include "hoapp.h"
#include "hodc.h"
#include "hostring.h"
#include "holabel.h"

class OknoObrazku: public HoWindow{
  HoImage *img;
  HoImage *akt;
  int zvetseni;
  public:
    OknoObrazku(HoImage *img, const char *nazev):
      HoWindow(NULL, nazev, img->w + 50, img->h + 50,
      HoWindow::styleBorder | HoWindow::styleCaption | HoWindow::styleSizing) {
        this->img = img;
        akt = new HoImage(*img, 1);
        zvetseni = 1;
      };
    virtual void paint(HoDC *dc) {
      dc->putImage(0, 0, akt->w, akt->h, akt, 0, 0);
    }

    virtual void mouseDown(int x, int y, int button) {
      if (!button) {
        if (zvetseni >= 16) return;
        zvetseni *= 2;
      } else {
        if (zvetseni <= 1) return;
        zvetseni /= 2;
      }

      delete akt;
      akt = new HoImage(*img, zvetseni);
      rePaint();
    }

    virtual ~OknoObrazku() {
      delete img;
      delete akt;
    }
};


class MojeOkno: public HoWindow{
  public:
    MojeOkno(HoWindow *parent, const HoString &title):
    HoWindow(parent, title){};
};

HoImage *obraz = NULL;
HoWindow *hlavni, *edl;
HoEdit *es, *ew, *et, *ep;

HoImage *nactiObrazek(char *filename, char *w, char *typ, char *pozadi,
                      HoImage **oboji = NULL,
                      HoImage **alfaObraz = NULL) {
  int width = atoi(w);
  if (width <= 0) {
    width = 64;
  }
  bool t1555 = !strcmp(typ, "1555");

  unsigned poz = strtol(pozadi, NULL, 16);
  
  unsigned char pr = poz >> 16;
  unsigned char pg = poz >> 8;
  unsigned char pb = poz;


  FILE *f = fopen(filename, "rb");
  if (!f) return NULL;
  fseek(f, 0, SEEK_END);
  long byte = ftell(f);
  fseek(f, 0, SEEK_SET);
  


  int height;

  if (t1555) {
    height = byte / (2 * width);
  } else {
    height = byte / (4 * width);
  }

  unsigned char *x = (unsigned char *) malloc(width * height * 3);
  unsigned char *d = (unsigned char *) malloc(width * height * 3);
  unsigned char *a = (unsigned char *) malloc(width * height * 3);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (t1555) {
        unsigned short w;
        fread((void *)&w, 2, 1, f);
        a[i * width * 3 + j * 3] = (w >> 15) * 255;
        a[i * width * 3 + j * 3 + 1] = (w >> 15) * 255;
        a[i * width * 3 + j * 3 + 2] = (w >> 15) * 255;

        d[i * width * 3 + j * 3] = ((w >> 10) & 31) << 3;
        d[i * width * 3 + j * 3 + 1] = ((w >> 5) & 31) << 3;
        d[i * width * 3 + j * 3 + 2] = ((w >> 0) & 31) << 3;

        int k, l;
        for ((k = i * width * 3 + j * 3), (l = 0); k < i * width * 3 + j * 3 + 3; k++, l++) {
          x[k] = (a[k] ? d[k] : poz >> ((2 - l) * 8));
        }
        
      } else {
        unsigned w;
        fread((void *)&w, 4, 1, f);
        a[i * width * 3 + j * 3] =     (w >> 24);
        a[i * width * 3 + j * 3 + 1] = (w >> 24);
        a[i * width * 3 + j * 3 + 2] = (w >> 24);
        
        d[i * width * 3 + j * 3] =     (w >> 16);
        d[i * width * 3 + j * 3 + 1] = (w >> 8);
        d[i * width * 3 + j * 3 + 2] = (w >> 0);
        
        x[i * width * 3 + j * 3] =     (unsigned char) (pr * (255 - (w >> 24)) / 255.0 + ((w >> 16) & 255) * (w >> 24) / 255.0 + 0.5);
        x[i * width * 3 + j * 3 + 1] = (unsigned char) (pg * (255 - (w >> 24)) / 255.0 + ((w >> 8) & 255) * (w >> 24) / 255.0 + 0.5);
        x[i * width * 3 + j * 3 + 2] = (unsigned char) (pb * (255 - (w >> 24)) / 255.0 + (w & 255) * (w >> 24) / 255.0 + 0.5);;
      }
    }
  }

  fclose(f);
  HoImage *img = new HoImage(width, height, (void *)d);
 // img = new HoImage(*img, 2);
  if (alfaObraz) {
    *alfaObraz = new HoImage(width, height, (void *)a);
  }
   if (oboji) {
    *oboji = new HoImage(width, height, (void *)x);
  }
  free(d);
  free(x);
  free(a);
  return img;
}

void push(HoPushButton *b, void *param) {
  HoImage *alfa;
  HoImage *oboji;
  HoImage *i = nactiObrazek(
    es->getText().toString(),
    ew->getText().toString(),
    et->getText().toString(),
    ep->getText().toString(),
    &oboji,
    &alfa);
  if (i) {
    (new OknoObrazku(i, "Obrazek"))->create();
    (new OknoObrazku(oboji, "Obrazek s alfou"))->create();
    (new OknoObrazku(alfa, "Alfa"))->create();
  }
}

int homain(int argc, char * argv[]){
  HoApp *a;

  a = new HoApp(argc, argv);
  hlavni = new MojeOkno(NULL, "Argbview");
  new HoLabel(hlavni, "Soubor");
  es = new HoEdit(hlavni,
   (argc > 1 ? argv[1] : "C:\\signpost_0.argb"), 64);
  new HoLabel(hlavni, HoString("©íøka", 0));
  ew = new HoEdit(hlavni, "512", 30);
  new HoLabel(hlavni, "Formát");
  et = new HoEdit(hlavni, "1555", 30);
  new HoLabel(hlavni, "Barva pozadi (RGB)");
  ep = new HoEdit(hlavni, "FFFFFF", 30);
  new HoPushButton(hlavni, "Naèti", push, hlavni);
  hlavni->pack();
  hlavni->create();
  a->run();
  delete a;
  return 0;
}

