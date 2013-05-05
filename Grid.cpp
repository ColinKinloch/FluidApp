#include "Grid.h"

Grid::Grid(int w, int h)
{
	width = winWidth = w;
	height = winHeight = h;
	num = w*h;
	
	super();
}

void Grid::initData()
{
	D = 2, Q = 9;
	demo = 0;
	//Input horizontal velocity
	vx = Settings::root["vx"].asFloat();
	//Input vertical velocity
	vy = Settings::root["vy"].asFloat();
	//Input density ρ = rho
	rho = Settings::root["rho"].asFloat();
	//Relaxation time τ = tau
	tau = Settings::root["tau"].asFloat();
	
	//Wrapping
	hWrap = Settings::root["wrap"]["horizontal"].asBool();
	vWrap = Settings::root["wrap"]["vertical"].asBool();
	
	lattice.resize(Q*num);
	solid.resize(2);
	for(int i=0; i<solid.size(); ++i)
	 solid[i].resize(num);
	
	float w0 = 4.f/9.f;
	float w1 = 1.f/9.f;
	float w2 = 1.f/36.f;
	
	float vSq = 1.5f*(vx*vx+vy*vy);
	
	//Fill lattice with equilibrium velocities
	for(int i=0; i<num; ++i)
	{
		int j=i*Q;
		lattice[j]	 = w0*rho*(1.f																			-vSq);
		lattice[j+1] = w1*rho*(1.f+3.f*vx				+4.5f*vx*vx							-vSq);
		lattice[j+2] = w1*rho*(1.f+3.f*vy				+4.5f*vy*vy							-vSq);
		lattice[j+3] = w1*rho*(1.f-3.f*vx				+4.5f*vx*vx							-vSq);
		lattice[j+4] = w1*rho*(1.f-3.f*vy				+4.5f*vy*vy							-vSq);
		lattice[j+5] = w2*rho*(1.f+3.f*( vx+vy)	+4.5f*( vx+vy)*( vx+vy)	-vSq);
		lattice[j+6] = w2*rho*(1.f+3.f*(-vx+vy)	+4.5f*(-vx+vy)*(-vx+vy)	-vSq);
		lattice[j+7] = w2*rho*(1.f+3.f*(-vx-vy)	+4.5f*(-vx-vy)*(-vx-vy)	-vSq);
		lattice[j+8] = w2*rho*(1.f+3.f*( vx-vy)	+4.5f*( vx-vy)*( vx-vy)	-vSq);
		solid[0][i] = false;
		solid[1][i] = true;
	}
	
	
	
	lSize = Q*num*sizeof(float);
	size_t size = num*sizeof(float);
	
	glGenRenderbuffers(1, &rendBuff);
	glBindRenderbuffer(GL_RENDERBUFFER, rendBuff);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, width, height);
	
	glGenFramebuffers(1, &frameBuff);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rendBuff);
	
	GLenum hey = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	std::cout<<"Framebuffer:";
	switch(hey)
	{
		case GL_FRAMEBUFFER_COMPLETE:												std::cout<<"complete"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:					std::cout<<"incomplete attachment"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	std::cout<<"incomplete missing attachment"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:					std::cout<<"incomplete draw buffer"; break;
		default:																						std::cout<<"unknown"; break;
	}
	std::cout<<std::endl;
	glFinish();
	
	clVBOs.push_back(cl::BufferRenderGL(context, CL_MEM_READ_WRITE, rendBuff));
	
	clLattice.push_back(cl::Buffer(context, CL_MEM_WRITE_ONLY, lSize));
	clLattice.push_back(cl::Buffer(context, CL_MEM_WRITE_ONLY, lSize));
	clVelMag = cl::Buffer(context, CL_MEM_WRITE_ONLY, size);
	for(int i=0; i<solid.size(); ++i)
	 clSolid.push_back(cl::Buffer(context, CL_MEM_WRITE_ONLY, num*sizeof(char)));
	
	try
	{
		queue.enqueueWriteBuffer(clLattice[0], CL_TRUE, 0, lSize, &lattice[0], NULL, &event);
		queue.enqueueWriteBuffer(clLattice[1], CL_TRUE, 0, lSize, &lattice[0], NULL, &event);
		for(int i=0; i<solid.size(); i++)
		 queue.enqueueWriteBuffer(clSolid[i], CL_TRUE, 0, num*sizeof(char), &(solid[i])[0], NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: initData", e);
	}
	
	//init kernels
	try
	{
		
		clStream = cl::Kernel(program, "stream");
		clHWrap = cl::Kernel(program, "hWrap");
		clVWrap = cl::Kernel(program, "vWrap");
		clSolidBB = cl::Kernel(program, "solidBB");
		clInflow = cl::Kernel(program, "inflow");
		clCollide = cl::Kernel(program, "collide");
		clRender = cl::Kernel(program, "render");
		
	}
	catch(cl::Error e)
	{
		cluErr("Grid: initData", e);
	}
	
	try
	{
		
		clStream.setArg(2, width);
		clStream.setArg(3, height);
		
		clHWrap.setArg(1, width);
		clHWrap.setArg(2, height);
		clVWrap.setArg(1, width);
		clVWrap.setArg(2, height);
		
		clSolidBB.setArg(2, width);
		
		clInflow.setArg(1, vx);
		clInflow.setArg(2, rho);
		clInflow.setArg(3, width);
		
		clCollide.setArg(1, clVelMag);
		clCollide.setArg(2, width);
		clCollide.setArg(3, tau);
		
		clRender.setArg(0, clVelMag);
		clRender.setArg(2, width);
		clRender.setArg(3, height);
		clRender.setArg(4, clVBOs[0]);
		
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: initData", e);
	}
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuff);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Grid::restart()
{
	for(int j=0; j<solid.size(); ++j)
	{
		for(int i=0; i<num; ++i)
		{
			solid[j][i] = false;
		}
	}
	queue.enqueueWriteBuffer(clLattice[0], CL_TRUE, 0, lSize, &lattice[0], NULL, &event);
	queue.enqueueWriteBuffer(clLattice[1], CL_TRUE, 0, lSize, &lattice[0], NULL, &event);
	queue.finish();
}

