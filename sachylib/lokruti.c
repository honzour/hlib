/*********************************************************/
/* lokruti.c - Test sachu ohrozeni a lokalni             */
/* (tj. nikoliv v partii) tahy                           */
/* 9.1. 2001 Jan Nemec                                   */
/*********************************************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "volby.h"
#include "cotyvar.h"
#include "generato.h"
#include "hashtab.h"
#include "lokruti.h"
#include "pole.h"
/***************************************************/
/**  Ohrozeno   Je dane policko ohrozene ?        **/
/***************************************************/
int Ohrozeno(s8 *p, int bilym) /*bilym - ohrozuje to pole bily*/
/* p je pointer do sachovnice na zkouman‚ policko*/
 {int j,k;

 if (bilym)
 {
 if (*(p-9)==1) return 1;/*pescem*/
 if (*(p-11)==1) return 1;/*pescem*/
 for(j=8;j<=15;j++)if (*(p+ofsety[j])==2) return 1;/*konem*/
 for(j=0;j<=7;j++)if (*(p+ofsety[j])==6) return 1;/*kralem*/
 for(j=0;j<=3;j++) /*vezi nebo damou po rade/sloupci*/
 {k=ofsety[j];
 while(1){if((*(p+k)==4)||(*(p+k)==5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
  for(j=4;j<=7;j++) /*strelcem nebo damou po diagonale*/
 {k=ofsety[j];
 while(1){if((*(p+k)==3)||(*(p+k)==5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
 } else /*cernym*/
  {
 if (*(p+9)==-1) return 1;/*pescem*/
 if (*(p+11)==-1) return 1;/*pescem*/
 for(j=8;j<=15;j++)if (*(p+ofsety[j])==-2) return 1;/*konem*/
 for(j=0;j<=7;j++)if (*(p+ofsety[j])==-6) return 1;/*kralem*/
 for(j=0;j<=3;j++) /*vezi nebo damou po rade/sloupci*/
 {k=ofsety[j];
 while(1){if((*(p+k)==-4)||(*(p+k)==-5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
  for(j=4;j<=7;j++) /*strelcem nebo damou po diagonale*/
 {k=ofsety[j];
 while(1){if((*(p+k)==-3)||(*(p+k)==-5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
 }
 return 0;
 }

 static int OhrozenoNekralem(s8 *p, int bilym){
/*Jako Ohrozeno z lokruti.h, ale neuvazuje napadeni
kralem*/
 /*bilym - ohrozuje to pole bily*/
/* p je pointer do sachovnice na zkouman‚ policko*/
 {int j,k;

 if (bilym)
 {
 if (*(p-9)==1) return 1;/*pescem*/
 if (*(p-11)==1) return 1;/*pescem*/
 for(j=8;j<=15;j++)if (*(p+ofsety[j])==2) return 1;/*konem*/
 for(j=0;j<=3;j++) /*vezi nebo damou po rade/sloupci*/
 {k=ofsety[j];
 while(1){if((*(p+k)==4)||(*(p+k)==5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
  for(j=4;j<=7;j++) /*strelcem nebo damou po diagonale*/
 {k=ofsety[j];
 while(1){if((*(p+k)==3)||(*(p+k)==5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
 } else /*cernym*/
  {
 if (*(p+9)==-1) return 1;/*pescem*/
 if (*(p+11)==-1) return 1;/*pescem*/
 for(j=8;j<=15;j++)if (*(p+ofsety[j])==-2) return 1;/*konem*/
 for(j=0;j<=3;j++) /*vezi nebo damou po rade/sloupci*/
 {k=ofsety[j];
 while(1){if((*(p+k)==-4)||(*(p+k)==-5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
  for(j=4;j<=7;j++) /*strelcem nebo damou po diagonale*/
 {k=ofsety[j];
 while(1){if((*(p+k)==-3)||(*(p+k)==-5))return 1;if(*(p+k))break;k+=ofsety[j];}
 }
 }
 return 0;
 }
}
static int CistoMezi(s8 *p1, s8 *p2, int o){
/*Jsou same nuly mezi dvema policky na sachovnici ?
musi byt od sebe o nasobek o ?*/
 s8 *p;
 if(p1>p2){p=p1;p1=p2;p2=p;}
 for(p1+=o;p1<p2;p1+=o)if(*p1)return 0;
 return 1;
}

int HrozbaBilemuKraliDamou(TUloha *u){
/*vraci 1 kdyz existuje policko vedle krale
ktere je napadeno (nejakou) souperovou damou a jeste
necim jinym (to neco jineho muze byt i za tou potencialne
ohrozujici damou) a neni kryte nicim jinym nez kralem

- to je casty obrazek banalnich jednotahovych matu*/
int i,dx,dy,x,y,kx,ky;
int o,p;
div_t t;
s8 *pole;

  if(!u->material.c[4])return 0;
  t=div(u->material.bk-a1,10);
  kx=t.rem; ky=t.quot; /* souradnice krale*/
  for(i=a1;i<=h8;i++)if(u->pozice.sch[i]==-5){
   t=div(i-a1,10); dx=t.rem; dy=t.quot; /* souradnice damy*/
   pole=u->pozice.sch+u->material.bk-11;
   for(y=ky-1;y<ky+2;y++,pole+=7)
    for(x=kx-1;x<kx+2;x++,pole++)
    {
	 if(x==kx && y==ky ||
	 *pole<0||*pole>6) continue;
	 p=0;
	 if(x==dx){
	  if(CistoMezi(u->pozice.sch+i,pole,10))p=1;
	 } else
	 if(y==dy){
	  if(CistoMezi(u->pozice.sch+i,pole,1))p=1;
	 }
	 else
	 if(y-dy==x-dx){
	  if(CistoMezi(u->pozice.sch+i,pole,11))p=1;
	 }
	 else
	 if(y-dy==dx-x){
	  if(CistoMezi(u->pozice.sch+i,pole,9))p=1;
	 }
	 if(!p) continue; /*Dama to policko nenapada*/

   if(!OhrozenoNekralem(pole,1))
    {
    u->pozice.sch[i]=0;
	o=Ohrozeno(pole,0);
	u->pozice.sch[i]=-5;
    if(o)return 1;
   }
 }/* od for cyklu pres okoli krale*/
   if(u->material.c[4]==1) break; /*Kdyz ma jen jednu, nebudu hledat dalsi*/
  }
 return 0;
}

int HrozbaCernemuKraliDamou(TUloha *u){
/*vraci 1 kdyz existuje policko vedle krale
ktere je napadeno (nejakou) souperovou damou a jeste
necim jinym (to neco jineho muze byt i za tou potencialne
ohrozujici damou) a neni kryte nicim jinym nez kralem

- to je casty obrazek banalnich jednotahovych matu*/
int i,dx,dy,x,y,kx,ky;
int o,p;
div_t t;
s8 *pole;

  if(!u->material.b[4])return 0;
  t=div(u->material.ck-a1,10);
  kx=t.rem; ky=t.quot; /* souradnice krale*/
  for(i=a1;i<=h8;i++)if(u->pozice.sch[i]==5){
   t=div(i-a1,10); dx=t.rem; dy=t.quot; /* souradnice damy*/
   pole=u->pozice.sch+u->material.ck-11;
   for(y=ky-1;y<ky+2;y++,pole+=7)
    for(x=kx-1;x<kx+2;x++,pole++)
    {
	 if(x==kx && y==ky || *pole>0) continue;
	 p=0;
	 if(x==dx){
	  if(CistoMezi(u->pozice.sch+i,pole,10))p=1;
	 } else
	 if(y==dy){
	  if(CistoMezi(u->pozice.sch+i,pole,1))p=1;
	 }
	 else
	 if(y-dy==x-dx){
	  if(CistoMezi(u->pozice.sch+i,pole,11))p=1;
	 }
	 else
	 if(y-dy==dx-x){
	  if(CistoMezi(u->pozice.sch+i,pole,9))p=1;
	 }
	 if(!p) continue; /*Dama to policko nenapada*/

   if(!OhrozenoNekralem(pole,0))
    {
    u->pozice.sch[i]=0;
	o=Ohrozeno(pole,1);
	u->pozice.sch[i]=5;
    if(o)return 1;
   }
 }/* od for cyklu pres okoli krale*/
   if(u->material.b[4]==1) break; /*Kdyz ma jen jednu, nebudu hledat dalsi*/
  }
 return 0;
}


int Sach(int komu, s8 *sch)
/*************************************************************************/
/*   Sach je komu v sachu ?                                              */
/*   sch - pointr na 1. policko sachovnice 12*10 (tedy nikoliv a1)       */
/*************************************************************************/
{s8 kral;

 if (komu) kral=6; else kral=-6;
 for(sch+=a1; (*sch)!=kral; sch++);
 return Ohrozeno(sch, !komu);
}
void UlozStav(TUloha *uloha)
 {s16 hloubka;
  uloha->zasobnik.mimoch[hloubka=uloha->zasobnik.pos]=uloha->pozice.mimoch;
  uloha->zasobnik.roch[hloubka]=uloha->pozice.roch;
 }
 /**************************************************************************
 *   JednoTahni - Tahne v testu pripustnosti tahu                          *
 *   Na zasobnik ulozi jen udaje pro TahniZpet, nepocita cenu              *
 *   ani hash funkce												       *
 ***************************************************************************/
void JednoTahni(u16 tah, TUloha *uloha)
 { u8 odkud,kam;
   s8 co;
  s16 hloubka;

  hloubka=uloha->zasobnik.pos++;
  uloha->pozice.mimoch=0; /*Vetsina tahu neni pescem o 2, pokud ano, osetri se*/
  uloha->pozice.bily=!uloha->pozice.bily;
  if(!(tah>>15)) /* Normalni tah*/
   {kam=(u8)(tah&127);
    odkud=(u8)(tah>>7);
    if(/* bud cerny tahne pescem o 2*/
		odkud-kam==20 && uloha->pozice.sch[odkud]==-1
		/* a bily pesec ciha */
	&& (uloha->pozice.sch[kam+1]==1 || uloha->pozice.sch[kam-1]==1)
	/* nebo bily tahne pescem o 2 */
	|| odkud-kam==-20 && uloha->pozice.sch[odkud]==1
		/* a cerny pesec ciha */
	&& (uloha->pozice.sch[kam+1]==-1 || uloha->pozice.sch[kam-1]==-1))
		
      uloha->pozice.mimoch=kam;
 /* Niceni rosad
   Pozn.: nejde dat vsude 'else', protoze napr. Va1xa8 nici obe velke rosady*/
    if (odkud==e1) uloha->pozice.roch&=12; else /* 1100b*/
    if (odkud==e8) uloha->pozice.roch&=3; else /* 0011b*/
  { if (kam==a1 || odkud==a1) uloha->pozice.roch&=13;/*1101b*/
    if (kam==h1 || odkud==h1) uloha->pozice.roch&=14;/*1110b*/
    if (kam==a8 || odkud==a8) uloha->pozice.roch&=7; /*0111b*/
    if (kam==h8 || odkud==h8) uloha->pozice.roch&=11;/*1011b*/
  }
/* Ulozim si sebranou figuru*/
 uloha->zasobnik.brani[hloubka]=uloha->pozice.sch[kam];
 BeznyTah:; /* sem se skace z konce brani mimochodem*/
 /* zakladni rutina normalniho tahu:*/
 uloha->pozice.sch[kam]=uloha->pozice.sch[odkud];
 uloha->pozice.sch[odkud]=0;
Konec:;
 return;
 }
  /* Nenormalni tah

     Mala bila rosada*/
  if (tah==MBRoch)
   {uloha->pozice.sch[e1]=0;
    uloha->pozice.sch[g1]=6;
    uloha->pozice.sch[h1]=0;
    uloha->pozice.sch[f1]=4;
    uloha->pozice.roch&=12;
    uloha->zasobnik.brani[hloubka]=0;
    goto Konec;}
  /*Velka bila rosada*/
  if (tah==VBRoch)
   {uloha->pozice.sch[e1]=0;
    uloha->pozice.sch[c1]=6;
    uloha->pozice.sch[a1]=0;
    uloha->pozice.sch[d1]=4;
    uloha->pozice.roch&=12;
    uloha->zasobnik.brani[hloubka]=0;
    goto Konec;}
 /*Mala cerna rosada*/
  if (tah==MCRoch)
   {uloha->pozice.sch[e8]=0;
    uloha->pozice.sch[g8]=-6;
    uloha->pozice.sch[h8]=0;
    uloha->pozice.sch[f8]=-4;
    uloha->pozice.roch&=3;
    uloha->zasobnik.brani[hloubka]=0;
    goto Konec;}
  /*Velka cerna rosada*/
  if (tah==VCRoch)
   {uloha->pozice.sch[e8]=0;
    uloha->pozice.sch[c8]=-6;
    uloha->pozice.sch[a8]=0;
    uloha->pozice.sch[d8]=-4;
    uloha->pozice.roch&=3;
    uloha->zasobnik.brani[hloubka]=0;
    goto Konec;}
  /*Promena bileho pesce*/
 if ((tah>>12)==12)
  {odkud=(u8)(a7+((tah>>7)&7));
   kam=(u8)(a8+((tah>>4)&7));
   co=(s8)(2+((tah>>10)&3));
   /* Ulozim si, co jsem sebral */
   uloha->zasobnik.brani[hloubka]=uloha->pozice.sch[kam];
   uloha->pozice.sch[odkud]=0;
   uloha->pozice.sch[kam]=co;
   if (kam==a8) /* meni pesce na a8, mohl sezrat vez => rosady...*/
    uloha->pozice.roch&=7; /*0111b*/
   if (kam==h8) /* meni pesce na h8, mohl sezrat vez => rosady...*/
    uloha->pozice.roch&=11; /*1011b */
   goto Konec;
  }
  /*Promena cerneho pesce*/
 if ((tah>>12)==13)
  {
  odkud=(u8)(a2+((tah>>7)&7));
   kam=(u8)(a1+((tah>>4)&7));
   co=(s8)(-(2+((tah>>10)&3))); 
	
   /* Ulozim si, co jsem sebral */
   uloha->zasobnik.brani[hloubka]=uloha->pozice.sch[kam];
   uloha->pozice.sch[odkud]=0;
   uloha->pozice.sch[kam]=co;
   if (kam==a1) /* meni pesce na a1, mohl sezrat vez => rosady...*/
    uloha->pozice.roch&=13; /*1101b*/
   if (kam==h1) /* meni pesce na h1, mohl sezrat vez => rosady...*/
    uloha->pozice.roch&=14; /*1110b*/
    goto Konec;
  }
 /* Brani mimochodem (nic jineho to uz byt nemuze)*/
 tah&=0x3fff; /* odstraneni prvnich dvou bitu, aby se lepe siftovalo*/
 kam=(u8)(tah&127);
 odkud=(u8)(tah>>7);
 if(odkud<kam)
  {uloha->pozice.sch[kam-10]=0;
   uloha->zasobnik.brani[hloubka]=-1;
 }
   /* to hral bily*/
 else
  {uloha->pozice.sch[kam+10]=0;
   uloha->zasobnik.brani[hloubka]=1;
 }
   /* cerny*/
 goto BeznyTah; /* zbytek je stejny jako pro normalni tah*/
 }

void JednoTahniZpet(u16 tah, TUloha *uloha)
 {u8 odkud, kam, hloubka;

  hloubka=(u8)--uloha->zasobnik.pos;
  uloha->pozice.mimoch=uloha->zasobnik.mimoch[hloubka];
  uloha->pozice.roch=uloha->zasobnik.roch[hloubka];
  uloha->pozice.bily=!uloha->pozice.bily;
  if(!(tah>>15)) /* Normalni tah*/
   {kam=(u8)(tah&127);
    odkud=(u8)(tah>>7);
 uloha->pozice.sch[odkud]=uloha->pozice.sch[kam];
 uloha->pozice.sch[kam]=uloha->zasobnik.brani[hloubka];

 goto konec;
 }
  /* Nenormalni tah

     Mala bila rosada*/
  if (tah==MBRoch)
   {uloha->pozice.sch[e1]=6;
    uloha->pozice.sch[g1]=0;
    uloha->pozice.sch[h1]=4;
    uloha->pozice.sch[f1]=0;
    goto konec;}
  /*Velka bila rosada*/
  if (tah==VBRoch)
   {uloha->pozice.sch[e1]=6;
    uloha->pozice.sch[c1]=0;
    uloha->pozice.sch[a1]=4;
    uloha->pozice.sch[d1]=0;
    goto konec;}
 /*Mala cerna rosada*/
  if (tah==MCRoch)
   {uloha->pozice.sch[e8]=-6;
    uloha->pozice.sch[g8]=0;
    uloha->pozice.sch[h8]=-4;
    uloha->pozice.sch[f8]=0;
    goto konec;}
  /*Velka cerna rosada*/
  if (tah==VCRoch)
   {uloha->pozice.sch[e8]=-6;
    uloha->pozice.sch[c8]=0;
    uloha->pozice.sch[a8]=-4;
    uloha->pozice.sch[d8]=0;
    goto konec;}
  /*Promena bileho pesce*/
 if ((tah>>12)==12)
  {
   uloha->pozice.sch[a7+((tah>>7)&7)]=1;
   uloha->pozice.sch[a8+((tah>>4)&7)]=uloha->zasobnik.brani[hloubka];
   goto konec;
  }
  /*Promena cerneho pesce*/
 if ((tah>>12)==13)
  {
   uloha->pozice.sch[a2+((tah>>7)&7)]=-1;
   uloha->pozice.sch[a1+((tah>>4)&7)]=uloha->zasobnik.brani[hloubka];
   goto konec;
  }
 /* Brani mimochodem (nic jineho to uz byt nemuze)*/
 tah&=0x3fff; /* odstraneni prvnich dvou bitu, aby se lepe siftovalo*/
 kam=(u8)(tah&127);
 odkud=(u8)(tah>>7);
 if(odkud<kam)
  {uloha->pozice.sch[kam-10]=-1; /* to hraje bily*/
   uloha->pozice.sch[odkud]=1;}
 else
 {uloha->pozice.sch[kam+10]=1;  /* cerny */
  uloha->pozice.sch[odkud]=-1;}
 uloha->pozice.sch[kam]=0;
konec:;
 }
