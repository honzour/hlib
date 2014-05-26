/*********************************************************/
/* cotyvar.h - definice zakladnich typu a konstant       */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef cotyvarH
#define cotyvarH
#include <time.h>
#include "volby.h"
/******************************************/
/*       Definice ciselnych typu          */
/******************************************/
#define s8 signed char
#define u8 unsigned char
#define s16 signed short
#define u16 unsigned short
#define s32 signed int
#define u32 unsigned int
/*#define bool int
#define true 1
#define false 0*/
#define KontrolaTypu  if (sizeof(s8) != 1 || sizeof(u8) != 1||\
     sizeof(s16) != 2 || sizeof(u16) != 2||\
     sizeof(s32) != 4 || sizeof(u32) != 4)\
  Chyba("Chyba: spatne nastavene ciselne typy");

typedef s8 TSch[12*10];
 /* Klasicka sachovnice 12*10 */
typedef struct {
 u8 roch;  /* binarne 00...00vmVM */
	   /* V,v - moznost velke rosady bileho a cerneho
	      M,m - totez s malou  */
 u8 mimoch;  /* Pole, na nemz stoji pesec tahnuvsi v predchozim tahu o 2,
	     nebo 0 pokud se minule netahlo pescem o 2 */
 int bily;    /* Je bily na tahu ?*/
 TSch sch;
} TPozice;
#define MaxHloubkaPropoctu 8
/* Horni odhad delky vetve stromu propoctu - kvuli velikosti
   datovych struktur */
#define TahuZPozice 64
/* Tohle neni zadny horni odhad. Jen kvuli velikosti zasobniku
 tahu.*/
#define HloubkaZasobnikuTahu (TahuZPozice*MaxHloubkaPropoctu)
/* Kolik tahu na zasobniku */
typedef u16 TTah1;
/* Format TTah1::data:   def.: Nenormalni tah je
1) rosada
2) promena pesce
3) brani mimochodem
  def.: Normalni tah je kazdy tah, ktery neni nenormalni.
   Zbesily format TTah1.data je dusledkem me snahy o malou velikost TTah1
   a efektivni rutiny zejmena pro normalni tahy. (Nebot se s timto typem
   pocita v rekurzi a normalni tahy jsou skoro vsechny)
   binarne  1234567812345678
	        nmoooooookkkkkkk (normalni tah a brani mimochodem)
	        nmrcv00000000000 (rosada)
	        nmrcppoookkk0000 (promena pesce)
n - Je to nenormalni tah ?
  Pokud ne (tj. tah je normalni), je m=0 (MUSI byt 0) a ooooooo a kkkkkkk
   ma vyznam poli odkud a kam se tahne.
  Pokud ano (tj. tah je nenormalni):
   m - Je to rosada nebo promena pesce ?
    Pokud ne (tj. je to brani mimochodem) ma ooooooo a kkkkkkk tak‚ vyse
     uvedeny vyznam.
   Pokud ano:
    r - Je to rosada ?
     Pokud ano, potom
      c: je to rosada cerneho ? v: je to velka rosada ?
     Pokud ne (tj. je to promena pesce)
      c: je to promena cerneho pesce ?
      pp: v co se pesec meni 0=kun, 1=strelec, 2=vez, 3=dama
      ooo: cislo, ktere se musi pricist k A7 (je-li to promena bileho pesce)
	   nebo A2 (je-li to promena cerneho pesce), abychom dostali pole,
	   odkud pesec tahne
      kkk: totiz s polem promeny, jen misto A7 (resp. A2) se bere  A8
       (resp A1)

Presne v duchu predchozi definice si napisu konstanty pro rosady */
#define MBRoch (7<<13)
#define VBRoch ((7<<13)|(1<<11))
#define MCRoch (15<<12)
#define VCRoch (31<<11)
typedef struct {
u16 t;  /*data tahu*/
u8 roch;/*stav rosad pred zahranim - viz TPozice*/
u8 mimoch;/*stav mimochodem pred zahranim - viz TPozice*/
s8 brani;/*ktera figurka je tahem sebrana*/
u8 zmena;/*bylo tazeno pescem nebo bylo brani*/
u32 hashF;/*kvuli detekci opakovani*/
} TPartData;
typedef struct tpartie{
 struct tpartie *l;
 struct tpartie *p;
 TPartData data;} TPartie;
 /*Partie je obousmerny spojak tahu. Prvni uzel je nevyuzity
 - to abych mohl byt ve stavu pred zahranim tahu.*/
