#include "Simulation.h"

std::vector<cl::Platform> Simulation::platforms;
std::vector<std::vector<cl::Device> > Simulation::devices;

Simulation::Simulation()
{
	
	projMat = glm::mat4();
	modelMat = glm::mat4();
	
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

void Simulation::updateMatrix()
{
	glUseProgram(shader);
	GLuint projLoc = glGetUniformLocation(shader, "projectionMatrix");
	GLuint mvmLoc = glGetUniformLocation(shader, "modelViewMatrix");
	
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));
	glUniformMatrix4fv(mvmLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
}

void Simulation::createShader(std::string vertexPath, std::string fragmentPath)
{
	std::ifstream vFile(vertexPath);
	if(!vFile.good())
	 std::cerr<<"Sim: vShader: file not good"<<std::endl;
	std::string vertStr((std::istreambuf_iterator<char>(vFile)),
	 std::istreambuf_iterator<char>());
	
	std::ifstream fFile(fragmentPath);
	if(!fFile.good())
	 std::cerr<<"Sim: fShader: file not good"<<std::endl;
	std::string fragStr((std::istreambuf_iterator<char>(fFile)),
	 std::istreambuf_iterator<char>());
	
	const char* vc_str = vertStr.c_str();
	const char* fc_str = fragStr.c_str();
	
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vertShader, 1, &vc_str, NULL);
	glShaderSource(fragShader, 1, &fc_str, NULL);
	
	glCompileShader(vertShader);
	glCompileShader(fragShader);
	
	shader = glCreateProgram();
	glAttachShader(shader, vertShader);
	glAttachShader(shader, fragShader);
	
	glLinkProgram(shader);
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
