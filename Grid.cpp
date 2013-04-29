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
	D = 2, Q = 9, tau = Settings::root["tau"].asFloat();
	std::vector<float> lattice(Q*num);
	solid.resize(num);
	
	for(int i=0; i<num; ++i)
	 solid[i] = false;
	/*std::vector<glm::vec2> pos(num);
	std::vector<glm::vec4> vel(num);
	std::vector<glm::vec4> col(num);
	*/
	for(int i=0; i<Q*num; ++i)
	{
		lattice[i] = 1.f;
	}
		/*if(x==y)
		 lattice[i*Q+1] = !lattice[i*Q+1];*/
	
	/*
	for(int x=0; x<width; ++x)
	{
		for(int y=0; y<height; ++y)
		{
			for(int q=0; q<Q; ++q)
			{
				int i=(Q*(y*width)+(x+q));
				if(q == 0)
				{
					if((x+y)%2)
					 lattice[i] = 1.f;
					else
					 lattice[i] = 0.f;
				}
				else
				{
					lattice[i] = 0.5f;
				}
			}
		}
	}
	/*
	for(int i=0; i<num; i++)
	{
		int x = i%width;
		pos[i] = glm::vec2(glm::diskRand(1.f));
		
		vel[i] = glm::vec4(glm::sphericalRand(1.0f), glm::linearRand(0.f, 1.f));
		
		col[i] = glm::vec4(glm::sphericalRand(1.0f), 1);
		
	}*/
	
	//size_t sizev4 = num*sizeof(glm::vec4);
	//size_t sizev2 = num*sizeof(glm::vec2);
	size_t sizef = Q*num*sizeof(float);
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
	//posVBO = cluCreateGLVBO(&pos[0], sizev2, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	//colVBO = cluCreateGLVBO(&col[0], size, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	latticeVBO = cluCreateGLVBO(&lattice[0], sizef, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	/*GLuint checkerBoard[num];
	
	
	for(int i=0; i<num; ++i)
	{
		GLubyte* colors = (GLubyte*)&checkerBoard[i];
		if(i/128 & 16 ^ i % 128 & 16)
		{
			colors[0] = 0xFF;
			colors[1] = 0xFF;
			colors[2] = 0xFF;
			colors[3] = 0xFF;
		}
		else
		{
			colors[0] = 0xFF;
			colors[1] = 0x00;
			colors[2] = 0x00;
			colors[3] = 0xFF;
		}
	}
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerBoard);
	
	glm::vec2 vertCoord[4];
	glm::vec2 texCoord[4];
	GLuint indices[4];
	
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	
	vertCoord[0] = glm::vec2(0.f, 0.f);
	vertCoord[1] = glm::vec2(1.f, 0.f);
	vertCoord[2] = glm::vec2(1.f, 1.f);
	vertCoord[3] = glm::vec2(0.f, 1.f);
	
	texCoord[0] = glm::vec2(0.f, 0.f);
	texCoord[1] = glm::vec2(1.f, 0.f);
	texCoord[2] = glm::vec2(1.f, 1.f);
	texCoord[3] = glm::vec2(0.f, 1.f);
	
	glGenBuffers(1, &vertCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(glm::vec2), vertCoord, GL_STATIC_DRAW);
	
	glGenBuffers(1, &texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, 4*sizeof(glm::vec2), texCoord, GL_STATIC_DRAW);
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indices, GL_STATIC_DRAW);
	*/
	glFinish();
	
	
	//clVBOs.push_back(cl::BufferGL(context, CL_MEM_READ_WRITE, posVBO));
	//clVBOs.push_back(cl::BufferGL(context, CL_MEM_READ_WRITE, colVBO));
	//clVBOs.push_back(cl::BufferGL(context, CL_MEM_READ_WRITE, latticeVBO));
	clVBOs.push_back(cl::BufferRenderGL(context, CL_MEM_READ_WRITE, rendBuff));
	
	//clVel = cl::Buffer(context, CL_MEM_WRITE_ONLY, size);
	//clPosGen = cl::Buffer(context, CL_MEM_WRITE_ONLY, siz);
	//clVelGen = cl::Buffer(context, CL_MEM_WRITE_ONLY, size);
	clLattice = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizef);
	clTmpLattice = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizef);
	clVelMag = cl::Buffer(context, CL_MEM_WRITE_ONLY, size);
	clSolid = cl::Buffer(context, CL_MEM_WRITE_ONLY, num*sizeof(char));
	
	try
	{
		//queue.enqueueWriteBuffer(clVel, CL_TRUE, 0, size, &vel[0], NULL, &event);
		//queue.enqueueWriteBuffer(clPosGen, CL_TRUE, 0, sizev2, &pos[0], NULL, &event);
		//queue.enqueueWriteBuffer(clVelGen, CL_TRUE, 0, size, &vel[0], NULL, &event);
		queue.enqueueWriteBuffer(clLattice, CL_TRUE, 0, sizef, &lattice[0], NULL, &event);
		//queue.enqueueWriteBuffer(clTmpLattice, CL_TRUE, 0, sizef, &lattice[0], NULL, &event);
		queue.enqueueWriteBuffer(clSolid, CL_TRUE, 0, num*sizeof(char), &solid[0], NULL, &event);
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: initData", e);
	}
	
	//init kernel
	try
	{
		
		clRender = cl::Kernel(program, "render");
		clStream = cl::Kernel(program, "stream");
		clCollide = cl::Kernel(program, "collide");
		clSolidBC = cl::Kernel(program, "solidBC");
	}
	catch(cl::Error e)
	{
		cluErr("Grid: initData", e);
	}
	
	try
	{
		clRender.setArg(1, clSolid);
		clRender.setArg(2, width);
		clRender.setArg(3, height);
		clRender.setArg(4, clVBOs[0]);
		
		clStream.setArg(2, width);
		clStream.setArg(3, height);
		
		clCollide.setArg(1, clVelMag);
		clCollide.setArg(2, width);
		clCollide.setArg(3, tau);
		
		clSolidBC.setArg(1, clSolid);
		clSolidBC.setArg(2, width);
		
		queue.finish();
	}
	catch(cl::Error e)
	{
		cluErr("Grid: initData", e);
	}
	
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
	
	if(oddStep)
	{
		clRender.setArg(0, clTmpLattice);
		
		clStream.setArg(0, clTmpLattice);
		clStream.setArg(1, clLattice);
		
		clCollide.setArg(0, clTmpLattice);
		
		clSolidBC.setArg(0, clTmpLattice);
	}
	else
	{
		clRender.setArg(0, clLattice);
		
		clStream.setArg(0, clLattice);
		clStream.setArg(1, clTmpLattice);
		
		clCollide.setArg(0, clLattice);
		
		clSolidBC.setArg(0, clLattice);
	}
	
	
	clStream.setArg(4, dt);
	
	try
	{
		queue.enqueueNDRangeKernel(clStream, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		queue.enqueueNDRangeKernel(clSolidBC, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		queue.enqueueNDRangeKernel(clCollide, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		queue.enqueueNDRangeKernel(clRender, cl::NullRange,
		 cl::NDRange(width, height), cl::NullRange, NULL, &event);
		
		//queue.enqueueTask(clCopy, NULL, &event);
		
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
	
	oddStep = !oddStep;
}

void Grid::render()
{
	//updateMatrix();
	//glUseProgram(0);
	/*
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glPointSize(3.f);
	GLint posit = glGetAttribLocation(shader, "Pos");
	glEnableVertexAttribArray(posit);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		
		glVertexAttribPointer(posit, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, num);
	glDisableVertexAttribArray(posit);
	*/
	/*GLint texSampLoc = glGetUniformLocation(shader, "texSamp");
	
	GLint texCoordLoc = glGetAttribLocation(shader, "TexCoord");
	GLint vertCoordLoc = glGetAttribLocation(shader, "vertOffset");
	
	//glUseProgram(0);
	glActiveTexture(GL_TEXTURE0+texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texSampLoc, texture);
	
	
	glEnableVertexAttribArray(vertCoordLoc);
	glEnableVertexAttribArray(texCoordLoc);
	 
	 glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	 glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	 
	 glBindBuffer(GL_ARRAY_BUFFER, vertCoordVBO);
	 glVertexAttribPointer(vertCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	 
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	 glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
	 
	 /*
	 glBegin(GL_TRIANGLE_FAN);
	  glVertex2f(-1.f,-1.f);
	  glVertex2f(1.f,-1.f);
	  glVertex2f(1.f,1.f);
	  glVertex2f(-1.f,1.f);
	 glEnd();
	 
	glDisableVertexAttribArray(vertCoordLoc);
	glDisableVertexAttribArray(texCoordLoc);
	*/
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuff);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
	 width, height, 0, 0,
	 winWidth, 0, 0, winHeight,
	 GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_POINT_SMOOTH);
	glBindBuffer(GL_ARRAY_BUFFER, latticeVBO);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, latticeVBO);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDisableClientState(GL_NORMAL_ARRAY);
	
	glDrawArrays(GL_POINTS, 0, num);*/
	
}

void Grid::resize(int w, int h)
{
	winHeight = h;
	winWidth = w;
}

void Grid::draw(int x, int y, bool erase)
{
	float w = (float)width/winWidth;
	float h = (float)height/winHeight;
	int sx = w*x, sy = h*y;
	
	int i = sy*width+sx;
	
	solid[i] = true;
	
}
