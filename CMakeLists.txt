cmake_minimum_required(VERSION 2.8)
project(FluidApp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_SOURCE_DIR}/cmake/ClangOverrides.cmake")


set(FLUID_HEADERS
  Settings.h
  Grid.h
  clu.h
  Simulation.h
  cl.hpp
  clerr.h
)
set(FLUID_SRCS
  ${FLUID_HEADERS}
  clu.cpp
  Grid.cpp
  main.cpp
  Settings.cpp
  Simulation.cpp
)
add_executable(FluidApp ${FLUID_SRCS})


add_definitions("-std=c++11 -O2")
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

find_package(Boost REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	target_link_libraries(FluidApp ${Boost_SYSTEM_LIBRARY})
endif()


file(INSTALL data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
