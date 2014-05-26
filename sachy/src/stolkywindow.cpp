#include <hopanel.h>
#include <holistbox.h>
#include <howindow.h>
#include <hoapp.h>
#include <hopushbutton.h>
#include "stolkylistbox.h"
#include "stolkywindow.h"
#include "global.h"
#include "push.h"

StolkyWindow::StolkyWindow(HoWindow *parent)
    :HoWindow(parent, "Hrací místnost",
      HoWindow::styleSizing | HoWindow::styleCaption) {
  int fx, fy;

  programKonci = false;
  HoMainApp->getDefaultFontSize(&fx, &fy);
  delete m_layout;
  m_layout = new HoVBoxLayout(this);
      
  HoPanel *panel = new HoPanel(this);
  delete panel->m_layout;
  panel->m_layout = new HoHBoxLayout(panel);
  stolkyListbox = new StolkyListbox(panel, fx * 30, fy * 20);
  for (SCStolky::iterator i = stolky.begin(); i != stolky.end(); i++) {
    stolkyListbox->insert(i->second);
  }

  HoPanel *panel2 = new HoPanel(panel);
  new HoPushButton(panel2, HoString("Pøisednout", 0), 18 * fx, 2 * fy,
    pushPrisednout);
  new HoPushButton(panel2, "Pozorovatel", 18 * fx, 2 * fy, pushPozorovatel);
  new HoPushButton(panel2, "Nový", 18 * fx, 2 * fy, pushNovyStolek);
   //  new HoPushButton(panel2, "®iju", 18 * fx, 2 * fy, pushZiju);

  HoPanel *panel3 = new HoPanel(this);
  delete panel3->m_layout;
  panel3->m_layout = new HoHBoxLayout(panel3);
  chatEdit = new HoEdit(panel3, "", fx * 40, (fy * 4) / 2);
  new HoPushButton(panel3, HoString("Po¹li", 0) , 10 * fx, 2 * fy, pushChat);
  chatListbox = new HoListbox(this, fx * 50, fy * 8, NULL, false);
}

int StolkyWindow::canClose() {
  programKonci = true;
  pocetOken = 0;
  SCStolky::iterator i;
  for (i = stolky.begin(); i != stolky.end(); i++) {
    if (i->second->sw) {
      pocetOken++;  
    }
  }

  for (i = stolky.begin(); i != stolky.end(); i++) {
    if (i->second->sw) {
      i->second->sw->mamPoslatZpravu = 1;
      i->second->sw->canClose();
      i->second->sw->destroy();
      i->second->sw = NULL;
    }
  }
  return !pocetOken;
}

