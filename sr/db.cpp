#include <string.h>

/**
  Funkce pro ��ely lad�n�, z loginu a hesla ud�l� id u�ivatele. V�sledn�
  implementace bude sahat do datab�ze.
  @param login login u�ivatele
  @param heslo heslo u�ivatele
  @return in u�ivatele
*/
int getID(const char *login, const char *heslo) {
  if (!strcmp(login, "a") && !strcmp(heslo, "w"))
    return 1;
  if (!strcmp(login, "j") && !strcmp(heslo, "n"))
    return 2;
  if (!strcmp(login, "m") && !strcmp(heslo, "s"))
    return 3;
  return 0;
}

#if 0
static bool getLogin(int id, char *login) {
  switch (id) {
    case 1:
      strcpy(login, "a");
      return true;
    case 2:
      strcpy(login, "j");
      return true;
  }
  return false;
}
#endif

