// The start of a utility for graphing functions of the form z = f(x, y).
//
// Created by sean on 1/5/25.
//

// clang-format off
#include "lib/function_grapher.h"

// We need this for the model viewer code we're using.
//
// TODO: It would be nice to isolate this dependency,
// since we're not actually using it here.
//
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fmt/core.h>
// clang-format on

// --------------
// Configuration.

static constexpr Config CONFIG{
    .wireframe = true,
    .constantRotation = false,
};

// A simple function to graph for testing mesh generation.
static auto func = [](double x, double y) -> double { return x * x + y * y; };

// -------------
// Program main.

int main() {
  fmt::print("Starting function grapher.\n");

  FunctionMesh mesh{func};

  // Print out some info on the generated mesh.
  mesh.printMeshData();

  // TODO: Implement model generation in FunctionMesh constructor above.

  // Initialize GLFW window.
  GLFWWrapper window;

  if (!window.init()) {
    return -1;
  }

  // Load GL functions and set options.
  bool configured = configureGL(CONFIG);

  if (!configured) {
    return -1;
  }

  // -----------------
  // Main render loop.

  while (!window.shouldClose()) {
    window.processInput();

    clearBuffers();

    // TODO: Add code to draw models here.

    window.swapBuffers();
    GLFWWrapper::pollEvents();
  }

  // -----
  // Done.

  return 0;
}
