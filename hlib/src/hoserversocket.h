#ifndef HoServerSocketH
#define HoServerSocketH
#include "hoset.h"
#include "hosocket.h"

class HoServerSocket {
private:
  int handle;
  int port;
  int maxConnection;
  bool connected;
public:
  HoServerSocket(int port, int maxConnection);
  virtual bool listenBind();
  virtual HoSocket *accept();
  /**
     Pokud connected, zavøe socket.
   */
  virtual void close();
  virtual ~HoServerSocket() {
    if (connected) {
      close();
    }
  };
  virtual bool setBlocking(bool blocking);
  virtual bool waitForEvent(bool &error, bool &connection);
};

#endif

