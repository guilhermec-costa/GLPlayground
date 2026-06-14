#include "gl_utils.h"
#include "shader.h"
#include "texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>
#include <filesystem>
#include <stb_image.h>

void churrosTexture(GLFWwindow* window, std::optional<InputProcessor> inputProcessor) {
  float quad[] = {
    // position          // uv
    -0.5f, 0.5f, 0.0f,   0.0, 3.0,  // top left
    -0.5f, -0.5f, 0.0f,  0.0, 0.0, // bottom left
    0.5f, 0.5f, 0.0f,    3.0, 3.0,// top right
    0.5f, -0.5f, 0.0f,   3.0, 0.0// bottom right
  };

  unsigned int texVBO, texVAO, texEBO;
  glGenVertexArrays(1, &texVAO);
  glGenBuffers(1, &texVBO);
  glGenBuffers(1, &texEBO);

  glBindVertexArray(texVAO);

  glBindBuffer(GL_ARRAY_BUFFER, texVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  unsigned int quadIndices[] = {
    0, 1, 2,
    1, 2, 3
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  auto staticPath = std::filesystem::path(PROJECT_ROOT) / "static";
  auto woodTexPath = staticPath/"wood.jpg";
  Texture2D woodTex(woodTexPath.c_str(), GL_RGB, GL_RGB, false);
  woodTex.setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  woodTex.setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  woodTex.setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  woodTex.setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  woodTex.bindToTexUnit(0);

  auto happyfaceTexPath = staticPath/"happyface.png";
  Texture2D happyTex(happyfaceTexPath.c_str(),  GL_RGB, GL_RGBA, true);
  happyTex.setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  happyTex.setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  happyTex.setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  happyTex.setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  happyTex.bindToTexUnit(1);
  
  auto shaderPath = std::filesystem::path(PROJECT_ROOT)/"src"/"textures"/"churros";
  Shader shader((shaderPath/"vertex.glsl").c_str(), (shaderPath/"frag.glsl").c_str());

  shader.use();
  shader.setUniformInt("woodTex", 0);
  shader.setUniformInt("happyTex", 1);
  float brightness = 1.0f;
  shader.setUniformFloat("brightness", brightness);

  float brightnessFactor = 0.002f;
  float xOffset = 0.0;
  float yOffset = 0.0;
  float offsetFactor = 0.0022;

  while(!glfwWindowShouldClose(window)) {
    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    shader.use();
    woodTex.bindToTexUnit(0);
    happyTex.bindToTexUnit(1);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      yOffset += offsetFactor;
      shader.setUniformFloat("yOffset", yOffset);
      brightness -= brightnessFactor;
      brightness = std::max(brightness, 0.0f);
      shader.setUniformFloat("brightness", brightness);
    } 
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      yOffset -= offsetFactor;
      shader.setUniformFloat("yOffset", yOffset);
      brightness -= brightnessFactor;
      brightness = std::max(brightness, 0.0f);
      shader.setUniformFloat("brightness", brightness);
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      xOffset += offsetFactor;
      shader.setUniformFloat("offset", xOffset);
      brightness += brightnessFactor;
      brightness = std::min(brightness, 1.0f);
      shader.setUniformFloat("brightness", brightness);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      xOffset -= offsetFactor;
      shader.setUniformFloat("offset", xOffset);
      brightness += brightnessFactor;
      brightness = std::min(brightness, 1.0f);
      shader.setUniformFloat("brightness", brightness);
    }

    std::cout << "brightness: " << brightness << "\n";

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glBindVertexArray(texVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}