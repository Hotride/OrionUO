cmake_minimum_required(VERSION 3.7)
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} clang PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} clang++ PATHS ENV PATH NO_DEFAULT_PATH)

project(OrionUO)

#
# Custom Build Options
#
option(ENABLE_LTO "Enables Link Time Optimization" OFF)


list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/CMake
)

# Enable folders for IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Libraries to link
set(LIBS)

# CMake Customizations
include(CheckAndAddFlag)
include(CheckCCompilerFlag)
include(CompileDefinitions)
include(CCache)
include(Revision)
include(Compiler)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Build type (Release/Debug/RelWithDebInfo/MinSizeRel)" FORCE)
endif()

#
# Dependencies
#

#include(CheckLib)
include(CheckCXXSourceRuns)

find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
find_package(FreeImage REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
#include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${FREEIMAGE_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
#
# Project Files
#

configure_file(
  "${PROJECT_SOURCE_DIR}/OrionUO/GitRevision.h.in"
  "${PROJECT_BINARY_DIR}/GitRevision.h"
)
include_directories("${PROJECT_BINARY_DIR}/")

add_subdirectory(OrionUO)
