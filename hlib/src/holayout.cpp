#include <stdlib.h>
#include "holayout.h"
#include "howindow.h"

static int cmpVWindow(void *l,void *r){
  if (((HoWindow *)l)->y < ((HoWindow *)r)->y) return -1;
  if (((HoWindow *)l)->y > ((HoWindow *)r)->y) return 1;
  if (l < r) return -1;
  if (l > r) return 1;
  return 0;
}

static int cmpHWindow(void *l, void *r){
  if (((HoWindow *)l)->x < ((HoWindow *)r)->x) return -1;
  if (((HoWindow *)l)->x > ((HoWindow *)r)->x) return 1;
  if (l<r) return -1;
  if (l>r) return 1;
  return 0;
}

HoBoxLayout::HoBoxLayout(HoWindow *w, int d)
 :HoLayout(w)
{
  this->d = d;
  n = d;     // "y" souøadnice 1. prvku
  maxm = 0;  // "x" souøadnice nejtlustšího prvku
  cnt = 0;
  cntAll = 0;
}



HoVBoxLayout::HoVBoxLayout(HoWindow *w, int d)
 :HoBoxLayout(w,d)
{ 
  t = new HoTree(cmpVWindow, 0, 0);
}

HoHBoxLayout::HoHBoxLayout(HoWindow *w, int d)
 :HoBoxLayout(w,d)
{ 
  t = new HoTree(cmpHWindow,0,0);
}

void HoBoxLayout::add(HoWindow *w){

  cnt++;
  cntAll++;
  w->x = d;
  w->y = n;
  if (w->w < 0) w->w = 100;
  if (w->h < 0) w->h = 100;
  t->ins(w, NULL);
}

void HoBoxLayout::remove(HoWindow *w){
 cnt--;
 t->del(w);
}

void HoVBoxLayout::add(HoWindow *w){

 HoBoxLayout::add(w);
 n += w->getH();
}

void HoHBoxLayout::add(HoWindow *w){
  cnt++;
  cntAll++;
  w->x = n;
  w->y = d;
  if (w->w < 0) w->w = 100;
  if (w->h < 0) w->h = 100;
  t->ins(w, NULL);
  n += w->getW();
}

/**
Struktura pro metodu pack u obdélníkových layoutù.
*/
typedef struct {
  /**
   Maximální velikost prvku v nemìnné souøadnici
  */
  int max;
  /**
   Totez, co d u layoutu, odstup prvku od sebe a od okraju.
  */
  int d1;
  /**
   Aktualni velikost v promenne souradnici.
  */
  int d2;
} PackData;


static int packVCallback(void * key, void *, void * param) {
  HoWindow *w=(HoWindow *)key;
  PackData *d=(PackData *)param;
 
  w->pack();
  w->x = d->d1;
  w->y = d->d2;
  d->d2 += ((w->h = w->getH()) + d->d1);
  w->w = w->getW();
  if (d->max < w->w) d->max = w->w;
  w->move(w->x, w->y, w->w, w->h);
//  printf("w->move(%i, %i, %i, %i)\n", w->x, w->y, w->w, w->h);
  return 1;
}

void HoVBoxLayout::pack() {
  PackData d;
 
  d.max = 0;
  d.d1 = this->d;
  d.d2 = this->d;
  t->inOrder((void *)&d, packVCallback);
  maxm = d.max;
  n = d.d2;
  /* Domyslet atd. Velikosti okna atd.*/
  w->w = maxm + 2 * this->d + w->dx1 + w->dx2;
  w->h = n + w->dy1 + w->dy2;
}

static int packHCallback(void * key, void *, void * param){
  HoWindow *w=(HoWindow *)key;
  PackData *d=(PackData *)param;
 
  w->pack();
  w->x = d->d2;
  w->y = d->d1;
  d->d2 += ((w->w = w->getW()) + d->d1);
  w->h = w->getH();
  if (d->max < w->h) d->max = w->h;
  w->move(w->x, w->y, w->w, w->h);
  return 1;
}

void HoHBoxLayout::pack(){
  PackData d;
 
  d.max = 0;
  d.d1 = this->d;
  d.d2 = this->d;
  t->inOrder((void *)&d, packHCallback);
  maxm = d.max;
  n = d.d2;
  /* Domyslet atd. Velikosti okna atd.*/
  w->w = n + w->dx1 + w->dx2;
  w->h = maxm + 2 * this->d + w->dy1 + w->dy2;
}

HoBoxLayout::~HoBoxLayout(){
 delete t;
}

