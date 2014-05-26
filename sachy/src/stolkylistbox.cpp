#include <hostring.h>
#include <stdlib.h>
#include "stolkylistbox.h"
#include "global.h"

HoString getListboxString(SCStolek *stolek) {
//  return "nevim";
  HoString b("[volno]");
  HoString c = b;
  SCUseri::iterator i;
  if (stolek->bily && (i = useri.find(stolek->bily)) != useri.end()) {
    b = i->second->login;
  } else {
    if (stolek->bily) b = "?";
  }
  if (stolek->cerny && (i = useri.find(stolek->cerny)) != useri.end()) {
    c = i->second->login;
  } else {
    if (stolek->cerny) c = "?";
  }
  return (b + HoString(" x ") + c);
}

void StolkyListbox::changed(int stolek) {
  vector<SCStolek *>::iterator vi;
  int i = 1;
  
  for (vi = stoly.begin();
       vi != stoly.end();
       vi++) {
       
    if ((*vi)->id == stolek) {
      break;
    }
    i++;
  }
  if (vi == stoly.end()) return;
  changeString(i, getListboxString(*vi));
  char *s = getListboxString(*vi).toString();
  printf("%s\n", s);
  free(s);
}

void StolkyListbox::deleted(int stolek) {
  vector<SCStolek *>::iterator vi;
  int i = 1;
  
  for (vi = stoly.begin();
       vi != stoly.end();
       vi++) {
       
    if ((*vi)->id == stolek) {
      break;
    }
    i++;
  }
  if (vi == stoly.end()) return;
  removeString(i);
  stoly.erase(vi);
}

void StolkyListbox::insert(SCStolek *stolek) {
  stoly.push_back(stolek);
  addString(getListboxString(stolek));
}

