#ifndef HoSocketH
#define HoSocketH
#include "hoset.h"
#if hplatform==XW
#include <netinet/in.h>
#endif
/**
  T��da reprezentuj�c� klientsk� socket.
*/
class HoSocket{
#if hplatform==XW
  /**
   P�pa slou��c� k tomu, aby close() v jednom vl�knu p�eru�ilo zablokovan�
   read() v jin�m vl�knu. Na linuxu to toti� automaticky nefunguje.
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
     Jsme p�ipojen� k serveru. (Prob�hlo �sp�n� conect?)
   */
   int connected;
   /**
     Konstruktor, jen nastav� prom�nn�, nevytv��� ��dn� syst�mov� prost�edky.
   */
   HoSocket();
   /**
     Pokud connected, zav�e socket.
   */
   virtual void close();
   /**
     P�ipoj� se k serveru, vytvo�� syst�mov� socket a zavol� connect.
     @param hst nap�. "sachy.bonusweb.cz"
     @param port nap�. 80
     @return true ok, false chyba
   */
   virtual bool connect(const char *hst, int port);
   /**
     P�e do socketu tak dlouho, dokud nepo�le size byt� nebo nedojde k chyb�.
     @param buf pos�lan� buffer bufer
     @param size velikost buferu
     @return posl�no byt�
   */
   virtual int send(const void *buf, int size);
   /**
     �te ze socketu tak dlouho, dokud nep�e�te size byt� nebo nedojde k chyb�.
     @param buf �ten� bufer
     @param size velikost buferu
     @return p�e�teno byt�
   */
   virtual int recv(void *buf, int size);
   virtual bool nactiInt(int *i, bool changeOrder);
   /**
     Destruktor. Uzav�e p��padn� spojen�.
   */
   virtual ~HoSocket();

   virtual int getHandle();
   virtual bool setBlocking(bool blocking);
};
#endif

