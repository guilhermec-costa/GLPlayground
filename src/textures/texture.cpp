#include "texture.h"
#include <iostream>

Texture2D::Texture2D(const char *path, GLenum srcFormat, GLenum targetFormat, bool invert) {
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  int width, height, nrchannels;
  if(invert)
    stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path, &width, &height, &nrchannels, 0);
  if (data) {
    glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      srcFormat, 
      width, 
      height, 
      0, 
      targetFormat,
      GL_UNSIGNED_BYTE, 
      data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "failed to generate texture image\n";
  }

  std::cout << "texture created from " << path << "\n";
  stbi_image_free(data);
}

void Texture2D::setParameteri(GLenum pname, GLenum param) {
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void Texture2D::bindToTexUnit(unsigned int unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, ID);
  texUnit = unit;
}

unsigned int Texture2D::getUnit() { return texUnit; }