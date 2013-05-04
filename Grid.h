#ifndef CKFD_PARTICLES
#define CKFD_PARTICLES

#include "Simulation.h"

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
		std::vector<float> lattice;
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
		
		Grid(int width, int height);
		
		void nextDemo(bool previous = false);
		
		void initData();
		void step(float dt);
		void render();
		void restart();
		void resize(int w, int h);
		void draw(int x, int y, bool erase = false);
};

#endif//CKFD_PARTICLES
