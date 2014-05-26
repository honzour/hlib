#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "howindow.h"
#include "hopushbutton.h"
#include "hoedit.h"
#include "hoapp.h"
#include "hodc.h"
#include "hostring.h"

typedef struct {
  double *pd;
  int dnu;
  double max;
  double min;
} TVyvoj;

static double filtr(double *pd, int n, double q = 0.95) {
  double a = 1 / ((pow(q , n) - 1) / (q - 1));
  double suma = 0;
  for (int i = 0; i < n; i++) {
   // printf("a = %.2f ", a);
    suma += *pd * a;
    a *= q;
    pd--;
  }
  return suma;
}

class MojeOkno: public HoWindow {
  public:
    TVyvoj *pv;
    MojeOkno(HoWindow *parent, int x, int y, const HoString &title,
      TVyvoj *tv):

      HoWindow(parent, title,
        x, y,
        640, 480,
        HoWindow::styleCaption |
        HoWindow::styleBorder |
        HoWindow::styleSizing
      ){
        pv = tv;
      };
    virtual void paint(HoDC *dc);
    virtual void keyDown(wchar_t c, int code, int mods);
    virtual void mouseDown(int x, int y, int button);
};

void MojeOkno::mouseDown(int x, int y, int button) {
}

void MojeOkno::keyDown(wchar_t c, int code, int mods) {
}

void MojeOkno::paint(HoDC *dc){
  int i;
  int rw = w - dx1 - dx2 - 1;
  int rh = h - dy1 - dy2 - 1;

  if (pv->max == pv->min) return;
  if (!pv->dnu) return;
  for (i = 0; i < pv->dnu - 1; i++) {
    dc->drawLine(
    (int)(((i * rw) / (double) (pv->dnu - 1)) + 0.5),
    (int)(rh * (pv->max - pv->pd[i]) / (double) (pv->max - pv->min)),
    (int)((((i + 1) * rw) / (double) (pv->dnu - 1)) + 0.5),
    (int)(rh * (pv->max - pv->pd[i + 1]) / (double) (pv->max - pv->min))
    );
  }
  dc->setForeground(0xFFFF, 0, 0);
  for (i = 0; i < pv->dnu - 1; i++)
    dc->drawLine(
    (int)(((i * rw) / (double) (pv->dnu - 1)) + 0.5),
    (int)(rh * (pv->max - filtr(pv->pd + i, i)) / (double) (pv->max - pv->min)),
    (int)((((i + 1) * rw) / (double) (pv->dnu - 1)) + 0.5),
    (int)(rh * (pv->max - filtr(pv->pd + i + 1, i + 1)) / (double) (pv->max - pv->min))
    );
}

HoWindow **okna;
HoWindow *hlavni;
TVyvoj *tv;

void nactiGraf(int i, char *arg) {
  FILE *f;

//  FILE *fw = fopen("export.txt", "w");

  f = fopen(arg, "r");
  if (!f) {
    fputs("Nelze otevøít\n", stderr);
    exit(1);
  }
  
  int j;
  char str[256]; 
  int radku = -1;
  int radek = 0;

  while (fgets(str, 256, f)) {
    radku++;
  }
    
  fclose(f);
  if (radku < 1) {
    fputs("Chyba souboru\n", stderr);
    exit(-1);
  }
  
  tv[i - 1].dnu = radku;
  tv[i - 1].pd = (double *) malloc(sizeof(double) * radku);


    
  f = fopen(arg, "r");
  fgets(str, 256, f);
    
  while (fgets(str, 256, f)) {
    int delka, p;
    delka = strlen(str);
    for (j = 0; j < delka; j++) {
      if (str[j] == '%') {
        str[j] = 0;
        delka = j;
        break;
      }
      if (str[j] == ',') {
        str[j] = '.';
        continue;
      }
    }
      
    p = delka - 1;
    while (p > 0 && str[p] > 32) p--;

    int index = radku - radek - 1;
    tv[i - 1].pd[index] = atof(str + p + 1);
  
//    fprintf(fw, ",,,,,,%f\n", (tv[i - 1].pd[index] + 100)/100.0);

    if (!radek) {
      tv[i - 1].min = tv[i - 1].max = tv[i - 1].pd[index];
    }
      
    else {
      if (tv[i - 1].max < tv[i - 1].pd[index])
        tv[i - 1].max = tv[i - 1].pd[index];
      else
        if (tv[i - 1].min > tv[i - 1].pd[index])
          tv[i - 1].min = tv[i - 1].pd[index]; 
    }
    radek++;
  }
  fclose(f);
  //fclose(fw);

  {
    int dnu = tv[i - 1].dnu;
    double vykon = tv[i - 1].pd[dnu - 1];
      
    char s[256];
    sprintf(
      s,
      "%s: %.2f%% za %i dnu = %.2f p. a. <%.2f, %.2f>",
      arg,
      vykon,
      dnu,
      (exp(365.0 / dnu * log(vykon / 100.0 + 1)) - 1) * 100,
      tv[i - 1].min,
      tv[i - 1].max
    );
    okna[i-1] = new MojeOkno(hlavni, i * 10, i * 10, s, tv + (i - 1));
    okna[i-1]->create();
  }
}

int homain(int argc,char * argv[]){
  HoApp *a;
  int i;
 
  if (argc < 1) return -1;

  a = new HoApp(argc, argv);
  tv = (TVyvoj *) malloc(sizeof(TVyvoj) * (argc - 1));
  okna = (HoWindow **) malloc(sizeof(HoWindow *) * (argc - 1));

  hlavni = new HoWindow(NULL, "Hlavní okno", 50, 50, 700, 520, HoWindow::styleSizing);
  hlavni->create();
//MessageBox(NULL, "ziju1", "ziju1", MB_OK);  
  for (i = 1; i < argc; i++) {
  //  MessageBox(NULL, argv[i], argv[i], MB_OK);
    nactiGraf(i, argv[i]);
  }
//MessageBox(NULL, "ziju", "ziju", MB_OK);  
  a->run();
  delete a;
  for (i = 1; i < argc; i++) {
    free(tv[i - 1].pd);
  }
  free(tv);
  return 0;
}