void Grid::step(float dt)
{
	glFinish();
	
	
	try
	{
		queue.enqueueWriteBuffer(clSolid[demo], CL_TRUE, 0, num*sizeof(char), &(solid[demo])[0], NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: step: GLO", e);
	}
	
	clStream.setArg(0, clLattice[!odd]);
	clStream.setArg(1, clLattice[odd]);
	clStream.setArg(4, dt);
	
	clCollide.setArg(0, clLattice[odd]);
	
	clHWrap.setArg(0, clLattice[odd]);
	clVWrap.setArg(0, clLattice[odd]);
	
	clSolidBB.setArg(0, clLattice[odd]);
	clSolidBB.setArg(1, clSolid[demo]);
	
	clInflow.setArg(0, clLattice[odd]);
	
	
	try
	{
		//*
		queue.enqueueNDRangeKernel(clStream, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		//*
		if(hWrap)
		 queue.enqueueNDRangeKernel(clHWrap, cl::NullRange,
		  cl::NDRange(height), cl::NullRange, NULL, &event);
		if(vWrap)
		 queue.enqueueNDRangeKernel(clVWrap, cl::NullRange,
		  cl::NDRange(width), cl::NullRange, NULL, &event);
		queue.enqueueNDRangeKernel(clSolidBB, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		//queue.enqueueNDRangeKernel(clInflow, cl::NullRange,
		// cl::NDRange(height), cl::NullRange, NULL, &event);
		//*
		//*
		queue.enqueueNDRangeKernel(clCollide, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		
		
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: step: NDR", e);
	}
	
	odd = !odd;
}

void Grid::render()
{
	
	clRender.setArg(1, clSolid[demo]);
	try
	{
		queue.enqueueWriteBuffer(clSolid[demo], CL_TRUE, 0, num*sizeof(char), &(solid[demo])[0], NULL, &event);
		queue.enqueueAcquireGLObjects(&clVBOs, NULL, &event);
		queue.enqueueNDRangeKernel(clRender, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		queue.enqueueReleaseGLObjects(&clVBOs, NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: step: RGLO", e);
	}
	glBlitFramebuffer(
	 width, height, 0, 0,
	 winWidth, winHeight, 0, 0,
	 GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


void Grid::nextDemo(bool previous)
{
	if(previous&&demo!=0)
	 demo--;
	else if(!previous&&demo<(solid.size()-1))
	 demo++;
}

void Grid::resize(int w, int h)
{
	winWidth = w;
	winHeight = h;
}

void Grid::draw(int x, int y, bool erase)
{
	float w = (float)width/winWidth;
	float h = (float)height/winHeight;
	int sx = w*x, sy = h*y;
	
	if(sx<0||sx>width-1||sy<0||y>height-1)
	 return;
	
	int i = (sy*width+sx);
	solid[demo][i] = !erase;
	
}
