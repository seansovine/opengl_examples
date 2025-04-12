// coordinate_systems_multiple example from Joey De Vries' learnopengl.com.

// clang-format off
// NOTE: These must be included in this order.
#include "glad/glad.h"
#include <tools/glfw_wrapper.h>
#include <tools/gl_texture.h>
// clang-format on

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <iostream>

// --------------
// Main function.

int main() {
  GLFWWrapper window;
  if (!window.init()) {
    return -1;
  }

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader program
  // ------------------------------------
  std::string vertexShaderPath =
      FileSystem::getPath("src/examples/coordinate_systems/shaders/coordinate_systems.vs");
  std::string fragmentShaderPath =
      FileSystem::getPath("src/examples/coordinate_systems/shaders/coordinate_systems.fs");

  Shader ourShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  // clang-format off
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,   0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,   -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,   -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,   0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,   0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,   0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,   -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  // world space positions of our cubes
  glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                               glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                               glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                               glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                               glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  // clang-format on

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // load and create textures
  // ------------------------
  // texture 1
  // ---------
  GLTexture texture1("resources/learnopengl/textures/container.jpg", GL_RGB);
  if(!texture1.isLoaded()) {
    std::cout << "Failed to load texture" << std::endl;
    return -1;
  }
  // // texture 2
  // // ---------
  GLTexture texture2("resources/learnopengl/textures/awesomeface.png", GL_RGBA);
  if(!texture2.isLoaded()) {
    std::cout << "Failed to load texture" << std::endl;
    return -1;
  }

  // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
  // -------------------------------------------------------------------------------------------
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  // render loop
  // -----------
  while (!window.shouldClose()) {
    window.processInput();

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    // bind textures on corresponding texture units
    texture1.bind(GL_TEXTURE0);
    texture2.bind(GL_TEXTURE1);

    // activate shader
    ourShader.use();

    // create transformations
    auto view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    auto projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), window.dimensions().first / window.dimensions().second, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // pass transformation matrices to the shader
    ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame,
                                                 // but since the projection matrix rarely changes it's often
                                                 // best practice to set it outside the main loop only once.
    ourShader.setMat4("view", view);

    // render boxes
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      // calculate the model matrix for each object and pass it to shader before drawing
      auto model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      ourShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    window.swapBuffers();
    GLFWWrapper::pollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  return 0;
}
