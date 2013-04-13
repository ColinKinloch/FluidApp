#include "Simulation.h"

std::vector<cl::Platform> Simulation::platforms;
std::vector<std::vector<cl::Device> > Simulation::devices;

Simulation::Simulation()
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
		cluErr("Sim", e);
	}
	
}

Simulation::~Simulation()
{
	
}

void Simulation::createProgram(std::string path)
{
	std::ifstream file(path);
	if(!file.good())
	 std::cerr<<"Sim: file not good"<<std::endl;
	std::string srcStr((std::istreambuf_iterator<char>(file)),
	 std::istreambuf_iterator<char>());
	
	try
	{
		cl::Program::Sources source(1, std::make_pair(srcStr.c_str(), srcStr.length()+1));
		program = cl::Program(context, source);
	}
	catch(cl::Error e)
	{
		cluErr("Sim: program", e);
	}
	
	try
	{
		program.build(devices[p]);
	}
	catch(cl::Error e)
	{
		cluErr("Sim: program", e);
	}
}

void Simulation::init()
{
	std::string str;
	
	try
	{
		cl::Platform::get(&platforms);
	}
	catch(cl::Error e)
	{
		cluErr("Sim: init", e);
	}
	
	devices.resize(platforms.size());
	
	std::cout << "Platforms:" << std::endl;
	for(int i=0; i<platforms.size(); i++)
	{
		platforms[i].getInfo(CL_PLATFORM_VENDOR, &str);
		std::cout << "*" << str << std::endl;
		
		try
		{
			platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices[i]);
		}
		catch(cl::Error e)
		{
			cluErr("Sim: init", e);
		}
		
		for(int j=0; j<devices[i].size(); j++)
		{
			devices[i][j].getInfo(CL_DEVICE_NAME, &str);
			std::cout << "**" << str << std::endl;
		}
	}
}

void Simulation::step()
{
	
}

void Simulation::render()
{
	
}

void Simulation::initData()
{
	
}
