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

// --------------------
// Helper declarations.

std::shared_ptr<Shader> loadShader();

// --------------
// Configuration.

static constexpr Config CONFIG{
    .wireframe = true,
    .constantRotation = false,
};

// A simple function to graph for testing mesh generation.
static auto func = [](double x, double y) -> double { return 0.5 * (x * x + y * y); };

// -------------
// Program main.

int main() {
  fmt::print("Starting function grapher.\n");

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

  // Load shader.
  auto ourShader = loadShader();

  if (!ourShader) {
    return -1;
  }
  ourShader->use();

  // Generate meshes for function graph.
  FunctionMesh mesh{func};
  // Print out some info on the generated mesh.
  mesh.printMeshData();

  // Set uo transformations.
  Transformations transformations{ourShader, window.aspectRatio()};

  // Set our standard resize and mouse event callbacks.
  setCallbacks(window, transformations);

  // -----------------
  // Main render loop.

  while (!window.shouldClose()) {
    window.processInput();

    clearBuffers();
    mesh.draw(ourShader.get());

    window.swapBuffers();
    GLFWWrapper::pollEvents();
  }

  // -----
  // Done.

  return 0;
}

// -------------------
// Helper definitions.

std::shared_ptr<Shader> loadShader() {
  std::string vertexShaderPath = FileSystem::getPath("src/function_grapher/shaders/function_grapher.vs");
  std::string fragmentShaderPath = FileSystem::getPath("src/function_grapher/shaders/function_grapher.fs");

  // Load shaders.
  return std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}
