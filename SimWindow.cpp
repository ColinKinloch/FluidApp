#include <GL/glew.h>
#define USE_GLEW 1
#include <FL/gl.h>
#include <FL/Fl.h>
#include "SimWindow.h"
#include <iostream>

SimWindow::SimWindow(int w, int h, const char* title): Fl_Gl_Window(w,h,title)
{
  begin();
    resizable(this);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    //glutInitWindowSize(w, h);
    //glutCreateWindow(title);
  end();
}
SimWindow::SimWindow(int x, int y, int w, int h, const char* title): Fl_Gl_Window(x,y,w,h,title)
{
  begin();

    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    //glutInitWindowSize(w, h);
    //glutCreateWindow(title);
  end();
}

void SimWindow::draw()
{
  make_current();
  if(!context_valid())
  {
    std::cout<<w()<<"x"<<h()<<std::endl;
    //if(sim->inited)
      //sim->resize(w(), h());
    GLenum err = glewInit();
    if(GLEW_OK != err)
      std::cout<<"glew init error: "<<glewGetErrorString(err)<<std::endl;
    glViewport(0,0,h(),w());

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    //risize
  }else if(!valid())
  {
    glViewport(0,0,h(),w());
    sim->resize(w(), h());
  }
  //glClearColor(distribution(generator),distribution(generator),distribution(generator),0);
  //glClear(GL_COLOR_BUFFER_BIT);
  swap_buffers();
}
int SimWindow::handle(int event)
{
  switch(event) {
    case FL_MOVE:
    case FL_PUSH:
    case FL_DRAG:
      {
        cursor(FL_CURSOR_CROSS);
        int x1 = Fl::event_x();
        int y1 = Fl::event_y();
        bool leftBut = Fl::event_state(FL_BUTTON1);
        bool rightBut = Fl::event_state(FL_BUTTON3);

        float dx, dy, sx, sy;
        int x0 = mouseX, y0 = mouseY;
        dx = abs(x1-x0);
        dy = abs(y1-y0);
        if(x0<x1) sx=1; else sx=-1;
        if(y0<y1) sy=1; else sy=-1;
        float err = dx-dy;
        while(1)
        {
                if(leftBut)
                {
                        sim->draw(x0, y0);
                        sim->draw(x0-1, y0);
                        sim->draw(x0, y0-1);
                        sim->draw(x0, y0+1);
                        sim->draw(x0+1, y0);
                }
                else if(rightBut)
                {
                        sim->draw(x0, y0, true);
                        sim->draw(x0-1, y0, true);
                        sim->draw(x0, y0-1, true);
                        sim->draw(x0, y0+1, true);
                        sim->draw(x0+1, y0,true);
                }
                if(x0==x1&&y0==y1)
                 break;
                float e2 = 2*err;
                if(e2>-dy)
                {
                        err = err-dy;
                        x0 = x0+sx;
                }
                if(e2<dx)
                {
                        err = err+dx;
                        y0 = y0+sy;
                }
        }
        mouseX = x1;
        mouseY = y1;
      }
      return 1;
    case FL_RELEASE:
      return 1;
    case FL_FOCUS :
    case FL_UNFOCUS :
      return 1;
    case FL_KEYBOARD:
      return 1;
    case FL_SHORTCUT:
      return 1;
    default:
      return Fl_Gl_Window::handle(event);
  }
}
