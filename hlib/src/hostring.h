#ifndef HoStringH
#define HoStringH
#include <stdio.h>
class HoString;
class HoStringBody {
friend class HoString;
public:
	HoStringBody();
	~HoStringBody();
private:
  HoStringBody(wchar_t);
  HoStringBody(wchar_t *w, int alloc=1);
  HoStringBody(const char *);
  HoStringBody(const char *, int);
  void inc() {cnt++;};
  void dec();
  int strLen(void) const;
  int utf8Len(void) const {
    return strLen(); // !!!!!!
  }
  char * toString(void) const;
  void toString(char *str) const;
  char * toUtf8(void) const;
  void toUtf8(char *str) const;
  wchar_t * str;
  int cnt;
  static HoStringBody empty;
};

class HoString {
private:
  HoString(HoStringBody *);
  HoStringBody * body;
public:
  HoString();
  HoString(const HoString &);
  HoString & operator = (const HoString &);
  HoString operator + (const HoString &);
  int strLen(void) const {return body->strLen();};
  int utf8Len(void) const {return body->utf8Len();};
  void toString(char *str) const {body->toString(str);};
  void toUtf8(char *str) const {body->toUtf8(str);};
  void add(const HoString &s);
  void insert(const HoString &s, int pos);
  void del(int pos, int cnt);
  char * toString(void) const {return  body->toString();}; // vola malloc
  char * toUtf8(void) const {return  body->toUtf8();}; // vola malloc
  ~HoString();
  HoString(const char *);
  HoString(wchar_t c);
  HoString( const char *, int);
};
void init_encoding();
void done_encoding();

extern wchar_t *encoding, *iso, *win;
#endif
