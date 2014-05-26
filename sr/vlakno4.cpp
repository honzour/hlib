#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <holog.h>
#include <hothread.h>
#include "rutiny.h"
#include "global.h"
#include "zprava.h"
#include "zpravy.h"

void Vlakno4(void * param) {
  int t = 0;
  SCKlienti::iterator i;
  holog.log(I3, "Zaèátek kostícího vlákna ¾ivota 4\n");
  semaforKosteni.down();
  while (1) {
    semaforKosteni.down(10, 0);
    semaforKlienti.down();
    if (mamKoncit) {
      break;
    }
 
    t++;
    if (t == 3) t = 0;
    for (i = klienti.begin(); i != klienti.end(); i++) {
      posliZpravyKlientovi(i, true);
    }
    if (!t) {
      holog.log(I1, "Kostím\n");
      time_t now = time(NULL);
      bool vykosten;
      do {
        vykosten = false;
        for (i = klienti.begin(); i != klienti.end(); i++) {
          time_t t = i->second->aktiv;
          if (t > now || now - t > 60) {
            holog.log(I2, "Klient fd=%p vyko¹tìn.\n", i->second->socket);
            i->second->smazFrontu();
            klienti.erase(i);
            vykosten = true;
            break;
	  }
        }
      } while (vykosten);
    }
    semaforKlienti.up();
  }
  semaforKlienti.up();
  holog.log(I3, "Konec kostícího vlákna");
}

