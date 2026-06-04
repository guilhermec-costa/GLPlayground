#pragma once

#include <GLFW/glfw3.h>

int checkShaderCompilation(unsigned int shaderId);
int checkProgramLinkingStatus(unsigned int programId);

typedef void (*InputProcessor) (GLFWwindow* window);