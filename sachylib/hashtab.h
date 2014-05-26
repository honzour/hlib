/**********************************************************/
/* hashtab.h - hash funkce pro tabulku ohodnocenych pozic */
/* 31.12. 2000 Jan Nemec                                  */
/**********************************************************/
#ifndef hashtabH
#define hashtabH
#include "volby.h"
#include "cotyvar.h"

typedef struct{
 u32 f[13][99];
/* [od cerneho krale pres prazdne pole az po bileho krale][pole]*/
 u32 bily;
 u32 roch[16];
 u32 mimoch[69];
} THashNahoda;
/* THashNahoda - obsahuje nahodna cisla pro vsechny dvojice
(typ kamene, pole), dale pro stav mimochodem, rosad a kdo
je na tahu*/

extern THashNahoda hnF;
extern THashNahoda hnG; /* Dve sady nahodnych cisel pro
dve funkce */
cfunkce void HashInit(void);
/*********************************************************/
/* HashInit naplni koeficienty obou hashovacich funkci   */
/* pseudonahodnymi cisly                                 */
/* Vola se JEDNOU na zacatku celeho PROGRAMU             */
/*********************************************************/
cfunkce u32 HashF(TPozice *pos);
cfunkce u32 HashG(TPozice *pos);
/**********************************************************************/
/* HashF a HashG												      */
/* Hashovaci funkce, vrati hash kod pozice. Nevola se v rekursivnim   */
/* propoctu, ale jen jednou na zacatku propoctu. V rekursi se pocita  */
/* "inkementalne" postupnym xorovanim tahu.                           */
/* F urcuje pozici v tabulce, G cislo pro kontrolu                    */
/**********************************************************************/
#endif
