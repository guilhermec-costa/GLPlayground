#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

int checkShaderCompilation(unsigned int shaderId);
int checkProgramLinkingStatus(unsigned int programId);

typedef void (*InputProcessor)(GLFWwindow *window);

unsigned int createShaderProgram(const std::string &vertexShaderSrc,
                                 const std::string &fragShaderSrc);