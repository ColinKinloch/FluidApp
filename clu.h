#ifndef CK_CLU
#define CK_CLU

#include <GL/glew.h>
#include <GL/glu.h>
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
void errorCallbackCL(const char* errorinfo, const void* private_info_size, ::size_t cb, void* user_data);
void errorCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);

#endif//CK_CLU
