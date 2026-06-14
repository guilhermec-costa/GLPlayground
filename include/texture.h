#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class Texture2D {
public:
  Texture2D(const char *path, GLenum srcFormat, GLenum targetFormat, bool invert);

  void setParameteri(GLenum pname, GLenum param);
  void bindToTexUnit(unsigned int unit);
  unsigned int getUnit();

private:
  unsigned int ID;
  unsigned int texUnit;
};
