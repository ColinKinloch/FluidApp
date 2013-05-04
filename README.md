# Computational Simulation of Fluid Dynamics - Artifact #
An implimentation of Lattice Blotzmann method in OpenCL.

## User Guide ##
Click and drag on the screen to draw obsticals to flow.

Plus and minus keyboard buttons cycle through demo obsticals.

Settings such as resolution and flow velocity can be edited in "./data/settings.json", an explination of each setting
is listed under "Settings".

### Keyboard Controls ###
* Esc or Ctrl+Q or Ctrl+C - Close program
* Ctrl + F - Fullscreen
* p - Pause simulation
* r - Restart simulation
* = or + - Next demo
* _ or - - Previous demo

### Settings ###
* width: The width of the simulation. Integer
* height: The height of the simulation. Integer
* fps: Framerate of simulation. Integer
* vx: Velocity of horizontal flow. Floating point
* vy: Velocity of vertical flow. Floating point
* rho: Input density. Floating point
* tau: Relaxation time of the fluid. Floating point
* wrap.horizontal: Whether the display wraps horizontaly. Boolean
* wrap.vertical: Whether the display wraps vertical. Boolean
* OpenCL.platform: Index of platform run simulation on. Integer
* OpenCL.device: Index of device to run simulation on. Integer

## Building ##
[CMake][1] is used to produce system specific build files. The artifact was developed on Linux and while all
dependanies are cross platform there may be compatability issues with other operating systems. 

### Dependancy list ###
* [GLEW][2]
* [freeGLUT][3]
* [OpenGL][4]
* [OpenCL][5]
* [JsonCpp][6]

[1]: http://www.cmake.org/ "CMake"
[2]: http://glew.sourceforge.net/ "GLEW"
[3]: http://freeglut.sourceforge.net/ "freeGLUT"
[4]: http://www.khronos.org/opengl/ "OpenGL"
[5]: http://www.khronos.org/opencl/ "OpenCL"
[6]: http://jsoncpp.sourceforge.net/ "JsonCpp"

*Colin Kinloch - 1026970*
