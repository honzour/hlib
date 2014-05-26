#include "struktury.h"
#include <stdlib.h>

void SCUseri::print() {
  for (SCUseri::iterator i = begin(); i != end(); i++)
    i->second->print();
}

void SCUser::print() {
  char *s = login.toString();
  printf("[%i, %s]\n", id, s);
  free(s);
}

void SCUseri::clearDelete() {
  for (SCUseri::iterator i = begin(); i != end(); i++)
    delete i->second;
  clear();
}

void SCStolky::clearDelete() {
  for (SCStolky::iterator i = begin(); i != end(); i++)
    delete i->second;
  clear();
}

bool parseLogin(const Zprava *z, SCStolky *s, SCUseri *u) {
  s->clearDelete();
  u->clearDelete();
  if (z->typ != ZPRAVA_LOGIN_OK) {
    puts("Typ zpravy");
    return false;
  }
  if (z->delka < 12) {
    printf("Delka zpravy %i < 12\n", z->delka);
    return false;
  }
  unsigned char *c = z->param;
  unsigned char *k = c + z->delka;
  unsigned char *p = c;
  p += 4;
  int n = ntohl(*((int *) p));
  p += 4;
  int m = ntohl(*((int *) p));
  p += 4;
  int i, j;
  for (i = 0; i < n; i++) {
    if (k - p < 5) {
      printf("k - p = %i < 5, n = %i, m = %i\n", k - p, n, m);
      return false;
    }
    int id = ntohl(*(int *)p);
    if (u->find(id) != u->end()) {
      puts("find");
      return false;
    }
    p += 4;
    c = p;
    while (*p && p < k) p++;
    if (*p) {
      puts("*p");
      return false;
    }
    HoString s((char *) c);
    p++;
    u->insert(SCUseri::value_type(id, new SCUser(id, s))); 
  }
  for (i = 0; i < m; i++) {
    if (k - p < 24) {
      puts("k - p < 24");
      return false;
    }
    SCStolek *stolek = new SCStolek(
      ntohl(*(int *) p),
      ntohl(*(int *) (p + 4)),
      ntohl(*(int *) (p + 8)),
      ntohl(*(int *) (p + 12)),
      ntohl(*(int *) (p + 16))
      );
    int prihlizi = ntohl(*(int *) (p + 20));
    p += 24;
    if (prihlizi * 4 > k - p) {
      delete stolek;
      puts("prihlizi * 4 > k - p");
      return false;
    }
    for (j = 0; j < prihlizi; j++) {
      stolek->prihlizi.push_back(ntohl(*(int *) p));
      p += 4;
    }
    printf("Stolek[%i %i]\n", stolek->bily, stolek->cerny);
    s->insert(SCStolky::value_type(stolek->id, stolek));
  }
  return true;
}

bool SCStolek::prisedl(int hrac, bool pozorovatel) {
  if (bily == hrac || cerny == hrac) return false;
  vector<int>::iterator vi;
  for (vi = prihlizi.begin(); vi != prihlizi.end(); vi++) {
    if (*vi == hrac) return false;
  }
  if (pozorovatel) prihlizi.push_back(hrac); else {
    if (!bily) bily = hrac; else
    if (!cerny) cerny = hrac; else return false;
  }
  return true;
}
bool SCStolky::prisedl(int stolek, int hrac, bool pozorovatel) {
  SCStolky::iterator i = find(stolek);
  if (i == end()) return false;
  return i->second->prisedl(hrac, pozorovatel);
}
