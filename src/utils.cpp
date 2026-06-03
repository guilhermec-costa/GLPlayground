#include <glad/glad.h>
#include <iostream>

int checkShaderCompilation(unsigned int shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;

    return -1;
  }

  return 0;
}

int checkProgramLinkingStatus(unsigned int programId) {
  int success;
  char infoLog[512];
  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    return -1;
  }
  return 0;
}