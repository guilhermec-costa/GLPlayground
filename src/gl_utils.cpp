#include "gl_utils.h"
#include <stdexcept>

unsigned int createShaderProgram(const std::string &vertexShaderSrc,
                                 const std::string &fragShaderSrc) {
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vss = vertexShaderSrc.c_str();
  glShaderSource(vertexShader, 1, &vss, NULL);
  glCompileShader(vertexShader);

  if (checkShaderCompilation(vertexShader) == -1)
    throw std::runtime_error("failed to compile vertex shader");

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fss = fragShaderSrc.c_str();
  glShaderSource(fragmentShader, 1, &fss, NULL);
  glCompileShader(fragmentShader);

  if (checkShaderCompilation(fragmentShader) == -1)
    throw std::runtime_error("failed to compile fragment shader");

  unsigned int program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  if (checkProgramLinkingStatus(program) == -1) {
    throw std::runtime_error("failed to link program");
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}