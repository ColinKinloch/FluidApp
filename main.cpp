#include <GL/glew.h>
#if defined __APPLE__ || defined(MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
//#include <GL/freeglut.h>
#include "clu.h"
#include <iostream>

#include "Settings.h"
#include "Simulation.h"

#include "Grid.h"

Grid* sim;

int window = 0;

GLsizei width = 800, height = 600;

bool fullscreen = false;
bool paused = false;

int fps = 30;
int ticks;
int newTicks;
int dt;

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

int mouseX, mouseY;
int mouseBut = 0;

float rotX = 0.0, rotY = 0.0;
float transZ = -1.f;

namespace display
{
	bool help = true;
}

int main(int argc, char** argv)
{
	Settings::loadFromFile("data/settings.json");
	
	width = Settings::root["width"].asInt();
	height = Settings::root["height"].asInt();
	fps = Settings::root["fps"].asInt();
	
	initGL(argc, argv);
	
	Simulation::init();
	sim = new Grid(width, height);
	sim->createKernel("data/kernels/lattice.cl");
	sim->initData();
	
	ticks = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
	return 1;
}

void initGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	window = glutCreateWindow("LBM");
	glutDisplayFunc(render);
	glutTimerFunc(1000/fps, timer, 1000/fps);
	glutKeyboardFunc(inputKeys);
	glutSpecialFunc(inputSpecial);
	glutMouseFunc(inputMouse);
	glutMotionFunc(inputMotion);
	
	glutReshapeFunc(resize);
	
	glewInit();
	
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(errorCallbackGL, NULL); //This thing
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
}

void render()
{
	newTicks = glutGet(GLUT_ELAPSED_TIME);
	dt = newTicks-ticks;
	//std::cout<<dt/1000.f<<std::endl;
	if(!paused)
	 sim->step(dt/1000.f);
	sim->render();
	ticks = newTicks;
	
	glutSwapBuffers();
}

void timer(int ms)
{
	glutTimerFunc(ms, timer, ms);
	glutPostRedisplay();
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
		case GLUT_KEY_F11:
			fullToggle();
			break;
	}
}

void inputMouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		mouseBut |= 1<<button;
	}
	else if(state == GLUT_UP)
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
	float dx, dy, sx, sy;
	int x0 = mouseX, y0 = mouseY;
	dx = abs(x1-x0);
	dy = abs(y1-y0);
	if(x0<x1) sx=1; else sx=-1;
	if(y0<y1) sy=1; else sy=-1;
	float err = dx-dy;
	while(1)
	{
		if(mouseBut & 1)
		{
			sim->draw(x0, y0);
			sim->draw(x0-1, y0);
			sim->draw(x0, y0-1);
			sim->draw(x0, y0+1);
			sim->draw(x0+1, y0);
		}
		else if(mouseBut & 4)
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

void resize(GLsizei w, GLsizei h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	
	sim->resize(w, h);
	glutPostRedisplay();
}

void fullToggle()
{
	if(fullscreen)
	{
		glutReshapeWindow(width, height);
		fullscreen = false;
	}
	else
	{
		glutFullScreen();
		fullscreen = true;
	}
}

void cleanup()
{
	delete sim;
	if(window)
	 glutDestroyWindow(window);
}
