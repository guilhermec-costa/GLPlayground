#pragma once

#include "stb_image.h"
#include <GLFW/glfw3.h>

void load_texture(uint *texId, const char *texpath, int internalformat,
                  int pixelformat);

void bind_to_active_texture(short texIndex, uint texId);