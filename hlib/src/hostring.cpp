#include <stdlib.h>
#include <string.h>
#include "hostring.h"
#include "hoset.h"

static wchar_t iso8859_2[128]={
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x00A0,0x0104,0x02D8,0x0141,0x00A4,0x013D,0x015A,0x00A7,
0x00A8,0x0160,0x015E,0x0164,0x0179,0x00AD,0x017D,0x017B,
0x00B0,0x0105,0x02DB,0x0142,0x00B4,0x013E,0x015B,0x02C7,
0x00B8,0x0161,0x015F,0x0165,0x017A,0x02DD,0x017E,0x017C,
0x0154,0x00C1,0x00C2,0x0102,0x00C4,0x0139,0x0106,0x00C7,
0x010C,0x00C9,0x0118,0x00CB,0x011A,0x00CD,0x00CE,0x010E,
0x0110,0x0143,0x0147,0x00D3,0x00D4,0x0150,0x00D6,0x00D7,
0x0158,0x016E,0x00DA,0x0170,0x00DC,0x00DD,0x0162,0x00DF,
0x0155,0x00E1,0x00E2,0x0103,0x00E4,0x013A,0x0107,0x00E7,
0x010D,0x00E9,0x0119,0x00EB,0x011B,0x00ED,0x00EE,0x010F,
0x0111,0x0144,0x0148,0x00F3,0x00F4,0x0151,0x00F6,0x00F7,
0x0159,0x016F,0x00FA,0x0171,0x00FC,0x00FD,0x0163,0x02D9};

static wchar_t cp1250[128]={
0x20AC,0x0000,0x201A,0x0000,0x201E,0x2026,0x2020,0x2021,
0x0000,0x2030,0x0160,0x2039,0x015A,0x0164,0x017D,0x0179,
0x0000,0x2018,0x2019,0x201C,0x201D,0x2022,0x2013,0x2014,
0x0000,0x2122,0x0161,0x203A,0x015B,0x0165,0x017E,0x017A,
0x00A0,0x02C7,0x02D8,0x0141,0x00A4,0x0104,0x00A6,0x00A7,
0x00A8,0x00A9,0x015E,0x00AB,0x00AC,0x00AD,0x00AE,0x017B,
0x00B0,0x00B1,0x02DB,0x0142,0x00B4,0x00B5,0x00B6,0x00B7,
0x00B8,0x0105,0x015F,0x00BB,0x013D,0x02DD,0x013E,0x017C,
0x0154,0x00C1,0x00C2,0x0102,0x00C4,0x0139,0x0106,0x00C7,
0x010C,0x00C9,0x0118,0x00CB,0x011A,0x00CD,0x00CE,0x010E,
0x0110,0x0143,0x0147,0x00D3,0x00D4,0x0150,0x00D6,0x00D7,
0x0158,0x016E,0x00DA,0x0170,0x00DC,0x00DD,0x0162,0x00DF,
0x0155,0x00E1,0x00E2,0x0103,0x00E4,0x013A,0x0107,0x00E7,
0x010D,0x00E9,0x0119,0x00EB,0x011B,0x00ED,0x00EE,0x010F,
0x0111,0x0144,0x0148,0x00F3,0x00F4,0x0151,0x00F6,0x00F7,
0x0159,0x016F,0x00FA,0x0171,0x00FC,0x00FD,0x0163,0x02D9
};

wchar_t *encoding, *iso, *win;

void init_encoding(){
  wchar_t i; /* S int i nefungovala release verze s MS VC++5.0*/
  iso = (wchar_t *) malloc(sizeof(wchar_t) * 256);
  win = (wchar_t *) malloc(sizeof(wchar_t) * 256);
  for (i = 0; i < 128; i++) win[i] = iso[i] = (wchar_t)i;
  for(i = 128; i < 256; i++){
    iso[i] = iso8859_2[i - 128];
    win[i] = cp1250[i - 128];
  }
#if hplatform==XW
  encoding = iso;
#elif hplatform==MW
  encoding = win;
#endif
}
void done_encoding(){
 free(iso);
 free(win);
}

static inline wchar_t cw(char c, wchar_t *pole){
 return pole[(unsigned char)c];
}

static char wc(wchar_t w, wchar_t *pole){
/* Tohle budu muset zlepsit */
  for (int i = 0; i < 256; i++)
    if (pole[i] == w) return i;
  return '?';
}


HoStringBody::HoStringBody(wchar_t c) {
  str = (wchar_t *) malloc(2 * sizeof(wchar_t));
  str[0] = c;
  str[1] = 0;
  cnt = 1;
}

