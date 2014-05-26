#ifndef rutinyH
#define rutinyH
#include <hosocket.h>
#include "zprava.h"
#include "fronta.h"
#include "struktury.h"
void Vykosti(HoSocket *socket, bool lock);
/**
  Roze�le 1 nebo 2 zpr�vy hr���m. Semafor klient� neskl�p�, mus� u� b�t dole.
  Pokud je to zpr�va stolku, skl�p� semafor klient�. Zpr�vy pos�l� rovnou,
  pokud byloZiju, jinak do �ekac� fronty.
  @param zprava1 1. zpr�va se po�le v�dy
  @param zprava2 2. zpr�va, je-li NULL, nepo�le se
  @param stolek je-li 0, v�em hr���m, jinak jen hr���m u stolku
  @param kromeSoket tomu se nepos�l�, pokud NULL, ��dn� omezen�
*/

void Rozesli2ZpravyHracum(Zprava *zprava1, Zprava *zprava2, int stolek,
  HoSocket *kromeSoket, bool zamykatStolky = true);
/**
  Roze�le 1 zpr�vu hr���m. Semafor klient� neskl�p�, mus� u� b�t dole.
  Pokud je to zpr�va stolku, skl�p� semafor klient�. Zpr�vy pos�l� rovnou,
  pokud byloZiju, jinak do �ekac� fronty.
  @param zprava1 1. zpr�va se po�le v�dy
  @param stolek je-li 0, v�em hr���m, jinak jen hr���m u stolku
  @param kromeSoket tomu se nepos�l�, pokud NULL, ��dn� omezen�
*/
void RozesliZpravuHracum(Zprava *zprava, int stolek, HoSocket *kromeSoket,
  bool zamykatStolky = true);
/**
Ode�le okam�it� (bez �ek�n� na �iju) zpr�vu ke hr��i tj push do fronty. 
@param zprava co se pos�l�
@param fronta fronta, kam se pos�l�
*/
void OdesliZpravuKeHraci(Zprava *zprava, Fronta<Zprava *> *fronta);
/**
Ode�le okam�it� (bez �ek�n� na �iju) zpr�vu ke hr��i.
@param zprava co se pos�l�
@param id id hr��e. Semafor hr��� neskl�p� (mus� u� b�t sklopen�).
*/

void OdesliZpravuKeHraciID(Zprava *zprava, int id);
/**
Po�le zpr�vu bez parametru hr��i t�m, �e ji vlo�� do fronty, z n� se u�
rovnou pos�l� a ne�ek� se na �iju.
@param socket komu se pos�l� (ne �e by se tam rovnou psalo, socket je jen
       identifik�tor hr��e)
@param zprava typ zpr�vy
@param posledni m� se u� rovnou poslat nebo hned p�id�me dal��?
*/
void PosliJednoduchouZpravu(HoSocket *socket, int zprava, char posledni);
/**
Po�le zpr�vu s int parametrem hr��i t�m, �e ji vlo�� do fronty, z n� se u�
rovnou pos�l� a ne�ek� se na �iju.
@param socket komu se pos�l� (ne �e by se tam rovnou psalo, socket je jen
       identifik�tor hr��e)
@param zprava typ zpr�vy
@param posledni m� se u� rovnou poslat nebo hned p�id�me dal��?
*/
void PosliIntZpravu(HoSocket *socket, int zprava, int param, char posledni);
/**
Po�le zpr�vu ZPRAVA_OK hr��i t�m, �e ji vlo�� do fronty, z n� se u�
rovnou pos�l� a ne�ek� se na �iju.
@param socket komu se pos�l� (ne �e by se tam rovnou psalo, socket je jen
       identifik�tor hr��e)
@param posledni m� se u� rovnou poslat nebo hned p�id�me dal��?
*/

void PosliOK(HoSocket *socket, char posledni);
/**
Roze�le v�em logout hr��e. Nic neskl�p�, semafor klient� mus� b�t dole.
Pokud byloZiju pos�l� rovnou, jinak do fronty.
@param id kdo se odhl�sil.
*/
void RozesliLogout(int id);
//void rozesliNezijuZpravy(); 
/**
Funkce po�le klientovi (pokud byloZiju) v�echny zpr�vy v�etn� odpov�di na �iju.
Pokud nebylo �iju, ned�l� se nic.

Vol� se pokud:
1. jednou za 10 vte�in (odpov�� na �iju)
2. klientovi do�la zpr�va (pokud !byloZiju, nestane se nic)
3. od klienta do�lo �iju (nap��klad do�la zpr�va, kdy� nebylo �iju)

@param i ukazatel na klienta
@param iJenZiju m� se poslat zpr�va, i kdy� krom� �iju ��dn� nen�
*/
void posliZpravyKlientovi(SCKlienti::iterator i, bool iJenZiju);
#endif

