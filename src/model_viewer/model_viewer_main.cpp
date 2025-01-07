// Model viewer program: For experimenting with displaying 3D models with OpenGL.
//
// Created by sean on 12/4/24.
//
// Much help and inspiration drawn from www.learnopengl.com examples.
//

// clang-format off
#include "lib/model_viewer.h"

// We need this define and include combination exactly once.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fmt/core.h>

#include <memory>
// clang-format on

// --------------
// Configuration.

static constexpr Config CONFIG{
    .wireframe = true,
    .constantRotation = false,
};

// -------------
// Program main.

int main() {
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

  // Load shader and model.
  auto shaderAndModels = loadShaderAndModels();

  if (!shaderAndModels.isOkay()) {
    fmt::print("Failed to load shaders and models.\n");

    return -1;
  }

  auto ourShader = shaderAndModels.mShader;
  auto model1 = shaderAndModels.mModel1;
  auto model2 = shaderAndModels.mModel2;

  // Set uo transformations.
  Transformations transformations{ourShader, window.aspectRatio()};

  // Set GLFW event callbacks for window size and mouse interaction.
  setCallbacks(window, transformations);

  // -----------------
  // Main render loop.

  while (!window.shouldClose()) {
    window.processInput();

    clearBuffers();
    model1->draw(ourShader.get());
    model2->draw(ourShader.get());

    window.swapBuffers();
    GLFWWrapper::pollEvents();

    if constexpr (CONFIG.constantRotation) {
      constantRotation(transformations);
    }
  }

  return 0;
}
