#include <holog.h>
#include "zprava.h"
#include "fronta.h"
#include "vlaknok.h"
#include "global.h"
#include "rutiny.h"
/**
Vlákno ète z fronty hráèe a pøeposílá mu zprávy po socketu.
@param p FrontaSocket pøetypovaný na void *. Ze které fronty má èíst a kam má
 psát.
*/
void VlaknoVedlejsiPipy(void *p) {
 
  FrontaSocket *fs = (FrontaSocket *) p;
  FrontaZprav *fronta = fs->f;
  HoSocket *socket = fs->s;
  delete fs;
  holog.log(I2, "VlaknoVedlejsiPipy %p %p\n", fronta, socket);

  Zprava *z;
  while (z = fronta->pop()) { 
   // TODO nejakej test, jestli je pak fronta prazdna a pokud ne, poslat
   // vsechno s priznakem "posledni" az u posledni zpravy
    holog.log(I2, "Fronta pop %p %p\n", fronta, socket);
    unsigned char buf[9];
    buf[0] = z->posledni;
    *((int *)(buf + 1)) = htonl(z->typ);
    *((int *)(buf + 5)) = htonl(z->delka);
    if (socket->send((void *)buf, 9) != 9 ||
        socket->send((void *)(z->param), z->delka) != z->delka) {
      holog.log(W2, "Chyba send, asi spatny klient\n");
      delete z;
      Vykosti(socket, true);
      break;
    } else {
      holog.log(W2, "Bylo send\n");
    }
    delete z;
  }
  while (!fronta->empty()) {
    z = fronta->pop();
    if (z) delete z;
  }

  holog.log(I2, "Konec while cyklu %p %p\n", fronta, socket);
}

