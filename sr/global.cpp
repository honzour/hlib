#include "global.h"
#include "fronta.h"
#include "zprava.h"

FrontaZprav *hlavniFronta;
SCKlienti klienti;
SCStolky stolky;
HoSemaphore semaforKlienti;
HoSemaphore semaforStolky;
HoSemaphore semaforKosteni;

bool mamKoncit = false;
int port, ovladaci_port;


