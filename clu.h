//A set of helpful functions for dealing with OpenCL

#pragma once

//#define CL_VERSION_1_1
//#undef CL_VERSION_1_2
//#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define __CL_ENABLE_EXCEPTIONS

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include "cl.hpp"
#else
#include <CL/cl.h>
#include <CL/cl.hpp>
#endif
#include "clerr.h"

GLuint cluCreateGLVBO(const void* data, int size, GLenum target, GLenum usage);
std::string cluGetErrorString(cl_int err);
cl_int cluGetPlatforms(std::vector<cl_platform_id>* platforms);
cl_int cluGetDevices(cl_platform_id platform, std::vector<cl_device_id>* devices);
char* cluGetProgramBuildStatus(cl_program program, cl_device_id device);
void cluErr(std::string label, cl::Error e);
void errorCallbackCL(const char* errorinfo, const void* private_info_size, ::size_t cb, void* user_data);
void errorCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam);
