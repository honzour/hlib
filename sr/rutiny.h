#ifndef rutinyH
#define rutinyH
#include <hosocket.h>
#include "zprava.h"
#include "fronta.h"
#include "struktury.h"
void Vykosti(HoSocket *socket, bool lock);
/**
  Roze¹le 1 nebo 2 zprávy hráèùm. Semafor klientù nesklápí, musí u¾ být dole.
  Pokud je to zpráva stolku, sklápí semafor klientù. Zprávy posílá rovnou,
  pokud byloZiju, jinak do èekací fronty.
  @param zprava1 1. zpráva se po¹le v¾dy
  @param zprava2 2. zpráva, je-li NULL, nepo¹le se
  @param stolek je-li 0, v¹em hráèùm, jinak jen hráèùm u stolku
  @param kromeSoket tomu se neposílá, pokud NULL, ¾ádné omezení
*/

void Rozesli2ZpravyHracum(Zprava *zprava1, Zprava *zprava2, int stolek,
  HoSocket *kromeSoket, bool zamykatStolky = true);
/**
  Roze¹le 1 zprávu hráèùm. Semafor klientù nesklápí, musí u¾ být dole.
  Pokud je to zpráva stolku, sklápí semafor klientù. Zprávy posílá rovnou,
  pokud byloZiju, jinak do èekací fronty.
  @param zprava1 1. zpráva se po¹le v¾dy
  @param stolek je-li 0, v¹em hráèùm, jinak jen hráèùm u stolku
  @param kromeSoket tomu se neposílá, pokud NULL, ¾ádné omezení
*/
void RozesliZpravuHracum(Zprava *zprava, int stolek, HoSocket *kromeSoket,
  bool zamykatStolky = true);
/**
Ode¹le okam¾itì (bez èekání na ¾iju) zprávu ke hráèi tj push do fronty. 
@param zprava co se posílá
@param fronta fronta, kam se posílá
*/
void OdesliZpravuKeHraci(Zprava *zprava, Fronta<Zprava *> *fronta);
/**
Ode¹le okam¾itì (bez èekání na ¾iju) zprávu ke hráèi.
@param zprava co se posílá
@param id id hráèe. Semafor hráèù nesklápí (musí u¾ být sklopený).
*/

void OdesliZpravuKeHraciID(Zprava *zprava, int id);
/**
Po¹le zprávu bez parametru hráèi tím, ¾e ji vlo¾í do fronty, z ní¾ se u¾
rovnou posílá a neèeká se na ¾iju.
@param socket komu se posílá (ne ¾e by se tam rovnou psalo, socket je jen
       identifikátor hráèe)
@param zprava typ zprávy
@param posledni má se u¾ rovnou poslat nebo hned pøidáme dal¹í?
*/
void PosliJednoduchouZpravu(HoSocket *socket, int zprava, char posledni);
/**
Po¹le zprávu s int parametrem hráèi tím, ¾e ji vlo¾í do fronty, z ní¾ se u¾
rovnou posílá a neèeká se na ¾iju.
@param socket komu se posílá (ne ¾e by se tam rovnou psalo, socket je jen
       identifikátor hráèe)
@param zprava typ zprávy
@param posledni má se u¾ rovnou poslat nebo hned pøidáme dal¹í?
*/
void PosliIntZpravu(HoSocket *socket, int zprava, int param, char posledni);
/**
Po¹le zprávu ZPRAVA_OK hráèi tím, ¾e ji vlo¾í do fronty, z ní¾ se u¾
rovnou posílá a neèeká se na ¾iju.
@param socket komu se posílá (ne ¾e by se tam rovnou psalo, socket je jen
       identifikátor hráèe)
@param posledni má se u¾ rovnou poslat nebo hned pøidáme dal¹í?
*/

void PosliOK(HoSocket *socket, char posledni);
/**
Roze¹le v¹em logout hráèe. Nic nesklápí, semafor klientù musí být dole.
Pokud byloZiju posílá rovnou, jinak do fronty.
@param id kdo se odhlásil.
*/
void RozesliLogout(int id);
//void rozesliNezijuZpravy(); 
/**
Funkce po¹le klientovi (pokud byloZiju) v¹echny zprávy vèetnì odpovìdi na ¾iju.
Pokud nebylo ¾iju, nedìlá se nic.

Volá se pokud:
1. jednou za 10 vteøin (odpovìï na ¾iju)
2. klientovi do¹la zpráva (pokud !byloZiju, nestane se nic)
3. od klienta do¹lo ¾iju (napøíklad do¹la zpráva, kdy¾ nebylo ¾iju)

@param i ukazatel na klienta
@param iJenZiju má se poslat zpráva, i kdy¾ kromì ¾iju ¾ádná není
*/
void posliZpravyKlientovi(SCKlienti::iterator i, bool iJenZiju);
#endif

