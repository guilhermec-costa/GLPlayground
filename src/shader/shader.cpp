#include "shader.h"
#include "gl_utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::cout << std::filesystem::current_path() << '\n';
  std::string vertexShaderSrc;
  std::string fragmentShaderSrc;

  std::ifstream vertexShaderFile(vertexPath);
  std::ifstream fragmentShaderFile(fragmentPath);
  try {
    if(!vertexShaderFile.is_open()) {
      std::cerr << "Error: Could not open vertex shader file.\n";
      return;
    }

    if(!fragmentShaderFile.is_open()) {
      std::cerr << "Error: Could not open fragment shader file.\n";
      return;
    }

    std::stringstream vertexShaderStream, fragmentShaderStream;
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderSrc = vertexShaderStream.str();
    fragmentShaderSrc = fragmentShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char *vertexShaderCode = vertexShaderSrc.c_str();
  const char *fragmentShaderCode = fragmentShaderSrc.c_str();

  unsigned int vertexShader, fragmentShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
  glCompileShader(vertexShader);

  if(checkShaderCompilation(vertexShader) == -1) {
    throw std::runtime_error("failed to compile vertex shader");
  };

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
  glCompileShader(fragmentShader);

  if(checkShaderCompilation(fragmentShader) == -1) {
    throw std::runtime_error("failed to compile fragment shader");
  };

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  if(checkProgramLinkingStatus(ID) == -1) {
    throw std::runtime_error("failed to link program");
  };

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setUniformBool(const std::string &name, bool value) {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniformFloat(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniformInt(const std::string &name, int value) {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniformMatrix4fv(const std::string &name, float* mat) {
  int matLoc = glGetUniformLocation(ID, name.c_str());
  glUniformMatrix4fv(matLoc, 1, GL_FALSE, mat);
}