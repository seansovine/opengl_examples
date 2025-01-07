// Model viewer program: Work in progress.
//
// In this version we're working on incorporating
// models loaded using Assimp, based on De Vries'
// tutorial on model loading and its code, from
// www.learnopengl.com.
//
// Created by sean on 12/22/24.
//

// clang-format off
#include "lib/model_viewer.h"

// We need this define and include combination exactly once.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <learnopengl/filesystem.h>
#include <tools/model_data.h>
// clang-format on

// --------------
// Configuration.

static constexpr Config CONFIG{
    .wireframe = true,
    .constantRotation = false,
};

static const auto modelPath = std::string(project_root) + "/resources/learnopengl/backpack.obj";

// -------------
// Helper decls.

std::shared_ptr<Shader> loadShader();

// -------------
// Program main.

int main() {
  // ---------
  // Do setup.

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

  // TODO: Update texture handling for Assimp-loaded models.

  // Load model with Assimp.
  Model model{modelPath};

  // Set uo transformations.
  Transformations transformations{ourShader, window.aspectRatio()};

  // Set our resize callback that updates the projection.
  setCallbacks(window, transformations);

  // -----------------
  // Main render loop.

  while (!window.shouldClose()) {
    window.processInput();

    ourShader->use();
    clearBuffers();

    model.draw(*ourShader);
    window.swapBuffers();

    GLFWWrapper::pollEvents();

    if constexpr (CONFIG.constantRotation) {
      constantRotation(transformations);
    }
  }

  return 0;
}

// ------------
// Helper defs.

std::shared_ptr<Shader> loadShader() {
  std::string vertexShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer_assimp.vs");
  std::string fragmentShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer_assimp.fs");

  // Load shaders.
  return std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}
