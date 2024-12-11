// Model viewer program: Work in progress.
//
// So far it shows a textured cube in the center of the window.
// We plan to add the ability to load arbitrary textured models
// and use keyboard input to move and rotate the displayed model.
// And later we'll experiment with other effects, like lighting etc.
//
// Created by sean on 12/4/24.
// Much help drawn from www.learnopengl.com examples.

// Note, this must be included first.
#include "glad/glad.h"

#include "lib/textured_model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <tools/gl_texture.h>
#include <tools/glfw_wrapper.h>

#include <memory>

// -------------
// Helper decls.

bool gl_configure();

using ShaderAndModel = std::pair<std::shared_ptr<Shader>, std::shared_ptr<TexturedModel>>;

ShaderAndModel loadShaderAndModel();

void setupMatrices(const Shader* shader, float aspectRatio);

void updateModelTransformation(const Shader* shader);

void updateViewTransformation(const Shader* shader);

void render(const TexturedModel* model);

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

  // Load shader and model.
  auto [ourShader, model] = loadShaderAndModel();

  if (!ourShader || !model) {
    return -1;
  }

  // Set view transformations.
  setupMatrices(ourShader.get(), window.aspectRatio());

  // -----------------
  // Main render loop.

  while (!window.shouldClose()) {
    window.processInput();

    render(model.get());

    window.swapBuffers();
    GLFWWrapper::pollEvents();

    // Rotates the model by a fixed amount on each call.
    // Right now this is not tied to a clock, so the rotation
    // speed will be different on each system. (We'll fix that.)
    updateModelTransformation(ourShader.get());
    // Camera is steadily moving away from object.
    updateViewTransformation(ourShader.get());
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

ShaderAndModel loadShaderAndModel() {
  std::string vertexShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer.vs");
  std::string fragmentShaderPath = FileSystem::getPath("src/model_viewer/shaders/model_viewer.fs");

  // Load shaders.
  auto ourShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  // Load texture.
  auto texture1 = std::make_shared<GLTexture>("resources/textures/Bricks098_2K-JPG_Color.jpg", GL_RGB);

  if (!texture1->isLoaded()) {
    std::cout << "Failed to load texture" << std::endl;

    return { nullptr, nullptr }; // Early return.
  }

  // Set texture as a uniform for shader.
  ourShader->use();
  ourShader->setInt("texture1", 0);

  // Load model
  auto model = std::make_shared<TexturedModel>( std::move(texture1) );

  return { ourShader, model };
}

// Helper for constructing the model matrix.
void makeModelMatrix(glm::mat4& model,const glm::vec3& position, float angle) {
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, -1.0f));
  float scale_factor = 0.7f;
  model = glm::scale(model, glm::vec3(scale_factor, scale_factor, scale_factor));
}

// NOTE: Assumes we don't resize the window.
// We could have the window resize callback call this to
// update the aspect ratio in the perspective transformation.
void setupMatrices(const Shader* shader, const float aspectRatio) {
  // Matrices are based on www.learnopengl.com coordinate systems example.

  // Performs perspective projection.
  auto projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), aspectRatio,
                                0.1f, 100.0f);

  // Converts world coordinates to camera viewpoint coordinates.
  auto view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  float angle = 20.0f;
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

  // Converts model local coordinates to world coordinates.
  auto model = glm::mat4(1.0f);
  makeModelMatrix(model, position, angle);

  // Pass matrices to shader as uniforms.
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);
  shader->setMat4("model", model);
}

void updateModelTransformation(const Shader* shader) {
  constexpr float increment = 0.4f;
  constexpr unsigned int mod = 360 / increment;
  static unsigned int step = 0;

  // Update model matrix.

  float angle = 20.0f + increment * step;
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

  auto model = glm::mat4(1.0f);
  makeModelMatrix(model, position, angle);
  shader->setMat4("model", model);

  // Update "time" step.
  step = (step + 1) % mod;
}

void updateViewTransformation(const Shader* shader) {
  constexpr float increment = 0.4f;
  static auto rotation = glm::mat4(1.0f);

  // Update view matrix.

  glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f);

  auto view = glm::mat4(1.0f);
  view = glm::translate(view, position) * glm::inverse(rotation);
  shader->setMat4("view", view);

  // Rotation camera position vector around y-axis.
  rotation = glm::rotate(rotation, glm::radians(increment), glm::vec3(0.0f, 1.0f, 0.0f));
}

// ----------------------------
// Call OpenGL rendering funcs.

void render(const TexturedModel* model) {
  // Clear buffers. Use black background.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind texture to OpenGL texture unit.
  model->bindTexture(GL_TEXTURE0);

  // Bind VAO.
  glBindVertexArray(model->VAO());

  // Draw the model.
  glDrawArrays(GL_TRIANGLES, 0, model->vertexCount());
}
