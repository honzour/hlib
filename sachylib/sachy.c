/*********************************************************/
/* sachy.c - obsahuje main a zakladni init a done        */
/* 10.3. 2001 Jan Nemec                                   */
/*********************************************************/
#include "volby.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cotyvar.h"
#include "partie.h"
TUloha *InitUlohu()
/*********************************************/
/* Inicializace ulohy (Alokace tabulek atd)  */
/*********************************************/
{TUloha *uloha;

 uloha=(TUloha *)malloc(sizeof(TUloha));
 (void) memset((void *) uloha, 0, sizeof(TUloha));
 uloha->prt=NULL;
 uloha->pozice=ZakladniPostaveni;
 InitPartie(&(uloha->prt),&(uloha->pozice));
 uloha->KonecPartie=0;
 return uloha;
}
void DoneUlohu(TUloha **uloha)
/**********************************/
/* mazani ulohy a jejich tabulek  */
/**********************************/
 {DonePartie(&((*uloha)->prt));
 free((void *)*uloha);
 *uloha=NULL;
}

