#ifndef HoPanelH
#define HoPanelH
#include "hoset.h"
#if hplatform==XW
#include <X11/Xlib.h>
#elif hplatform==MW
#include <windows.h>
#endif

#include "howindow.h"
#include "hostring.h"

class HoPanel : public HoWindow
{
public:
   HoPanel(HoWindow *parent, int w = -1, int h = -1) // kontajner
     :HoWindow(parent, "", w, h) {
   };
   // -1 znamená neurèeno
   HoPanel(HoWindow *parent, int x, int y, int w, int h):
     HoWindow(parent, "", w, h, w, h) {
   };
};
#endif
