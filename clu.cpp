#include "clu.h"
std::string cluGetErrorString(cl_int err)
{
	std::string eStr;
	switch (err)
	{
		case CL_SUCCESS:													return "Success!";
		case CL_DEVICE_NOT_FOUND:									return "Device not found.";
		case CL_DEVICE_NOT_AVAILABLE:							return "Device not available";
		case CL_COMPILER_NOT_AVAILABLE:						return "Compiler not available";
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:		return "Memory object allocation failure";
		case CL_OUT_OF_RESOURCES:									return "Out of resources";
		case CL_OUT_OF_HOST_MEMORY:								return "Out of host memory";
		case CL_PROFILING_INFO_NOT_AVAILABLE:			return "Profiling information not available";
		case CL_MEM_COPY_OVERLAP:									return "Memory copy overlap";
		case CL_IMAGE_FORMAT_MISMATCH:						return "Image format mismatch";
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:				return "Image format not supported";
		case CL_BUILD_PROGRAM_FAILURE:						return "Program build failure";
		case CL_MAP_FAILURE:											return "Map failure";
		case CL_INVALID_VALUE:										return "Invalid value";
		case CL_INVALID_DEVICE_TYPE:							return "Invalid device type";
		case CL_INVALID_PLATFORM:									return "Invalid platform";
		case CL_INVALID_DEVICE:										return "Invalid device";
		case CL_INVALID_CONTEXT:									return "Invalid context";
		case CL_INVALID_QUEUE_PROPERTIES:					return "Invalid queue properties";
		case CL_INVALID_COMMAND_QUEUE:						return "Invalid command queue";
		case CL_INVALID_HOST_PTR:									return "Invalid host pointer";
		case CL_INVALID_MEM_OBJECT:								return "Invalid memory object";
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:	return "Invalid image format descriptor";
		case CL_INVALID_IMAGE_SIZE:								return "Invalid image size";
		case CL_INVALID_SAMPLER:									return "Invalid sampler";
		case CL_INVALID_BINARY:										return "Invalid binary";
		case CL_INVALID_BUILD_OPTIONS:						return "Invalid build options";
		case CL_INVALID_PROGRAM:									return "Invalid program";
		case CL_INVALID_PROGRAM_EXECUTABLE:				return "Invalid program executable";
		case CL_INVALID_KERNEL_NAME:							return "Invalid kernel name";
		case CL_INVALID_KERNEL_DEFINITION:				return "Invalid kernel definition";
		case CL_INVALID_KERNEL:										return "Invalid kernel";
		case CL_INVALID_ARG_INDEX:								return "Invalid argument index";
		case CL_INVALID_ARG_VALUE:								return "Invalid argument value";
		case CL_INVALID_ARG_SIZE:									return "Invalid argument size";
		case CL_INVALID_KERNEL_ARGS:							return "Invalid kernel arguments";
		case CL_INVALID_WORK_DIMENSION:						return "Invalid work dimension";
		case CL_INVALID_WORK_GROUP_SIZE:					return "Invalid work group size";
		case CL_INVALID_WORK_ITEM_SIZE:						return "Invalid work item size";
		case CL_INVALID_GLOBAL_OFFSET:						return "Invalid global offset";
		case CL_INVALID_EVENT_WAIT_LIST:					return "Invalid event wait list";
		case CL_INVALID_EVENT:										return "Invalid event";
		case CL_INVALID_OPERATION:								return "Invalid operation";
		case CL_INVALID_GL_OBJECT:								return "Invalid OpenGL object";
		case CL_INVALID_BUFFER_SIZE:							return "Invalid buffer size";
		case CL_INVALID_MIP_LEVEL:								return "Invalid mip-map level";
		default:																	return "Unknown";
	}
}

GLuint cluCreateGLVBO(const void* data, int size, GLenum target, GLenum usage)
{
	GLuint id = 0;
	
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, usage);
	
	int bSize = 0;
	glGetBufferParameteriv(target, GL_BUFFER_SIZE, &bSize);
	if(size != bSize)
	{
		glDeleteBuffers(1, &id);
		id=0;
		std::cerr<<"Failed to create VBO"<<std::endl;
	}
	glBindBuffer(target, 0);
	return id;
}

std::string cluGetProgramBuildStatus(cl_int status)
{
	switch(status)
	{
		case CL_BUILD_NONE:					return "None";
		case CL_BUILD_ERROR:				return "Error";
		case CL_BUILD_SUCCESS:			return "Success";
		case CL_BUILD_IN_PROGRESS:	return "In progress";
		default:										return "Unknown";
	}
}

void cluErr(std::string label, cl::Error e)
{
	std::cerr<<"\e[1;31m"<<label<<": "<<e.what()<<": "<<cluGetErrorString(e.err())<<":"<<e.err()<<"."<<"\e[0m"<<std::endl;
	exit(EXIT_FAILURE);
}

void errorCallbackCL(const char* errorinfo, const void* private_info_size, ::size_t cb, void* user_data)
{
	std::cerr<<"\e[1;30mCLError: "<<"\e[0m"<<std::endl;
	std::cerr<<" Error: "<<errorinfo<<std::endl;
}

void errorCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam)
{
	std::cerr<<"\e[1;31mGLError: "<<source<<":"<<id<<"\e[0m"<<std::endl;
	
	std::cerr<<" ";
	switch(type)
	{
		case GL_DEBUG_TYPE_ERROR:								std::cout<<"Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	std::cout<<"Deprecated Behavior"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	std::cout<<"Undefined Behavior"; break;
		case GL_DEBUG_TYPE_PORTABILITY:					std::cout<<"Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:					std::cout<<"Performance"; break;
		case GL_DEBUG_TYPE_OTHER:								std::cout<<"Other"; break;
		default:																std::cout<<"Unknown";
	}
	std::cerr<<": "<<message<<std::endl;
	
	std::cerr<<" Severity: ";
	switch(severity)
	{
		case GL_DEBUG_SEVERITY_LOW:					std::cout<<"Low"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:					std::cout<<"Medium"; break;
		case GL_DEBUG_SEVERITY_HIGH:					std::cout<<"High"; break;
		default:														std::cout<<"Unknown";
	}
	std::cerr<<std::endl;
}
