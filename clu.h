#ifndef CK_CLU
#define CK_CLU

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include <CL/opencl.h>
#include "clerr.h"
#include <CL/cl.hpp>

GLuint cluCreateGLVBO(const void* data, int size, GLenum target, GLenum usage);
std::string cluGetErrorString(cl_int err);
cl_int cluGetPlatforms(std::vector<cl_platform_id>* platforms);
cl_int cluGetDevices(cl_platform_id platform, std::vector<cl_device_id>* devices);
char* cluGetProgramBuildStatus(cl_program program, cl_device_id device);
void cluErr(std::string label, cl::Error e);

#endif//CK_CLU
