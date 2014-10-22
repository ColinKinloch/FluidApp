#include "ContWindow.h"
#include <FL/Fl_Pack.h>

ContWindow::ContWindow(int w, int h, const char* title): Fl_Window(w,h,title)
{
  Fl_Pack* contPack = new Fl_Pack(0,0,w,h);
  contPack->type(Fl_Pack::HORIZONTAL);
  contPack->begin();
    restart = new Fl_Button(0,0,h,h, "@|<");
    play = new Fl_Toggle_Button(0,0,h,h, "@>");
    proceed = new Fl_Button(0,0,h,h, "@|>");
    tools = new Fl_Toggle_Button(0,0,50,h, "Tools");
    settings = new Fl_Toggle_Button(0,0,50,h, "Settings");
  contPack->end();
  set_non_modal();
  //resizable(contPack);
}
