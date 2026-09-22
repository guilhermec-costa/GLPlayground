#include <glad/glad.h>
#include "texture_utils.h"

void load_texture(uint* texId, const char* texpath, int internalformat, int pixelformat) {
  glGenTextures(1, texId);
  glBindTexture(GL_TEXTURE_2D, *texId);

  int w, h, nrch;
  unsigned char *texData = stbi_load(texpath, &w, &h, &nrch, 0);
  if(texData) {
    glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      internalformat, 
      w, h, 0, 
      pixelformat, 
      GL_UNSIGNED_BYTE, 
      texData
    );
    stbi_image_free(texData);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void bind_to_active_texture(short texIndex, uint texId) {
  glActiveTexture(GL_TEXTURE0 + texIndex);
  glBindTexture(GL_TEXTURE_2D, texId);
}