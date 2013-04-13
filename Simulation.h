#ifndef CKFD_SIMULATION
#define CKFD_SIMULATION

#include <GL/glew.h>
#include <GL/glx.h>
#include <glm/glm.hpp>
#include "clu.h"
#include "Settings.h"

class Simulation
{
	public:
		static std::vector<cl::Platform> platforms;
		static std::vector<std::vector<cl::Device> > devices;
		
		static void init();
		
		
		int p;
		int d;
		
		cl::Context context;
		cl::CommandQueue queue;
		cl::Program program;
		cl::Kernel kernel;
		cl::Event event;
		
		Simulation();
		~Simulation();
		
		void createProgram(std::string path);
		
		virtual void initData();
		virtual void step();
		virtual void render();
};

#endif//CKFD_SIMULATION
