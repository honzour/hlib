#include "homsgbox.h"
#include "hopushbutton.h"
#include "holabel.h"

static void globalPush(HoPushButton *b, void *param) {
  HoMsgBox *mb = (HoMsgBox *) param;
  if (mb->push) mb->push(mb, mb->param);
  mb->destroy();
}

HoMsgBox::HoMsgBox(HoString caption, HoString label,
  void (* push) (HoMsgBox *, void *), void *param
)
 : HoWindow(NULL, caption)
 {
  new HoLabel(this, label);
  this->push = push;
  this->param = param;
  new HoPushButton(this, "OK", globalPush, this);
  pack();
 }
 
