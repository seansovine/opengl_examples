// Model viewer program: Work in progress.
//
// So far it shows a textured cube in the center of the window.
// We plan to add the ability to load arbitrary textured models
// and use keyboard input to move and rotate the displayed model.
// And later we'll experiment with other effects, like lighting etc.
//
// Created by sean on 12/4/24.
// Much help drawn from www.learnopengl.com examples.

// clang-format off
// Note, this must be included first.
#include "glad/glad.h"

#include "lib/textured_mesh.h"

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <tools/gl_texture.h>
#include <tools/glfw_wrapper.h>
#include <tools/transformations.h>

#include <memory>
// clang-format on

// -------------
// Helper decls.

bool gl_configure();

using ShaderAndModel = std::pair<std::shared_ptr<Shader>, std::shared_ptr<TexturedMesh>>;
ShaderAndModel loadShaderAndModel();

void clearBuffers();

void constantRotation(Transformations &transformations);

// -------------
// Program main.

int main() {
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

  // Load shader and model.
  auto [ourShader, model] = loadShaderAndModel();

  if (!ourShader || !model) {
    return -1;
  }

  // Set uo transformations.
  Transformations transformations{ourShader, window.aspectRatio()};

  // Set our resize callback that updates the projection.
  window.callbackInterface().mUserResizeCallback = [&window, &transformations](float, float) {
    transformations.updateProjectionTransformation(window.aspectRatio());
  };

  // Set the click-and-drag rotation callback.
  window.callbackInterface().mUserMouseDragCallback = [&transformations](double xAmt, double yAmt) {
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

    clearBuffers();
    model->draw(GL_TEXTURE0);

    window.swapBuffers();
    GLFWWrapper::pollEvents();

    constexpr bool ROTATING = false;
    if constexpr (ROTATING) {
      constantRotation(transformations);
    }
  }

  return 0;
}

// -------------------
// Helper definitions.

bool gl_configure() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;

    return false; // Early return.
  }

  glEnable(GL_DEPTH_TEST);

  return true;
}

ShaderAndModel loadShaderAndModel() {
  std::string vertexShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer.vs");
  std::string fragmentShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer.fs");

  // Load shaders.
  auto ourShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  // Load texture.
  auto texture1 = std::make_shared<GLTexture>("resources/textures/Bricks098_2K-JPG_Color.jpg", GL_RGB);

  if (!texture1->isLoaded()) {
    std::cout << "Failed to load texture" << std::endl;

    return {nullptr, nullptr}; // Early return.
  }

  ourShader->use();
  // Load model
  auto model = std::make_shared<TexturedMesh>(std::move(texture1));

  return {ourShader, model};
}

void constantRotation(Transformations &transformations) {
  // Rotates the model by a fixed amount on each call.
  // Right now this is not tied to a clock, so the rotation
  // speed will be different on each system. (We'll fix that.)
  transformations.updateModelTransformation();
  // Rotate by constant amount around x- and y-axes.
  transformations.rotateViewTransformation(0.01, 0.0);
  transformations.updateViewTransformation();
}

// ----------------------------
// Call OpenGL rendering funcs.

void clearBuffers() {
  // Clear buffers. Use black background.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
