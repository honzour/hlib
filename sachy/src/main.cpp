#include <hoapp.h>

#include "loginwindow.h"
#include "push.h"
#include "global.h"
#include "vlakna.h"

#include "zpravy.h"
#include "zprava.h"

int homain(int argc,char * argv[]){
 
  new HoApp(argc, argv);
  HoMainApp->userEvent = onEvent;
  loginOkno = new LoginWindow;
  semaforSocketu = new HoSemaphore(1);
  semaforPsaniSocketu = new HoSemaphore(1);
  HoMainApp->run();
  delete semaforPsaniSocketu;
  delete semaforSocketu;
  delete HoMainApp;
  return 0;
}

