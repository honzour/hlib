/*********************************************************/
/* sachy.h - obsahuje main a zakladni init a done        */
/* 31.12. 2000 Jan Nemec                                 */
/*********************************************************/
#ifndef sachyH
#define sachyH
#include "volby.h"
#include "cotyvar.h"
cfunkce TUloha *InitUlohu();
/*********************************************/
/* Inicializace ulohy (Alokace tabulek atd)  */
/*********************************************/
cfunkce void DoneUlohu(TUloha **uloha);
/**********************************/
/* mazani ulohy a jejich tabulek  */
/**********************************/
#endif
