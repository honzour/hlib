#ifndef hologH
#define hologH
#include <stdlib.h>

#define D1 __FILE__, __LINE__, 0
#define D2 __FILE__, __LINE__, 1
#define D3 __FILE__, __LINE__, 2
#define D4 __FILE__, __LINE__, 3

#define I1 __FILE__, __LINE__, 4
#define I2 __FILE__, __LINE__, 5
#define I3 __FILE__, __LINE__, 6
#define I4 __FILE__, __LINE__, 7

#define W1 __FILE__, __LINE__, 8
#define W2 __FILE__, __LINE__, 9
#define W3 __FILE__, __LINE__, 10
#define W4 __FILE__, __LINE__, 11

#define E1 __FILE__, __LINE__, 12
#define E2 __FILE__, __LINE__, 13
#define E3 __FILE__, __LINE__, 14
#define E4 __FILE__, __LINE__, 15

#define F1 __FILE__, __LINE__, 16
#define F2 __FILE__, __LINE__, 17
#define F3 __FILE__, __LINE__, 18
#define F4 __FILE__, __LINE__, 19


class HoLog {
  private:
    char *fileName;
    int d, i, w, e, f;
  public:
    HoLog(const char *fileName = NULL,
      int d = 5, int i = 4, int w = 3, int e = 2, int f = 1) {
      this->fileName = NULL;
      init(fileName, d, i, w, e, f);
    };
    void init(const char *fileNeme = NULL,
      int d = 5, int i = 4, int w = 3, int e = 2, int f = 1);
    void log(const char *source, int line, int level, const char *format, ...);
    ~HoLog() {
	  if (fileName) free(fileName);
    };
};

extern HoLog holog;
#endif

