#ifndef dbH
#define dbH
/**
  Funkce pro úèely ladìní, z loginu a hesla udìlá id u¾ivatele. Výsledná
  implementace bude sahat do databáze.
  @param login login u¾ivatele
  @param heslo heslo u¾ivatele
  @return in u¾ivatele
*/
int getID(const char *login, const char *heslo);
#endif

