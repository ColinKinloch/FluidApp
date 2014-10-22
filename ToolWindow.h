

#pragma once

#include <FL/Fl_Window.h>
#include <FL/Fl_Pack.h>
#include <FL/Fl_Radio_Button.h>

class ToolWindow: public Fl_Window
{
public:
  ToolWindow(int w, int h, const char* title);

  Fl_Radio_Button* line;
  Fl_Radio_Button* fill;
  Fl_Radio_Button* source;
  Fl_Radio_Button* sink;
  Fl_Radio_Button* vector;
};
