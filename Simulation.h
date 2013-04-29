#ifndef CKFD_SIMULATION
#define CKFD_SIMULATION

#include <GL/glew.h>
#include <GL/glx.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "clu.h"
#include "Settings.h"

class Simulation
{
	public:
		static std::vector<cl::Platform> platforms;
		static std::vector<std::vector<cl::Device> > devices;
		
		static void init();
		
		GLuint vertShader;
		GLuint fragShader;
		GLuint shader;
		
		glm::mat4 projMat;
		glm::mat4 modelMat;
		
		void updateMatrix();
		void createShader(std::string vertexPath, std::string fragmentPath);
		
		int p;
		int d;
		
		cl::Context context;
		cl::CommandQueue queue;
		cl::Program program;
		cl::Event event;
		
		Simulation();
		~Simulation();
		
		void createKernel(std::string path);
		void restart();
		
		virtual void initData();
		virtual void step(float dt);
		virtual void render();
		virtual void resize(int h, int w);
		virtual void draw(int x, int y, bool erase = false);
};

#endif//CKFD_SIMULATION
