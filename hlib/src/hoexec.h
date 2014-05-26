#ifndef HoExecH
#define HoExecH
#include "hopipe.h"
bool hoExec(const char *path, char *const argv[]);
HoPipe * hoExecWithPipe(const char *path, char *const argv[]);
#endif
