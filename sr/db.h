#ifndef dbH
#define dbH
/**
  Funkce pro ��ely lad�n�, z loginu a hesla ud�l� id u�ivatele. V�sledn�
  implementace bude sahat do datab�ze.
  @param login login u�ivatele
  @param heslo heslo u�ivatele
  @return in u�ivatele
*/
int getID(const char *login, const char *heslo);
#endif

