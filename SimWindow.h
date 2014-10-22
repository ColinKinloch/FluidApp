

#pragma once

#include <GL/glew.h>
#include <FL/gl.h>
#include <FL/Fl_Button.h>
#include "Simulation.h"

class SimWindow: public Fl_Gl_Window
{
public:
  SimWindow(int w, int h, const char* title);
  SimWindow(int x, int y, int w, int h, const char* title);

  Simulation* sim;

  int mouseX, mouseY;

  void draw();
  void make_valid();
  int handle(int event);
};
