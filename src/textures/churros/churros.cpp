#include "gl_utils.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <filesystem>
#include <stb_image.h>

void churrosTexture(GLFWwindow* window, std::optional<InputProcessor> inputProcessor) {
  float quad[] = {
    // position          // uv
    -0.5f, 0.5f, 0.0f,   0.0, 1.0,  // top left
    -0.5f, -0.5f, 0.0f,  0.0, 0.0, // bottom left
    0.5f, 0.5f, 0.0f,    1.0, 1.0,// top right
    0.5f, -0.5f, 0.0f,   1.0, 0.0// bottom right
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

  auto shaderPath = std::filesystem::path(PROJECT_ROOT)/"src"/"textures"/"churros";
  Shader shader((shaderPath/"vertex.glsl").c_str(), (shaderPath/"frag.glsl").c_str());

  unsigned int tex;
  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  
  int width, height, nrchannels;
  auto staticPath = std::filesystem::path(PROJECT_ROOT) / "static";
  auto texPath = staticPath/"churros.png";
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrchannels, 0);
  if(data) {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB,
      width, height, 0,
      GL_RGB, GL_UNSIGNED_BYTE,
      data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  while(!glfwWindowShouldClose(window)) {
    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setUniformInt("churrosTex", 0);

    glBindVertexArray(texVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}