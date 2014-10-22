//The grid class runs the lattice Boltzman simulation

#pragma once

#include "Simulation.h"

#define LTYPE float

class Grid: public Simulation
{
		typedef Simulation super;
	public:
		std::vector<cl::Memory> clVBOs;
		std::vector<cl::Buffer> clLattice;
		cl::Buffer clVelMag;
		std::vector<cl::Buffer> clSolid;
		cl::Buffer clCMap;
		
		cl::Kernel clStream;
		cl::Kernel clHWrap;
		cl::Kernel clVWrap;
		cl::Kernel clSolidBB;
		cl::Kernel clInflow;
		cl::Kernel clCollide;
		cl::Kernel clRender;
		
		bool odd = false;
		int demo = 0;
		
		GLuint rendBuff;
		GLuint frameBuff;
		std::vector<LTYPE> lattice;
		std::vector< std::vector<char> > solid;
		
		int width;
		int height;
		int winHeight;
		int winWidth;
		int D;
		int Q;
		bool hWrap;
		bool vWrap;
		float tau;
		float vx;
		float vy;
		float rho;
		int num;
		size_t lSize;
		
		Grid(int width, int height, Fl_Gl_Window* wind);
		
		void nextDemo(bool previous = false);
		
		void initData();
		void step(float dt);
		void render();
		void restart();
		void resize(int w, int h);
		void draw(int x, int y, bool erase = false);
};
