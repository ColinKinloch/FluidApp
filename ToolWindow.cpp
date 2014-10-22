#include "ToolWindow.h"

ToolWindow::ToolWindow(int w, int h, const char* title) : Fl_Window(w, h, title)
{
  Fl_Pack* contPack = new Fl_Pack(0,0,w,h);
  contPack->type(Fl_Pack::VERTICAL);
  contPack->begin();

  contPack->end();
  set_non_modal();
}
