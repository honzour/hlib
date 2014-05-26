#ifndef HoPipeH
#define HoPipeH
#include "hoset.h"
#if hplatform == MW
#include <windows.h>
#endif
#include "horeader.h"
#include "howriter.h"

class HoPipe : public HoReader, HoWriter {
#if hplatform == MW
  HANDLE
#else
  int
#endif
 fd[2];

public:
  HoPipe();
#if hplatform == MW
  HoPipe(HANDLE fr, HANDLE fw)
#else
  HoPipe(int fr, int fw)
#endif
 {
   fd[0] = fr;
   fd[1] = fw;
  }
  virtual int read(int size, void *data);
  virtual int write(int size, void *data);
  virtual ~HoPipe();
};
#endif

