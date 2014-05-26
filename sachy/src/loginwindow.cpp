#include <hoapp.h>
#include <hostring.h>
#include <holabel.h>
#include <hopushbutton.h>
#include "loginwindow.h"
#include "global.h"
#include "push.h"

LoginWindow::LoginWindow():
  HoWindow(NULL, HoString("Pøihlášení", 1),
    50, 50, 700, 520, HoWindow::styleSizing | HoWindow::styleCaption)
{
  int fx, fy;

  HoMainApp->getDefaultFontSize(&fx, &fy);
  new HoLabel(this, " Login:", fx * 20, (fy * 3) / 2);
  loginEdit = new HoEdit(this, "", fx * 20, (fy * 4) / 2);
  new HoLabel(this, " Heslo:", fx * 20, (fy * 3) / 2);
  hesloEdit = new HoEdit(this, "", fx * 20, (fy * 4) / 2);
  new HoPushButton(this, HoString("Budiž", 1), fx * 20, (fy * 5) / 2,
    pushLogin);
  pack();
  create();
}

