#include <iostream>

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

int main() {
  std::cout << "Running GLFW test..." << std::endl;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  return 0;
}

// c & ../example_build.sh window
// ./build/window
