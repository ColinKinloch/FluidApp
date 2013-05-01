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
	tau = Settings::root["tau"].asFloat();
	std::vector<float> lattice(Q*num);
	solid.resize(num);
	
	float roout = 1.f;
	float vxin = 0.04f;
	
	float w0 = 4.f/9.f;
	float w1 = 1.f/9.f;
	float w2 = 1.f/36.f;
	
	for(int i=0; i<num; ++i)
	{
		int j=i*Q;
		lattice[j]	 = w0*roout*(1.f												-1.5f*vxin*vxin);
		lattice[j+1] = w1*roout*(1.f+3.f*vxin+4.5f*vxin*vxin-1.5f*vxin*vxin);
		lattice[j+2] = w1*roout*(1.f												-1.5f*vxin*vxin);
		lattice[j+3] = w1*roout*(1.f-3.f*vxin+4.5f*vxin*vxin-1.5f*vxin*vxin);
		lattice[j+4] = w1*roout*(1.f												-1.5f*vxin*vxin);
		lattice[j+5] = w2*roout*(1.f+3.f*vxin+4.5f*vxin*vxin-1.5f*vxin*vxin);
		lattice[j+6] = w2*roout*(1.f-3.f*vxin+4.5f*vxin*vxin-1.5f*vxin*vxin);
		lattice[j+7] = w2*roout*(1.f-3.f*vxin+4.5f*vxin*vxin-1.5f*vxin*vxin);
		lattice[j+8] = w2*roout*(1.f+3.f*vxin+4.5f*vxin*vxin-1.5f*vxin*vxin);
		//lattice[i+8] = 0.f;
	 solid[i] = false;
	}
	
	
	//Stupid color map!
	int ncol;
	float rcol, gcol, bcol;
	FILE* fp_col = fopen("./data/cmap.dat", "r");
	if(fp_col==NULL)
	{
		printf("Error: cant find cmap.dat\n");
		return;
	}
	fscanf(fp_col, "%d", &ncol);
	std::vector<float> cmap_rgba(ncol*4);
	for(int i=0; i<ncol; ++i)
	{
		int j = i*4;
		fscanf(fp_col, "%f%f%f", &rcol, &gcol, &bcol);
		cmap_rgba[j]  =((float)(1.f));
		cmap_rgba[j+1]=((float)(rcol));
		cmap_rgba[j+2]=((float)(gcol));
		cmap_rgba[j+3]=((float)(bcol));
	}
	fclose(fp_col);
	
	size_t lSize = Q*num*sizeof(float);
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
	clSolid = cl::Buffer(context, CL_MEM_WRITE_ONLY, num*sizeof(char));
	clCMap = cl::Buffer(context, CL_MEM_WRITE_ONLY, ncol*sizeof(unsigned int)*4);
	
	try
	{
		queue.enqueueWriteBuffer(clLattice[0], CL_TRUE, 0, lSize, &lattice[0], NULL, &event);
		queue.enqueueWriteBuffer(clLattice[1], CL_TRUE, 0, lSize, &lattice[0], NULL, &event);
		queue.enqueueWriteBuffer(clSolid, CL_TRUE, 0, num*sizeof(char), &solid[0], NULL, &event);
		queue.enqueueWriteBuffer(clCMap, CL_TRUE, 0, ncol*sizeof(unsigned int)*4, &cmap_rgba[0], NULL, &event);
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
		clWrap = cl::Kernel(program, "wrap");
		clSolidBC = cl::Kernel(program, "solidBC");
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
		
		clWrap.setArg(1, width);
		clWrap.setArg(2, height);
		
		clSolidBC.setArg(1, clSolid);
		clSolidBC.setArg(2, width);
		
		clCollide.setArg(1, clVelMag);
		clCollide.setArg(2, width);
		clCollide.setArg(3, tau);
		
		clRender.setArg(0, clVelMag);
		clRender.setArg(1, clSolid);
		clRender.setArg(2, width);
		clRender.setArg(3, height);
		clRender.setArg(4, clVBOs[0]);
		clRender.setArg(5, clCMap);
		
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: initData", e);
	}
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuff);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Grid::step(float dt)
{
	glFinish();
	
	
	try
	{
		queue.enqueueWriteBuffer(clSolid, CL_TRUE, 0, num*sizeof(char), &solid[0], NULL, &event);
		queue.enqueueAcquireGLObjects(&clVBOs, NULL, &event);
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
	
	clWrap.setArg(0, clLattice[odd]);
	
	clSolidBC.setArg(0, clLattice[odd]);
	
	
	try
	{
		//*
		queue.enqueueNDRangeKernel(clStream, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		//*
		queue.enqueueNDRangeKernel(clSolidBC, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		//*
		queue.enqueueNDRangeKernel(clWrap, cl::NullRange,
		 cl::NDRange(width), cl::NullRange, NULL, &event);
		//*
		queue.enqueueNDRangeKernel(clCollide, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		
		queue.enqueueNDRangeKernel(clRender, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: step: NDR", e);
	}
	
	try
	{
		queue.enqueueReleaseGLObjects(&clVBOs, NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: step: RGLO", e);
	}
	
	odd = !odd;
}

void Grid::render()
{
	glBlitFramebuffer(
	 width, height, 0, 0,
	 winWidth, winHeight, 0, 0,
	 GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
	
	int i = (sy*width+sx);
	
	solid[i] = !erase;
	
}
