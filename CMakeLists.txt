# Build system for our OpenGL examples, including
# those from DeVries' tutorial.
#
# Based on gloom: https://github.com/aleksaro/gloom

cmake_minimum_required (VERSION 3.0)
project (opengl_examples)


# Based these on gloom but not sure we want all of them or not.

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_VERBOSE_MAKEFILE 0) # 1 should be used for debugging
set (CMAKE_SUPPRESS_REGENERATION TRUE) # Suppresses ZERO_CHECK
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++20")
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
endif()


# Add GLFW

option (GLFW_INSTALL OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)

add_subdirectory (thirdparty/glfw)


# Set include paths

include_directories (src/
                     thirdparty/glad/include/
                     thirdparty/glfw/include/
                     thirdparty/glm/
                     thirdparty/stb/)


# Add files as done in gloom -- maybe change later

file (GLOB         VENDORS_SOURCES thirdparty/glad/src/glad.c)
file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                   src/*.cxx
                                   src/*.cc
                                   src/*.c)
file (GLOB_RECURSE PROJECT_SHADERS src/*.comp
                                   src/*.frag
                                   src/*.geom
                                   src/*.vert)
file (GLOB         PROJECT_CONFIGS CMakeLists.txt
                                   README.md
                                  .gitignore
                                  .gitmodules)


# Organizing files as done in gloom -- maybe change later

source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("shaders" FILES ${PROJECT_SHADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})
source_group ("vendors" FILES ${VENDORS_SOURCES})


# Set executable and target link libraries
#  - Following gloom uncritically for now here.

add_definitions (-DGLFW_INCLUDE_NONE
                 -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")


## Add executable and library targets. ##

# Provides glex_add_executable macro.
include(${CMAKE_CURRENT_LIST_DIR}/cmake/utilities.cmake)

# Basic glfw window examples.
set(glfw_window_sources src/examples/glfw_window/glfw_window.cpp)
glex_add_executable(glfw_window "${glfw_window_sources}")

set(hello_window_sources src/examples/glfw_window/hello_window.cpp)
glex_add_executable(hello_window "${hello_window_sources}")

# Our version of DeVries' "hello triangle".
set(hello_triangle_sources src/examples/de_Vries/hello_triangle/hello_triangle.cpp
                 src/examples/de_Vries/hello_triangle/shaders.h
                 src/tools/helpers.cpp
                 src/tools/helpers.h)
glex_add_executable(hello_triangle "${hello_triangle_sources}") # Quotes needed to pass whole list.
