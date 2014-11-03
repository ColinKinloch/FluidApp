#include "Simulation.h"
#include <fstream>
std::vector<cl::Platform> Simulation::platforms;
std::vector<std::vector<cl::Device> > Simulation::devices;

Simulation::Simulation(Fl_Gl_Window* win)
{
  inited = false;
	p = settings->getInt("$.OpenCL.platform");
	d = settings->getInt("$.OpenCL.device");

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
		CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[p])(),
		0};
	#else
	cl_context_properties cProps[] = {
	 CL_CONTEXT_PLATFORM, (cl_context_properties) (platforms[p])(),
	 //CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
	 CL_GL_CONTEXT_KHR, (cl_context_properties) win->context(),
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
  return createKernel(std::vector<std::string>({path}));
}
void Simulation::createKernel(std::vector<std::string> path)
{
  std::vector<std::string> sourceTexts;

  for(auto it = path.begin(); it != path.end(); ++it)
  {
    std::ifstream file(it->c_str());
    if(!file.good())
     std::cerr<<"Sim: Kernel: file not good"<<std::endl;
    sourceTexts.push_back(std::string((std::istreambuf_iterator<char>(file)),
     std::istreambuf_iterator<char>()));
  }

  try
  {
    cl::Program::Sources source = cl::Program::Sources();

    for(auto it = sourceTexts.begin(); it != sourceTexts.end(); ++it)
     source.push_back(std::make_pair(it->c_str(), it->size()));

    program = cl::Program(context, source);
  }
  catch(cl::Error e)
  {
          cluErr("Sim: program", e);
  }

  try
  {
          program.build(std::vector<cl::Device>{devices[p][d]});
  }
  catch(cl::Error e)
  {
          std::cout<<"Log: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[p][d], NULL)<<std::endl;
          cluErr("Sim: program", e);
  }
}

void Simulation::init()
{
	std::string str;
	
	
	int p = settings->getInt("$.OpenCL.platform");
	int d = settings->getInt("$.OpenCL.device");
	
	try
	{
		cl::Platform::get(&platforms);
	}
	catch(cl::Error e)
	{
		cluErr("Sim: init", e);
	}
	std::string version;
	platforms[p].getInfo(CL_PLATFORM_VERSION, &version);
	
	std::cout<<version<<std::endl;
	
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
			cl_device_type type = 0;
			devices[i][j].getInfo(CL_DEVICE_TYPE, &type);
			devices[i][j].getInfo(CL_DEVICE_NAME, &str);
			if(i==p&&j==d)
			 std::cout << ">";
			else
			 std::cout << "*";
			std::cout << "*" << str;
			if(type!=CL_DEVICE_TYPE_GPU)
			  std::cout<<", incompatible";
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
	inited = true;
}

void Simulation::draw(int x, int y, bool erase)
{
	
}
