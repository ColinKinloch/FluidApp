#include <GL/glew.h>
#define USE_GLEW 1
#include <FL/gl.h>
#include <FL/Fl.h>
#include "clu.h"
#include <iostream>
#include <chrono>

#include "Settings.h"
#include "Simulation.h"

#include "SimWindow.h"
#include "ContWindow.h"
#include "ToolWindow.h"

#include "Grid.h"
#include "Lbm.h"

Grid* sim;

int window = 0;

GLsizei width = 800, height = 600;

bool fullscreen = false;
bool paused = false;

int fps = 30;
std::chrono::system_clock::time_point ticks;
std::chrono::system_clock::time_point newTicks;
//std::chrono::duration<int,std::milli> dt;

void initGL(int argc, char** argv);
void render();
void timer(int ms);

void inputKeys(unsigned char key, int x, int y);
void inputSpecial(int key, int x, int y);
void inputMouse(int button, int state, int x, int y);
void inputMotion(int x, int y);

void resize(GLsizei w, GLsizei h);
void fullToggle();
void cleanup();

void pauseCB(Fl_Widget* w);
void restartCB(Fl_Widget* w);
void proceedCB(Fl_Widget* w);
void settingsCB(Fl_Widget* w);
void toolsCB(Fl_Widget* w);

int mouseX, mouseY;
int mouseBut = 0;

float rotX = 0.0, rotY = 0.0;
float transZ = -1.f;

SimWindow* mainWin;
ToolWindow* toolWin;
ContWindow* contWin;

namespace display
{
	bool help = true;
}

int main(int argc, char** argv)
{
  settings = new Settings("./data/settings.json");
  //settings->setInt("$.width", 10);
  std::cout<<settings->getInt("$.width")<<std::endl;

  width = settings->getInt("$.width");
  height = settings->getInt("$.height");
  fps = settings->getInt("$.fps");

  //Fl::wait();

  //initGL(argc, argv);
  Fl::visual(FL_DOUBLE|FL_INDEX);
  Fl::gl_visual(FL_RGB);

  mainWin = new SimWindow(settings->getInt("$.width"),settings->getInt("$.height"),"LBM");
  toolWin = new ToolWindow(500,500,"Tools");
  contWin = new ContWindow(300,30,"Controller");


  mainWin->show(argc, argv);
  //toolWin->show();
  contWin->show();
  mainWin->draw();

  //mainWin.hello->callback(restart);

  mainWin->make_current();
  Simulation::init();
  sim = new Grid(width, height, mainWin);
  sim->createKernel(std::vector<std::string>({
                                               "data/kernels/heatmap.cl",
                                               "data/kernels/lattice.cl"
                                             }));
  sim->initData();

  Lbm* lbm = new Lbm(
    Simulation::platforms[settings->getInt("$.OpenCL.platform")],
    Simulation::devices[settings->getInt("$.OpenCL.platform")][settings->getInt("$.OpenCL.device")],
    std::vector<std::string>({
      "data/kernels/heatmap.cl",
      "data/kernels/lattice.cl"
    })
  );

  mainWin->sim = sim;
  contWin->play->callback(pauseCB);
  paused = !(contWin->play->value());
  contWin->restart->callback(restartCB);
  contWin->proceed->callback(proceedCB);
  contWin->settings->callback(settingsCB);
  contWin->tools->callback(toolsCB);

  ticks = std::chrono::system_clock::now();//glutGet(GLUT_ELAPSED_TIME);
  //glutMainLoop();
  while(mainWin->shown())
  {
    newTicks = std::chrono::system_clock::now();
    auto dt = newTicks-ticks;
    if(!paused)
     sim->step(dt.count()/1000.f);
    mainWin->make_current();
    sim->render();
    mainWin->draw();
    ticks = newTicks;
    if(!contWin->shown())
    {
      contWin->show();
    }
    Fl::check();
  }
  //return Fl::run();
}

void pauseCB(Fl_Widget* w)
{
  paused = !((Fl_Toggle_Button*)w)->value();
}
void restartCB(Fl_Widget* w)
{
  sim->restart();
}
void proceedCB(Fl_Widget* w)
{
  sim->step(1.f);
}
void settingsCB(Fl_Widget* w)
{

}
void toolsCB(Fl_Widget* w)
{
  if(((Fl_Toggle_Button*)w)->value())
  {
    toolWin->show();
  }else
  {
    toolWin->hide();
  }
}

void initGL(int argc, char** argv)
{
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitWindowSize(width, height);
	//window = glutCreateWindow("LBM");
	//glutDisplayFunc(render);
	/*glutTimerFunc(1000/fps, timer, 1000/fps);
	glutKeyboardFunc(inputKeys);
	glutSpecialFunc(inputSpecial);
	glutMouseFunc(inputMouse);
	glutMotionFunc(inputMotion);

	glutReshapeFunc(resize);*/

	glewInit();
	
	glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(errorCallbackGL, NULL); //This thing
	
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	
	//glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
}

void render()
{
	//newTicks = std::chrono::system_clock::now();//glutGet(GLUT_ELAPSED_TIME);
	auto dt = newTicks-ticks;
	//std::cout<<dt/1000.f<<std::endl;
	if(!paused)
	 sim->step(dt.count()/1000.f);
	sim->render();
	//ticks = newTicks;
	
	//glutSwapBuffers();
}

void timer(int ms)
{
	//glutTimerFunc(ms, timer, ms);
	//glutPostRedisplay();
}

void inputKeys(unsigned char key, int x, int y)
{
	//std::cout<<key<<":"<<(int)key<<std::endl;
	switch(key)
	{
		case 27: //Esc
		case 17: //Ctrl + Q
		case 3: //Ctrl + C
			cleanup(); //Quit
			break;
		case 6: //Ctrl + F
			fullToggle(); //Fullscreen
			break;
		case 'p':
		case 'P':
			paused = !paused; //Pause
			break;
		case 'r':
		case 'R':
			sim->restart();
			break;
		case 'h':
		case 'H':
			display::help = !display::help;
			break;
		case '=':
		case '+': //Next demo
			sim->nextDemo();
			break;
		case '-':
		case '_': //Previous demo
			sim->nextDemo(true);
			break;
	}
}

void inputSpecial(int key, int x, int y)
{
	switch(key)
	{
		case 1://GLUT_KEY_F11:
			fullToggle();
			break;
	}
}

void inputMouse(int button, int state, int x, int y)
{
	if(state == 1)//GLUT_DOWN)
	{
		mouseBut |= 1<<button;
	}
	else if(state == 1)//GLUT_UP)
	{
		mouseBut = 0;
	}
	
	mouseX = x;
	mouseY = y;
	inputMotion(x,y);
}

// A simple Bresenham algorithm for line drawing
void inputMotion(int x1, int y1)
{

}

void resize(GLsizei w, GLsizei h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	
	sim->resize(w, h);
	//glutPostRedisplay();
}

void fullToggle()
{
	if(fullscreen)
	{
		//glutReshapeWindow(width, height);
		fullscreen = false;
	}
	else
	{
		//glutFullScreen();
		fullscreen = true;
	}
}

void cleanup()
{
	delete sim;
	if(window)
	 ;//glutDestroyWindow(window);
}
