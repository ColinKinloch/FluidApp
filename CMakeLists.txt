cmake_minimum_required(VERSION 2.8)
project(FluidApp)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(FLUID_HEADERS
  Settings.h
  Grid.h
  clu.h
  Simulation.h
  clerr.h
)
set(FLUID_SRCS
  ${FLUID_HEADERS}
  clu.cpp
  Grid.cpp
  main.cpp
  Settings.cpp
  Simulation.cpp

  data/kernels/lattice.cl
)
set(FLUID_DATA
  data/settings.json
)

add_executable(FluidApp ${FLUID_SRCS})

add_definitions("-std=c++11")
#Required?!?
add_definitions(${CMAKE_CXX_FLAGS} "-frounding-math")
#add_definitions(${CMAKE_CXX_FLAGS} "-fsignaling-nans")

include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_search_module(jsonGLib REQUIRED json-glib-1.0)
pkg_search_module(OpenCL REQUIRED OpenCL)

if(jsonGLib_FOUND)
  include_directories(${jsonGLib_INCLUDE_DIRS})
  target_link_libraries(FluidApp ${jsonGLib_LIBRARIES})
endif()
if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIRS})
  target_link_libraries(FluidApp ${OpenCL_LIBRARIES})
endif()

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

install(DIRECTORY data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
install(DIRECTORY data/kernels/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/kernels/)
