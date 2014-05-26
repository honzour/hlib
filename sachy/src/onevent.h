#ifndef oneventH
#define oneventH

#define CHYBA_SEND 1000000
#define CHYBA_RECV 1000001

//#error tohle se posila kdyz se spatne prihlasim a potom to zkusim znovu
// nebot jeste stale existuje stary socket
#define CHYBA_INIT_SOCKET_2 1000002
//#define CHYBA_RECV 1000001

void onEvent(void *param); 
#endif


