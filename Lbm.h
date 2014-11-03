

#pragma once

#include <CL/cl.hpp>

#include "Cell.h"

#define LTYPE float

class Lbm
{
  cl::Platform plat;
  cl::Device dev;
  cl::Context context;
  cl::CommandQueue queue;
  cl::Program program;
  cl::Event event;

  int width;
  int height;

  std::vector<LTYPE> lattice;

  int D;
  int Q;

  static float w0;
  static float w1;
  static float w2;

  bool hWrap;
  bool vWrap;

  float tau;
  float vx;
  float vy;
  float rho;

  virtual void createKernels(cl::Device device, std::vector<std::string> kernels);

public:
  Lbm(cl::Platform platform, cl::Device device, std::vector<std::string> kernels);
  ~Lbm();

  virtual void initData();
  virtual void step(float dt);
  virtual void reset();
  virtual void render();
  virtual void draw(int x, int y, bool erase=false, int type=0);

  virtual char* getImage();
};
