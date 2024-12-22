# Build system for our OpenGL examples, including
# those from DeVries' tutorial.
#
# Based on gloom: https://github.com/aleksaro/gloom

cmake_minimum_required (VERSION 3.28..3.31)
project (opengl_examples)

include(FetchContent)

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


# Add Assimp

set(ASSIMP_INSTALL OFF)
set(ASSIMP_INSTALL_PDB OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

add_subdirectory(thirdparty/assimp)


# Get fmt library.

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.0.2
        GIT_SUBMODULES_RECURSE ON
)

FetchContent_MakeAvailable(fmt)


# Set include paths

include_directories (src/
                     thirdparty/assimp/include/
                     thirdparty/glad/include/
                     thirdparty/glfw/include/
                     thirdparty/glm/
                     thirdparty/stb/)


# Provide C-string var for repo root.
#  - Borrowed from De Vries' LearnOpenGL repo.

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)


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
set(hello_triangle_sources
        src/examples/hello_triangle/hello_triangle.cpp
        src/examples/hello_triangle/shaders.h
        src/tools/helpers.cpp
        src/tools/helpers.h)
glex_add_executable(hello_triangle "${hello_triangle_sources}") # Quotes needed to pass whole list.

# De Vries' coordinate systems example.
set(coordinate_systems_sources
        src/examples/coordinate_systems/coordinate_systems_multiple_de_vries.cpp
        thirdparty/stb/stb_image.h src/tools/glfw_wrapper.h)
glex_add_executable(coordinate_systems "${coordinate_systems_sources}") # Quotes needed to pass whole list.

# Model viewer application.
set(model_viewer_sources
        src/model_viewer/model_viewer.cpp
        src/model_viewer/lib/textured_model.h
        thirdparty/stb/stb_image.h src/tools/glfw_wrapper.h)
glex_add_executable(model_viewer "${model_viewer_sources}") # Quotes needed to pass whole list.

# Assimp demo program.
set(assimp_demo_sources
        src/model_viewer/assimp_load_demo.cpp)
glex_add_executable(assimp_demo "${assimp_demo_sources}") # Quotes needed to pass whole list.
target_link_libraries(assimp_demo assimp fmt)
