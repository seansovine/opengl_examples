#include "helpers.h"

#include <stdexcept>

GLuint makeShader(const std::string &source, GLenum type) {
  const char *sourceCStr = source.c_str();
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &sourceCStr, NULL);
  glCompileShader(shader);
  return shader;
}

std::optional<std::string> checkShaderCompile(GLuint shader) {
  int success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    return std::string{infoLog};
  }

  return std::nullopt;
}

GLuint makeProgram(std::vector<GLuint> shaders) {
  if (size(shaders) == 0) {
    throw std::invalid_argument("Must provide at least one shader to makeProgram. None provided.");
  }
  GLuint shaderProgram = glCreateProgram();
  for (GLuint shader : shaders) {
    glAttachShader(shaderProgram, shader);
  }
  glLinkProgram(shaderProgram);
  return shaderProgram;
}

std::optional<std::string> checkProgramLink(GLuint program) {
  int success;
  char infoLog[512];

  glGetProgramiv(program, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    return std::string{infoLog};
  }

  return std::nullopt;
}
