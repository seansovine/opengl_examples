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
// Note, this must be included first.
#include "glad/glad.h"

#include "tools/model_data.h"
#include "tools/transformations.h"

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <tools/glfw_wrapper.h>

#include <memory>
// clang-format off

// -------
// Config.

constexpr bool CONSTANT_ROTATION = false;

static const auto modelPath = std::string(project_root) + "/resources/learnopengl/backpack.obj";

// -------------
// Helper decls.

bool gl_configure();

std::shared_ptr<Shader> loadShader();

void clearBuffers();

void applyConstantRotations(Transformations &transformations);

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
  bool configured = gl_configure();

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
  window.callbackInterface().mUserResizeCallback = [&window, &transformations](float, float) {
    transformations.updateProjectionTransformation(window.aspectRatio());
  };

  // Set the click-and-drag callback.
  window.callbackInterface().mUserMouseDragCallback = [&transformations](float xAmt, float yAmt) {
    transformations.rotateViewTransformation(0.05 * yAmt, 0.05 * xAmt);
    transformations.updateViewTransformation();
  };

  // Set mouse wheel zoom callback.
  window.callbackInterface().mUserMouseScrollCallback = [&window, &transformations](double, double yDelta) {
    transformations.updateFoV(yDelta);
    transformations.updateProjectionTransformation(window.aspectRatio());
  };

  // -----------------
  // Main render loop.

  while (!window.shouldClose()) {
    window.processInput();

    ourShader->use();
    clearBuffers();

    model.draw(*ourShader);
    window.swapBuffers();

    GLFWWrapper::pollEvents();

    if constexpr (CONSTANT_ROTATION) {
      applyConstantRotations(transformations);
    }
  }

  return 0;
}

// ------------
// Helper defs.

bool gl_configure() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;

    return false; // Early return.
  }

  glEnable(GL_DEPTH_TEST);

  return true;
}

std::shared_ptr<Shader> loadShader() {
  std::string vertexShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer_assimp.vs");
  std::string fragmentShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer_assimp.fs");

  // Load shaders.
  return std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}

void clearBuffers() {
  // Clear buffers. Use black background.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void applyConstantRotations(Transformations &transformations) {
  // Rotates the model by a fixed amount on each call.

  // TODO: Right now this is not tied to a clock, so
  // the rotation speed will be different on each system.

  transformations.updateModelTransformation();

  // Rotate by constant amount around x- and y-axes.
  transformations.rotateViewTransformation(0.01, 0.0);
  transformations.updateViewTransformation();
}
