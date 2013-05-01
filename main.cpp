#include <GL/glew.h>
#include <GL/freeglut.h>
#include "clu.h"
#include <iostream>

#include "Settings.h"
#include "Simulation.h"

#include "Grid.h"

Simulation* sim;

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
void renderText();
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
	
	//glutCreateMenu();
	glutReshapeFunc(resize);
	
	glewInit();
	
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(errorCallbackGL, NULL); //This thing
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
}

void render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//sim->updateMatrix();
	newTicks = glutGet(GLUT_ELAPSED_TIME);
	dt = newTicks-ticks;
	//std::cout<<dt/1000.f<<std::endl;
	if(!paused)
	 sim->step(dt/1000.f);
	//renderText();
	sim->render();
	ticks = newTicks;
	
	glutSwapBuffers();
}

void renderText()
{
	glUseProgram(0);
	std::string text = "\n";
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glRasterPos2f(-1, 1);
	if(display::help)
	 text+="H: hide this help\nEsc: Quit\nP: paused";
	glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)text.c_str());
	//sim->updateMatrix();
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
			paused = !paused;
			break;
		case 'r':
		case 'R':
			sim->restart();
			break;
		case 'h':
		case 'H':
			display::help = !display::help;
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

void inputMotion(int x, int y)
{
	float dx, dy;
	dx = x - mouseX;
	dy = y - mouseY;
	
	if(mouseBut & 1)
	{
		sim->draw(x, y);
	}
	else if(mouseBut & 4)
	{
		sim->draw(x, y, true);
	}
	mouseX = x;
	mouseY = y;
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
	glutLeaveMainLoop();
}
