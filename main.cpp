#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "Settings.h"
#include "Simulation.h"

#include "Particles.h"

Simulation* sim;

int window = 0;

GLsizei width = 800, height = 600;
bool fullscreen = false;

float transZ = -1.f;

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

main(int argc, char** argv)
{
	Settings::loadFromFile("data/settings.json");
	
	initGL(argc, argv);
	
	Simulation::init();
	sim = new Particles();
	sim->createProgram("data/kernels/part2.cl");
	sim->initData();
	
	glutMainLoop();
}

void initGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	window = glutCreateWindow("GLUT Artifact");
	
	glutDisplayFunc(render);
	glutTimerFunc(30, timer, 30);
	glutKeyboardFunc(inputKeys);
	glutSpecialFunc(inputSpecial);
	glutMouseFunc(inputMouse);
	glutMotionFunc(inputMotion);
	
	glutCreateMenu();
	glutReshapeFunc(resize);
	
	glewInit();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_DEPTH_TEST);
	
	glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat)width/(GLfloat)height, 0.1, 1000.0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, transZ);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	sim->step();
	sim->render();
	
	glutSwapBuffers();
}

void timer(int ms)
{
	glutTimerFunc(ms, timer, ms);
	glutPostRedisplay();
}

void inputKeys(unsigned char key, int x, int y)
{
	std::cout<<key<<":"<<(int)key<<std::endl;
	switch(key)
	{
		//Quit
		case 27: //Esc
		case 17: //Ctrl + Q
		case 3: //Ctrl + C
			cleanup();
			break;
		case 6:
			fullToggle();
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
	//handle mouse interaction for rotating/zooming the view
    if (state == GLUT_DOWN) {
        mouseBut |= 1<<button;
    } else if (state == GLUT_UP) {
        mouseBut = 0;
    }

    mouseX = x;
    mouseY = y;
}
void inputMotion(int x, int y)
{
	//hanlde the mouse motion for zooming and rotating the view
    float dx, dy;
    dx = x - mouseX;
    dy = y - mouseY;

    if (mouseBut & 1) {
        rotX += dy * 0.2;
        rotY += dx * 0.2;
    } else if (mouseBut & 4) {
        transZ += dy * 0.1;
    }

    mouseX = x;
    mouseY = y;

    // set view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, transZ);
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
}

void resize(GLsizei w, GLsizei h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat)width/(GLfloat)height, 0.1, 1000.0);
	
	glutPostRedisplay();
}

void fullToggle()
{
	if(fullscreen)
	{
		glutReshapeWindow(800, 600);
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
