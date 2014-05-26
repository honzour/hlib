#include "hoserversocket.h"
#include "holog.h"

#if hplatform==XW
#include <unistd.h>
#include <fcntl.h>
#elif hplatform==MW
#include <windows.h>
#endif

HoServerSocket::HoServerSocket(int port, int maxConnection) {
  this->port = port;
  this->maxConnection = maxConnection;
  connected = false;
}

bool HoServerSocket::listenBind() {
  struct sockaddr_in sockName;
  if ((handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    return false;
  }
  sockName.sin_family = AF_INET;
  sockName.sin_port = htons(port);
  sockName.sin_addr.s_addr = INADDR_ANY;
  if (bind(handle, (struct sockaddr *)&sockName, sizeof(sockName)) == -1) {
    uklid:
    close();
    handle = -1;
    return false;
  }
  if (listen(handle, maxConnection) == -1) {
    goto uklid;
    return false;
  }
  connected = true;
  return true;
}

HoSocket* HoServerSocket::accept() {
  if (handle == -1 ) {
    return NULL;
  }
  struct sockaddr_in clientInfo;
#if hplatform==XW
  socklen_t
#else
  int
#endif
	  addrlen = sizeof(clientInfo);
  int client = ::accept(handle, (struct sockaddr*)&clientInfo, &addrlen);
  if (client == -1) {
    return NULL;
  }
  HoSocket *r = new HoSocket();
  r->connected = 1;
  r->handle = client;
  return r;
}

void HoServerSocket::close() {
  holog.log(I4, "close(%i)\n", handle);
#if hplatform==XW
  ::close
#else
  ::closesocket
#endif
	  (handle);
  connected = false;
}

bool HoServerSocket::setBlocking(bool blocking) {
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
  return false;
#endif
}

bool HoServerSocket::waitForEvent(bool &error, bool &connection) {
  fd_set chyby, spojeni;
  FD_ZERO(&chyby);
  FD_ZERO(&spojeni);
  FD_SET(handle, &chyby);
  FD_SET(handle, &spojeni);
  int ret = select(1, &spojeni, NULL, &chyby, NULL);
  if (ret == -1) {
    return false;
  }
  error = FD_ISSET(handle, &chyby);
  connection = FD_ISSET(handle, &spojeni);
  return true;
}

