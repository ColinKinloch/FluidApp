

#pragma once

#include <FL/Fl_Button.h>
#include <FL/Fl_Toggle_Button.h>
#include <FL/Fl_Window.h>

class ContWindow: public Fl_Window
{
public:
  ContWindow(int w, int h, const char* title);

  Fl_Button* restart;
  Fl_Toggle_Button* play;
  Fl_Button* proceed;
  Fl_Toggle_Button* tools;
  Fl_Toggle_Button* settings;


};
