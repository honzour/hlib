#include "hopipe.h"
#if hplatform == MW
#include <windows.h>
#else
#include <unistd.h>
#endif



HoPipe::HoPipe() {
#if hplatform == MW
  CreatePipe(fd, fd + 1, NULL, 0);
#else
  pipe(fd);
#endif
};

int HoPipe::read(int size, void *data) {
#if hplatform == MW
  DWORD r;
  if (!ReadFile(fd[0], data, size, &r, NULL)) return -1;
  return r;
#else
  return ::read(fd[0], data, size);
#endif
};

int HoPipe::write(int size, void *data) {
#if hplatform == MW
  DWORD r;
  if (!WriteFile(fd[1], data, size, &r, NULL)) return -1;
  return r;
#else
  return ::read(fd[1], data, size);
#endif
}

HoPipe::~HoPipe() {
  for (int i = 0; i < 2; i++) {
#if hplatform == MW
    CloseHandle(fd[i]);
#else
    close(fd[i]);
#endif
  }
}

