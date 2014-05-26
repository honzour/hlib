#include "hoset.h"
#if hplatform==XW
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#elif hplatform==MW
#include <windows.h>
#endif
#include <fcntl.h>

#include "hosocket.h"

HoSocket::HoSocket(){
  connected = 0;
  handle = 0;
#if hplatform==XW
  pipe(pipa);
#endif
}

int HoSocket::getHandle() {
  return (int) handle;
}

void HoSocket::close() {

  if (!connected) return;
#if hplatform==XW
  char c = 0;
  write(pipa[1], (void *) &c, 1);
  ::close
#elif hplatform==MV
  closesocket
#endif	 
    (handle);
  connected = 0;
  handle = 0;
}

bool HoSocket::connect(const char *hst, int port) {
  sockaddr_in serverSock; 
  hostent *host;

  handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if ((host = gethostbyname(hst)) == NULL) return false;
  serverSock.sin_family = AF_INET;
  serverSock.sin_port = htons(port);
  memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);
  if (::connect(handle, (sockaddr *)&serverSock, sizeof(serverSock)) == -1) {
    close(); 
    return false;
  }
  connected = 1;
  return true;
}

int HoSocket::send(const void *buf, int size){
  int j, i;
  if (!connected) return 0;
  i = 0;
  do {
    j = ::send(handle,
#if hplatform==XW
      (const void *)
#elif hplatform==MW
      (const char *)
#endif
       (((const char *) buf) + i),
       size,
       0
      );
     if (j < 0) {
      close();
      return i;
    }
    i += j;
 } while (j > 0 && i < size);
  return i;
}

int HoSocket::recv(void *buf, int size){
  int j, i;
  
  if (!connected) return 0;
  i = 0;
  do {
#if hplatform==XW
    fd_set r; 
    FD_ZERO(&r);
    FD_SET(handle, &r);
    FD_SET(pipa[0], &r);
    select((handle > pipa[0] ? handle : pipa[0]) + 1, &r, NULL, NULL, NULL);
    if (FD_ISSET(pipa[0], &r)) {
      return i;
    }
#endif

    j = ::recv(handle,
#if hplatform==XW
      (void *)
#endif
      (((char *)buf) + i), size, 
#if hplatform==XW
      MSG_WAITALL
#else
      0
#endif
      );
    if (j < 0) {
      close();
      return i;
    }
    i += j;
  } while (j > 0 && i < size);
  return i;
}

bool HoSocket::nactiInt(int *i, bool changeOrder) {
  int cnt, tmp;
  
  cnt = recv(&tmp, 4);
  if (cnt != 4) {
    return false;
  }
  if (changeOrder) {
    tmp = ntohl(tmp);
  }
  *i = tmp;
  return true;
}

HoSocket::~HoSocket(){
  close();
#if hplatform==XW
  ::close(pipa[0]);
  ::close(pipa[1]);
#endif
}

bool HoSocket::setBlocking(bool blocking) {
#if hplatform==XW
  long arg = fcntl(handle, F_GETFL, 0);
  if (blocking) {
    arg &= ~O_NONBLOCK;
  } else {
    arg |= O_NONBLOCK;
  }
  if (fcntl(handle, F_SETFL, arg) == -1) {
    return false;
  }
  return true;
#else
  // TODO
  return false;
#endif
}


