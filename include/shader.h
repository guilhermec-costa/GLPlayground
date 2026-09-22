#pragma once

#include "glm/ext/vector_float3.hpp"
#include <glad/glad.h>
#include <string>

class Shader {
public:
  unsigned int ID;

public:
  Shader(const char* vertexPath, const char* fragmentPath);
  void use();
  void setUniformBool(const std::string& name, bool value);
  void setUniformInt(const std::string& name, int value);
  void setUniformFloat(const std::string& name, float value);
  void setUniformMat4fv(const std::string &name, float* mat);
  void setUniformVec3f(const std::string &name, glm::vec3 v);
};