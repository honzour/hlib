#ifndef HoSocketH
#define HoSocketH
#include "hoset.h"
#if hplatform==XW
#include <netinet/in.h>
#endif
/**
  Tøída reprezentující klientský socket.
*/
class HoSocket{
#if hplatform==XW
  /**
   Pípa slou¾ící k tomu, aby close() v jednom vláknu pøeru¹ilo zablokované
   read() v jiném vláknu. Na linuxu to toti¾ automaticky nefunguje.
  */
  int pipa[2];
#endif
  public:
  /**
    handle/fd socketu
  */
#if hplatform==XW
  int
#elif hplatform==MW
  SOCKET
#endif
   handle;
   /**
     Jsme pøipojení k serveru. (Probìhlo úspì¹nì conect?)
   */
   int connected;
   /**
     Konstruktor, jen nastaví promìnné, nevytváøí ¾ádné systémové prostøedky.
   */
   HoSocket();
   /**
     Pokud connected, zavøe socket.
   */
   virtual void close();
   /**
     Pøipojí se k serveru, vytvoøí systémový socket a zavolá connect.
     @param hst napø. "sachy.bonusweb.cz"
     @param port napø. 80
     @return true ok, false chyba
   */
   virtual bool connect(const char *hst, int port);
   /**
     Pí¹e do socketu tak dlouho, dokud nepo¹le size bytù nebo nedojde k chybì.
     @param buf posílaný buffer bufer
     @param size velikost buferu
     @return posláno bytù
   */
   virtual int send(const void *buf, int size);
   /**
     Ète ze socketu tak dlouho, dokud nepøeète size bytù nebo nedojde k chybì.
     @param buf ètený bufer
     @param size velikost buferu
     @return pøeèteno bytù
   */
   virtual int recv(void *buf, int size);
   virtual bool nactiInt(int *i, bool changeOrder);
   /**
     Destruktor. Uzavøe pøípadné spojení.
   */
   virtual ~HoSocket();

   virtual int getHandle();
   virtual bool setBlocking(bool blocking);
};
#endif

