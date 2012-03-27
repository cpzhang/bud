######################################################################
# Welcome to the CMake build system for CP's Game Dev Practice
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################
# cmake_policy(VERSION major.minor[.patch])
cmake_minimum_required(VERSION 2.8)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

# Specifies the build type for make based generators
set(CMAKE_BUILD_TYPE debug CACHE STRING
# set(CMAKE_BUILD_TYPE release CACHE STRING
  "documentation for this variable"
  FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "/MD /Zi /Od")

set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /machine:x86" )

# project name
project(bud)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../bin)
# subdirectory
# base
set(BaseName base)
add_subdirectory(base)

# math
set(MathName math)
add_subdirectory(math)

# render
set(RenderName render)
add_subdirectory(render)

# net
set(NetName net)
# add_subdirectory(net)

# tutorial
add_subdirectory(tutorial)

# tool
add_subdirectory(tool)