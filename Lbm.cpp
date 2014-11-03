#include "clu.h"
#include "Lbm.h"
#include "Settings.h"
#include <fstream>


float Lbm::w0 = 4.f/9.f;
float Lbm::w1 = 1.f/9.f;
float Lbm::w2 = 1.f/36.f;

Lbm::Lbm(cl::Platform platform, cl::Device device, std::vector<std::string> kernels)
{
  cl_context_properties cProps[] = {
    CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(),
    0};

  try
  {
    context = cl::Context(CL_DEVICE_TYPE_ALL, cProps, errorCallbackCL);
    queue = cl::CommandQueue(context, device);
  }
  catch(cl::Error e)
  {
    cluErr("Sim", e);
  }

  std::vector<std::string> sourceStr;
  for(auto it = kernels.begin(); it != kernels.end(); ++it)
  {
    std::ifstream file(it->c_str());
    if(!file.good())
     std::cerr<<"Sim: Kernel: file not good"<<std::endl;
    sourceStr.push_back(std::string((std::istreambuf_iterator<char>(file)),
     std::istreambuf_iterator<char>()));
  }

  try
  {
    cl::Program::Sources source = cl::Program::Sources();

    for(auto it = sourceStr.begin(); it != sourceStr.end(); ++it)
      source.push_back(std::make_pair(it->c_str(), it->size()));

    program = cl::Program(context, source);
  }
  catch(cl::Error e)
  {
    cluErr("Sim: program", e);
  }

  createKernels(device, kernels);
  initData();
}
Lbm::~Lbm()
{
}

void Lbm::createKernels(cl::Device device, std::vector<std::string> kernels)
{

  std::vector<std::string> sourceStr;
  for(auto it = kernels.begin(); it != kernels.end(); ++it)
  {
    std::ifstream file(it->c_str());
    if(!file.good())
     std::cerr<<"Sim: Kernel: file not good"<<std::endl;
    sourceStr.push_back(std::string((std::istreambuf_iterator<char>(file)),
     std::istreambuf_iterator<char>()));
  }

  try
  {
    cl::Program::Sources source = cl::Program::Sources();

    for(auto it = sourceStr.begin(); it != sourceStr.end(); ++it)
      source.push_back(std::make_pair(it->c_str(), it->size()));

    program = cl::Program(context, source);
  }
  catch(cl::Error e)
  {
    cluErr("Sim: program", e);
  }

  try
  {
    program.build(std::vector<cl::Device>{device});
  }
  catch(cl::Error e)
  {
    std::cout<<"Log: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device, NULL)<<std::endl;
    cluErr("Sim: Program", e);
  }
}

void Lbm::initData()
{
  D = 2, Q = 9;
  //Input horizontal velocity
  vx = settings->getDouble("$.vx");
  //Input vertical velocity
  vy = settings->getDouble("$.vy");
  //Input density ρ = rho
  rho = settings->getDouble("$.rho");
  //Relaxation time τ = tau
  tau = settings->getDouble("$.tau");

  //Wrapping
  hWrap = settings->getBool("$.wrap.horizontal");
  vWrap = settings->getBool("$.wrap.vertical");

  lattice.resize(Q*width*height);

  float vSq = 1.5f*(vx*vx+vy*vy);

  //Fill lattice with equilibrium velocities
  for(int i=0; i<width*height; ++i)
  {
    int j=i*Q;
    lattice[j]	 = w0*rho*(1.f																			-vSq);
    lattice[j+1] = w1*rho*(1.f+3.f*vx				+4.5f*vx*vx							-vSq);
    lattice[j+2] = w1*rho*(1.f+3.f*vy				+4.5f*vy*vy							-vSq);
    lattice[j+3] = w1*rho*(1.f-3.f*vx				+4.5f*vx*vx							-vSq);
    lattice[j+4] = w1*rho*(1.f-3.f*vy				+4.5f*vy*vy							-vSq);
    lattice[j+5] = w2*rho*(1.f+3.f*( vx+vy)	+4.5f*( vx+vy)*( vx+vy)	-vSq);
    lattice[j+6] = w2*rho*(1.f+3.f*(-vx+vy)	+4.5f*(-vx+vy)*(-vx+vy)	-vSq);
    lattice[j+7] = w2*rho*(1.f+3.f*(-vx-vy)	+4.5f*(-vx-vy)*(-vx-vy)	-vSq);
    lattice[j+8] = w2*rho*(1.f+3.f*( vx-vy)	+4.5f*( vx-vy)*( vx-vy)	-vSq);
  }

  size_t lSize = Q*height*width*sizeof(LTYPE);
  size_t size = height*width*sizeof(LTYPE);
}
void Lbm::step(float dt)
{

}
void Lbm::reset()
{

}
void Lbm::render()
{

}
void Lbm::draw(int x, int y, bool erase, int type)
{

}

char* Lbm::getImage()
{

}
