#include "Simulation.h"

std::vector<cl::Platform> Simulation::platforms;
std::vector<std::vector<cl::Device> > Simulation::devices;

Simulation::Simulation()
{
	p = Settings::root.get<int>("OpenCL.platform");
	d = Settings::root.get<int>("OpenCL.device");
	
	#if defined (__APPLE__) || defined(MACOSX)
		CGLContextObj appleContext = CGLGetCurrentContext();
		CGLShareGroupObj appleShareGroup = CGLGetShareGroup(appleContext);
		cl_context_properties cProps[] = {
		 CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)appleShareGroup,
		 0};
	#elif defined WIN32
		cl_context_properties cProps[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
		0};
	#else
	cl_context_properties cProps[] = {
	 CL_CONTEXT_PLATFORM, (cl_context_properties) (platforms[p])(),
	 CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
	 CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
	 0};
	#endif
	cl_command_queue_properties cqProps[] = {
	 //CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
	 0};
	
	try
	{
		context = cl::Context(CL_DEVICE_TYPE_GPU, cProps, errorCallbackCL);
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

void Simulation::createKernel(std::string path)
{
	std::ifstream file(path);
	if(!file.good())
	 std::cerr<<"Sim: Kernel: file not good"<<std::endl;
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
		program.build();
	}
	catch(cl::Error e)
	{
		cluErr("Sim: program", e);
	}
}

void Simulation::init()
{
	std::string str;
	
	
	int p = Settings::root.get<int>("OpenCL.platform");
	int d = Settings::root.get<int>("OpenCL.device");
	
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
			if(i==p&&j==d)
			 std::cout << ">";
			else
			 std::cout << "*";
			std::cout << "*" << str;
			std::cout << std::endl;
		}
	}
}

void Simulation::nextDemo(bool previous)
{
	
}

void Simulation::resize(int w, int h)
{
	
}

void Simulation::restart()
{
	
}

void Simulation::step(float dt)
{
	
}

void Simulation::render()
{
	
}

void Simulation::initData()
{
	
}

void Simulation::draw(int x, int y, bool erase)
{
	
}
