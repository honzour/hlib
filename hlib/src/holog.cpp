#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "hoset.h"
#if hplatform==MW
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include "holog.h"
void HoLog::init(const char *fileName, int d, int i, int w, int e, int f) {
  if (this->fileName) free((void *) this->fileName);
  this->fileName = (fileName ? strdup(fileName) : NULL);
  this->d = d;
  this->i = i;
  this->w = w;
  this->e = e;
  this->f = f;
}

void HoLog::log(const char *source, int line, int level,
  const char *format, ...) {
  char str[1024];
  va_list list;
  char c;
 
  if (level < 0 || level > 19) return;
  switch (level / 4) {
    case 0:
      if (level % 4 < d - 1) return;
      c = 'D';
      break;
    case 1:
      if (level % 4 < i - 1) return;
      c = 'I';
      break;
    case 2:
      if (level % 4 < w - 1) return;
      c = 'W';
      break;
    case 3:
      if (level % 4 < e - 1) return;
      c = 'E';
      break;
    case 4:
      if (level % 4 < this->f - 1) return;
      c = 'F';
      break;
  }
  FILE *f = NULL;
  if (fileName) {
    fopen(fileName, "a");
    if (!f) return;
  } else {
	f = stdout;
  }
  va_start(list, format);
#if hplatform==MW
  _vsnprintf
#else
  vsnprintf
#endif
  (str, 1023, format, list);
  va_end(list);
#if hplatform==XW
  time_t tt = time(NULL);
  struct tm *t = localtime(&tt);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  fprintf(f, "%c%i %i.%i.%i %i:%i:%i.%i %s:%i %s", c, level % 4 + 1,
    t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, 
    t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec,
    source, line, str);
#else
  SYSTEMTIME st;
  GetLocalTime(&st);
  fprintf(f, "%c%i %i.%i.%i %i:%i:%i.%i %s:%i %s", c, level % 4 + 1,
    st.wDay, st.wMonth, st.wYear, 
    st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
    source, line, str);
#endif
  if (!f) return;
  fclose(f);
}

HoLog holog("holog.txt", 5, 5, 5, 5, 5);

