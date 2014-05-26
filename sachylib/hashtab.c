/**********************************************************/
/* hashtab.c - hash funkce pro tabulku ohodnocenych pozic */
/* 31.12. 2000 Jan Nemec                                  */
/**********************************************************/
#include "volby.h"
#include <time.h>
#include <stdlib.h>
#include "cotyvar.h"
#include "hashtab.h"
#include "pole.h"

THashNahoda hnF, hnG;
/* Dve sady nahodnych cisel pro dve funkce */
u32 MojeRand(void){
/*************************************************************/
/* rand() nepokryva cele u32, vraci totiz int. MojeRand ano. */
/*************************************************************/
	return ((rand()&255)<<24)|((rand()&255)<<16)|((rand()&255)<<8)|(rand()&255);
}
void HashInit(void){
/*********************************************************/
/* HashInit naplni koeficienty obou hashovacich funkci   */
/* pseudonahodnymi cisly                                 */
/* Vola se JEDNOU na zacatku celeho PROGRAMU             */
/*********************************************************/
int i,k;

 for(i=0;i<=12;i++)
  for(k=a1;k<=h8;k++)
  {hnF.f[i][k]=MojeRand();
   hnG.f[i][k]=MojeRand();
  }
  for(i=0;i<=h5;i++){
	  hnF.mimoch[i]=MojeRand();
	  hnG.mimoch[i]=MojeRand();}
hnF.bily=MojeRand();
hnG.bily=MojeRand();
for(i=0;i<=15;i++){
	hnF.roch[i]=MojeRand();
	hnG.roch[i]=MojeRand();
}
}


u32 hash(TPozice *pos, THashNahoda *hn){
/**********************************************************/
/* Vypocet hash funkce dane nahodnymi cisly hn postupnym  */
/* xorovanim faktoru									  */
/**********************************************************/

	int i,j;
	u32 navrat;

	navrat=0;
	for(i=0;i<=7;i++)
		for(j=0;j<=7;j++)
			navrat^=hn->f[pos->sch[a1+j+i*10]+6][a1+j+i*10];
	navrat^=hn->mimoch[pos->mimoch];
	navrat^=hn->roch[pos->roch];
	if(pos->bily)navrat^=hn->bily;
	return navrat;
}
u32 HashF(TPozice *pos){
	return hash(pos,&hnF);
}
u32 HashG(TPozice *pos){
	return hash(pos,&hnG);
}
/**********************************************************************/
/* HashF a HashG												      */
/* Hashovaci funkce, vrati hash kod pozice. Nevola se v rekursivnim   */
/* propoctu, ale jen jednou na zacatku propoctu. V rekursi se pocita  */
/* "inkementalne" postupnym xorovanim tahu.                           */
/* F urcuje pozici v tabulce, G cislo pro kontrolu                    */
/**********************************************************************/


