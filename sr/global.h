#ifndef globalH
#define globalH
#include <hosemaphore.h>
#include <hoserversocket.h>
#include "fronta.h"
#include "zprava.h"
#include "struktury.h"

#define VerzeServeru 0

extern FrontaZprav *hlavniFronta;
extern SCKlienti klienti;
extern SCStolky stolky;
extern HoSemaphore semaforKlienti;
extern HoSemaphore semaforStolky;
extern HoSemaphore semaforKosteni;
extern HoServerSocket *mainSocket;
extern bool mamKoncit;
extern int port;
extern int ovladaci_port;

#endif