typedef struct{
 int VNT; /*Jsem v metodì nulového tahu ? Kolikrat vnorene ?*/
 int NulTah; /* Smim pouzit metodu nuloveho tahu. (Napr.
 v koncovce (nebo pri ladeni :-)) ne. */
 time_t sKonceMysleni; /*Kdy mam koncit*/
 int msKonceMysleni;
 volatile int MamKoncitMysleni;
 /*Je-li true, prestane se pocitat a vylezu z rekurze*/
} TStavPropoctu;

typedef struct {
 TTah1 tahy[HloubkaZasobnikuTahu]; /*Tahy */
 s16 hranice[MaxHloubkaPropoctu]; /* Pocatky skupin tahu
 hranice[0] by mela byt vzdy 0 */
 s16 pos; /*Prvni volna skupina (je-li 0, je zasobnik prazdny),
 ukazuje take hloubku zanoreni aktualni pozice ve stromu propoctu*/
 s8 mimoch[MaxHloubkaPropoctu];
/*Pole, na nemz stoji pesec tahnuvsi v predchozim tahu o 2,
  nebo 0 pokud se minule netahlo pescem o 2 */
 u8 roch[MaxHloubkaPropoctu];
 /*Stavy-moznosti rochovat*/
 s8 brani[MaxHloubkaPropoctu];
} TZasobnikTahu;
/* Je-li tedy zasobnik vynulovany byte po bytu, je "spravne prazdny" */
typedef struct{
	u32 kod; /* hash funkce pozice */
	s16 cena; /* cena pozice z hrace na tahu... */
	u8 hloubka; /* ....pri propoctu do hloubky hloubka */
	u8 priznak; /* xxxxxxHD
	 D - pozice je stejná nebo lepší než cena
	 H - pozice je stejná nebo horší než cena 
	(=> H&D - pozice má pøesnì danou hodnotu) */
} THashPrvek;
typedef u8 TVazby[8];
/*napr. NalezBileVazby spravne vyplni, obshuje policka.
Neni-li jich vsech 8, je za posledni 0*/
typedef struct {
/* Na kterem sloupci jsou bili/cerni pesci*/
 u8 BPSloupec[8];
 u8 CPSloupec[8];
/* u8 BVSloupec[8];
 u8 CVSloupec[8];*/
 u8 b[5]; /*Pocet figur bileho - pesec..dama*/
 u8 c[5]; /*Totez pro cerneho*/
 u8 bk, ck; /*Kde jsou kralove*/
} TMaterial;
typedef struct {
 TPozice pozice; /*V ni probiha propocet*/
 TZasobnikTahu zasobnik;/*Zasobnik pro rekurzivni propocet*/
 TMaterial material;/*Inkrementalne pocitane veci, hlavne material*/
 TPartie *prt;
 /*partie*/
 int KonecPartie;
 /*je partie ukoncena*/
 THashPrvek *ht;
 /*hlavni hash tabulka*/
 TVazby Vazby;
 /*kde jsou jake vazby (kvuli spec. pripadum v generatoru)*/
 } TUloha;
/* Je treba si pripravit reentrantnost. Mozna nekdy v budoucnu
   bude vysledek dll (pro wokna nebo unix),
   takze vsechny promenne, ktere by jinak byly globalni, si zabalim do
   jedne strukturky (pak budu mit seznam tehto struktur) */
#define mat 30000
/*Konstanta pro hodnotu pozice*/
extern const TPozice ZakladniPostaveni;
/*inicializacni konstantni promenna*/
#ifdef Vypisy
extern int ABVH;
/*hloubka pripadnych ladicich vypisu*/
#endif
#define MAX(X,Y)((X)>(Y)?(X):(Y))
#define MIN(X,Y)((X)<(Y)?(X):(Y))
/*obecne uzitecna makra*/
#endif
