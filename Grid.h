#ifndef CKFD_PARTICLES
#define CKFD_PARTICLES

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <boost/multi_array.hpp>
#include "Simulation.h"

class Grid: public Simulation
{
		typedef Simulation super;
	public:
		std::vector<cl::Memory> clVBOs;
		std::vector<cl::Buffer> clLattice;
		cl::Buffer clVelMag;
		cl::Buffer clSolid;
		cl::Buffer clCMap;
		
		cl::Kernel clRender;
		cl::Kernel clStream;
		cl::Kernel clCollide;
		cl::Kernel clWrap;
		cl::Kernel clSolidBC;
		
		bool odd = false;
		
		int latticeVBO;
		GLuint rendBuff;
		GLuint frameBuff;
		std::vector<char> solid;
		
		int width;
		int height;
		int winHeight;
		int winWidth;
		int D;
		int Q;
		float tau;
		int num;
		
		Grid(int width, int height);
		
		void initData();
		void step(float dt);
		void render();
		void resize(int w, int h);
		void draw(int x, int y, bool erase = false);
};

#endif//CKFD_PARTICLES
