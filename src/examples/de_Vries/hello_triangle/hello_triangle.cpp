/**
 *  Modified from:
 *    Joey De Vries' "hello triangle" example.
 *    From learnopengl.com.
 */

// clang-format off
#include "glad/glad.h"
#include <GLFW/glfw3.h>
// clang-format on

#include "tools/helpers.h"
#include "shaders.h"

#include <iostream>

struct GlState {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

GlState setupVerticxState();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // build and compile our shader program
  // ------------------------------------

  // vertex shader
  GLuint vertexShader = makeShader(vertexShaderSource, GL_VERTEX_SHADER);
  if (auto error = checkShaderCompile(vertexShader); error) {
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << error.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  // fragment shader one
  GLuint fragmentShaderOne = makeShader(fragmentShaderSourceOne, GL_FRAGMENT_SHADER);
  if (auto error = checkShaderCompile(fragmentShaderOne); error) {
    std::cout << "ERROR::SHADER::FRAGMENT_ONE::COMPILATION_FAILED\n" << error.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  // fragment shader one
  GLuint fragmentShaderTwo = makeShader(fragmentShaderSourceTwo, GL_FRAGMENT_SHADER);
  if (auto error = checkShaderCompile(fragmentShaderTwo); error) {
    std::cout << "ERROR::SHADER::FRAGMENT_TWO::COMPILATION_FAILED\n" << error.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  // link shaders
  GLuint shaderProgramOne = makeProgram({vertexShader, fragmentShaderOne});
  if (auto error = checkProgramLink(shaderProgramOne); error) {
    std::cout << "ERROR::SHADER::PROGRAM_TWO::LINKING_FAILED\n" << error.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  // link shaders
  GLuint shaderProgramTwo = makeProgram({vertexShader, fragmentShaderTwo});
  if (auto error = checkProgramLink(shaderProgramTwo); error) {
    std::cout << "ERROR::SHADER::PROGRAM_TWO::LINKING_FAILED\n" << error.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOne);
  glDeleteShader(fragmentShaderTwo);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  GlState state = setupVerticxState();

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
  // happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind
  // VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(state.vao); // seeing as we only have a single VAO there's no need to bind it every
                                  // time, but we'll do so to keep things a bit more organized

    // draw our first triangle
    glUseProgram(shaderProgramOne);

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    // draw our second triangle
    glUseProgram(shaderProgramTwo);

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    auto offset = 3 * sizeof(unsigned int);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const void *)offset);

    // glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &state.vao);
  glDeleteBuffers(1, &state.vbo);
  glDeleteBuffers(1, &state.ebo);
  glDeleteProgram(shaderProgramOne);
  glDeleteProgram(shaderProgramTwo);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

GlState setupVerticxState() {
  GlState state;

  float vertices[] = {
      0.55f, 0.5f, 0.0f,   // top right
      0.55f, -0.5f, 0.0f,  // bottom right
      -0.45f, -0.5f, 0.0f, // bottom left
      //
      -0.55f, 0.5f, 0.0f,  // top left
      -0.55f, -0.5f, 0.0f, // bottom left
      0.45f, 0.5f, 0.0f    // top right
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 2, // first Triangle
      3, 5, 4  // second Triangle
  };

  glGenVertexArrays(1, &state.vao);
  glGenBuffers(1, &state.vbo);
  glGenBuffers(1, &state.ebo);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s),
  // and then configure vertex attributes(s).
  glBindVertexArray(state.vao);

  glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as
  // the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object
  // IS stored in the VAO; keep the EBO bound.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return state;
}
