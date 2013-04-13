#include "Particles.h"

Particles::Particles()
{
	p = Settings::root["OpenCL"]["platform"].asInt();
	d = Settings::root["OpenCL"]["device"].asInt();
	
	cl_context_properties cProps[] = {
	 CL_CONTEXT_PLATFORM, (cl_context_properties) (platforms[p])(),
	 CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
	 CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
	 0};
	cl_command_queue_properties cqProps[] = {
	 //CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
	 0};
	
	try
	{
		context = cl::Context(CL_DEVICE_TYPE_GPU, cProps);
		queue = cl::CommandQueue(context, devices[p][d]);
	}
	catch(cl::Error e)
	{
		cluErr("Particles", e);
	}
}

void Particles::initData()
{
	num = 1000000;
	std::vector<glm::vec4> pos(num);
	std::vector<glm::vec4> vel(num);
	std::vector<glm::vec4> col(num);
	
	for(int i=0; i<num; i++)
	{
		float rad = frand(.2, .5);
		float x = rad*sin(2*3.14*i/num);
		float z = frand(0, 1);
		float y = rad*cos(2*3.14*i/num);
		pos[i] = glm::vec4(x, y, z, 1.0f);
		
		float life_r = frand(0.f, 1.f);
		vel[i] = glm::vec4(0.0, 0.0, 3.0f, life_r);
		
		col[i] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
	size_t size = num*sizeof(glm::vec4);
	
	posVBO = cluCreateGLVBO(&pos[0], size, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	colVBO = cluCreateGLVBO(&col[0], size, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	
	glFinish();
	
	clVBOs.push_back(cl::BufferGL(context, CL_MEM_READ_WRITE, posVBO));
	clVBOs.push_back(cl::BufferGL(context, CL_MEM_READ_WRITE, colVBO));
	
	
	clVel = cl::Buffer(context, CL_MEM_WRITE_ONLY, size);
	clPosGen = cl::Buffer(context, CL_MEM_WRITE_ONLY, size);
	clVelGen = cl::Buffer(context, CL_MEM_WRITE_ONLY, size);
	
	try
	{
		queue.enqueueWriteBuffer(clVel, CL_TRUE, 0, size, &vel[0], NULL, &event);
		queue.enqueueWriteBuffer(clPosGen, CL_TRUE, 0, size, &pos[0], NULL, &event);
		queue.enqueueWriteBuffer(clVelGen, CL_TRUE, 0, size, &vel[0], NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Particles: initData", e);
	}
	
	//init kernel
	try
	{
		kernel = cl::Kernel(program, "part2");
	}
	catch(cl::Error e)
	{
		cluErr("Particles: initData", e);
	}
	
	try
	{
		kernel.setArg(0, clVBOs[0]);
		kernel.setArg(1, clVBOs[1]);
		kernel.setArg(2, clVel);
		kernel.setArg(3, clPosGen);
		kernel.setArg(4, clVelGen);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Particles: initData", e);
	}
}

void Particles::step()
{
	glFinish();
	
	try
	{
		queue.enqueueAcquireGLObjects(&clVBOs, NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Particles: step", e);
	}
	
	float dt = .01f;
	kernel.setArg(5, dt);
	
	try
	{
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(num), cl::NullRange, NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Particles: step", e);
	}
	
	try
	{
		queue.enqueueReleaseGLObjects(&clVBOs, NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Particles: step", e);
	}
}

void Particles::render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.);
	
	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glVertexPointer(4, GL_FLOAT, 0, 0);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	
	glDrawArrays(GL_POINTS, 0, num);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

float Particles::frand(float min, float max)
{
	float r = random()/(float)RAND_MAX;
	return min+(max-min)*r;
}
