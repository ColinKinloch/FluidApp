cmake_minimum_required(VERSION 2.8)
project(FluidApp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_executable(FluidApp main.cpp)
add_library(Settings Settings.cpp)
add_library(Simulation Simulation.cpp)
add_library(Particles Particles.cpp)
add_library(clu clu.cpp)

target_link_libraries(FluidApp Particles Simulation Settings)
target_link_libraries(Simulation clu)

target_link_libraries(FluidApp jsoncpp)

add_definitions("-std=c++11")
#Required?!?
#add_definitions(${CMAKE_CXX_FLAGS} "-frounding-math")
#add_definitions(${CMAKE_CXX_FLAGS} "-fsignaling-nans")

include_directories(include)

#target_link_libraries(FluidApp jsoncpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
if(OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	target_link_libraries(FluidApp ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)
if(GLUT_FOUND)
	include_directories(${GLUT_INCLUDE_DIRS})
	target_link_libraries(FluidApp ${GLUT_LIBRARIES})
endif(GLUT_FOUND)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(FluidApp ${GLEW_LIBRARIES})
endif(GLEW_FOUND)

find_package(OpenCL REQUIRED)
if(OPENCL_FOUND)
	include_directories(${OPENCL_INCLUDE_DIRS})
	target_link_libraries(FluidApp ${OPENCL_LIBRARIES})
endif(OPENCL_FOUND)

file(INSTALL data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
