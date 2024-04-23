#include "helpers.h"

GLuint makeShader(const std::string &source, GLenum type) {
  const char *sourceCStr = source.c_str();
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &sourceCStr, NULL);
  glCompileShader(shader);
  return shader;
}