HoStringBody::HoStringBody(wchar_t *w, int alloc){
  if (!alloc) str = w; else {
    int i;
    for (i = 0; w[i]; i++);
    i++;
    str = (wchar_t *) malloc(i * sizeof(wchar_t));
    for (i=0; w[i]; i++) str[i] = w[i];
    str[i] = w[i];
  }
  cnt = 1;
}

HoStringBody::~HoStringBody() {
  free(str);
}

void HoStringBody::dec() {
  if (!--cnt) delete this;
}

HoStringBody::HoStringBody() {
  str = (wchar_t *)malloc(sizeof(wchar_t));
  str[0] = 0;
  cnt = 1;
}

HoStringBody HoStringBody::empty;

HoStringBody::HoStringBody(const char *s, int coding){
  wchar_t *cd,pom, *p;

  cnt = 1;
  str = (wchar_t *) malloc(sizeof(wchar_t) * (strlen(s) + 1));
  cd = str;
  
  if (coding) p = win; else p = iso;
  while (1) {
    
    pom = cw(*s, p);
    if (!pom && *s) pom = 32;
    if (!(*cd = pom)) break;
    cd++; s++;
 }
}

HoStringBody::HoStringBody(const char *s){
  wchar_t *cd, pom;
  cnt = 1;
  str = (wchar_t *) malloc(sizeof(wchar_t) * (strlen(s) + 1));
  cd = str;
  while(1) {
    pom = cw(*s, encoding);
    if (!pom && *s) pom = 32;
    if (!(*cd = pom)) break;
    cd++; s++;
  }
}

HoString::HoString(const char *str, int coding){
  body = new HoStringBody(str, coding);
}

HoString::HoString(const char *str){
  body = new HoStringBody(str);
}

HoString::HoString(wchar_t c){
  body = new HoStringBody(c);
}

HoString::HoString(){
  (body = &HoStringBody::empty)->inc();
}

HoString::HoString(const HoString &h) {
  body = h.body;
  body->inc();
}

HoString & HoString::operator=(const HoString &h) {
  body = h.body;
  body->inc();
  return *this;
}

HoString HoString::operator + (const HoString &h) {
  HoString r(*this);
  r.add(h);
  return r;
}

void HoStringBody::toString(char *s) const {
  wchar_t *cs;
  cs = str;
  while (1) {
    if (!(*s = wc(*cs, encoding))) break;
    cs++; s++;
  }
}

char * HoStringBody::toString(void) const{
  int l = strLen();
  char *s = (char *)malloc(l+1);
  toString(s);
  return s;
}

void HoStringBody::toUtf8(char *s) const {
  wchar_t *cs;
  cs = str;
  while (1) { // !!!!!!!!!!!!!!!!
    if (!(*s = wc(*cs, encoding))) break;
    cs++; s++;
  }
}

char * HoStringBody::toUtf8(void) const{
  int l = utf8Len();
  char *s = (char *)malloc(l+1);
  toUtf8(s);
  return s;
}

HoString::~HoString(){
  body->dec();
}

int HoStringBody::strLen() const{
  int i;
  for(i = 0; str[i]; i++);
  return i;
}

void HoString::add(const HoString &s){
  int ja, on, my, i, j;
  ja = strLen();
  on = s.strLen();
  if (!on) return;
  my = ja + on + 1;
  wchar_t *w = (wchar_t *) malloc(sizeof(wchar_t) * my);
  for (i = 0; i < ja; i++) w[i] = body->str[i];
  for (j = 0; j <= on; j++, i++) w[i] = s.body->str[j];
  body->dec();
  body = new HoStringBody(w, 0);
}

void HoString::insert(const HoString &s, int pos){
 int ja, on, my, i, j;
 ja=strLen();
 if(pos<0 || pos>ja) return;
 if(pos==ja){add(s); return;}
 on=s.strLen();
 if(!on)return;
 my=ja+on+1;
 wchar_t *w=(wchar_t *)malloc(sizeof(wchar_t)*my);
 for(i=0; i<pos; i++) w[i]=body->str[i];
 for(j=0; j<on; j++, i++) w[i]=s.body->str[j];
 for(j=pos; i<my; j++, i++) w[i]=body->str[j];
 body->dec();
 body=new HoStringBody(w,0);
}

void HoString::del(int pos, int cnt){
 int i,j,ja;
 if(pos<0 || cnt<=0)return;
 ja=strLen();
 if(pos>=ja)return;
 i=ja-pos;
 if(i<cnt)cnt=i;
 wchar_t *w=(wchar_t *)malloc(sizeof(wchar_t)*(ja-cnt+1));
 for(j=0;j<pos;j++)w[j]=body->str[j];
 for(i=pos+cnt;i<=ja;i++,j++)w[j]=body->str[i];

 body->dec();
 body=new HoStringBody(w,0);
}

