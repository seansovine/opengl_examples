#ifndef HELPERS_H
#define HELPERS_H

#include "glad/glad.h"

#include <iostream>
#include <optional>
#include <utility>
#include <vector>

GLuint makeShader(const std::string &source, GLenum type);

std::optional<std::string> checkShaderCompile(GLuint shader);

GLuint makeProgram(std::vector<GLuint> shaders);

std::optional<std::string> checkProgramLink(GLuint program);

#endif
