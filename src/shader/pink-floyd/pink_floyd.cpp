#include "gl_utils.h"
#include "rectangle.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <optional>

void pinkFloyd(GLFWwindow *window,std::optional<InputProcessor> inputProcessor) {

  unsigned int mainTriangleVBO, mainTriangleVAO;
  unsigned int secondaryTriangleVBO, secondaryTriangleVAO;
  unsigned int innerTriangleVBO, innerTriangleVAO;
  unsigned int fadedTriangleVBO, fadedTriangleVAO;

  float mainTriangleVertexData[] = {
    // position         // color
    0.0f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f
  };

  float secondaryTriangleVertexData[] = {
    // position         // color
    0.0f, 0.49f, 0.0f,   0.18f, 0.18f, 0.18f,
    -0.45f, -0.47f, 0.0f, 0.18f, 0.18f, 0.18f,
    0.45f, -0.47f, 0.0f,  0.18f, 0.18f, 0.18f
  };

  float innerTriangleVertexData[] = {
    // position         // color
    0.0f, 0.47f, 0.0f,   0.f, 0.f, 0.f,
    -0.44f, -0.46f, 0.0f, 0.f, 0.f, 0.f,
    0.44f, -0.46f, 0.0f,  0.f, 0.f, 0.f
  };

  float fadedTriangleVertexData[] = {
    // position         // color
    -0.2f, 0.1f, 0.0f,   1.0f, 1.0f, 1.0f,
    0.128f,  0.2f, 0.0f,   0.08f, 0.08f, 0.08f,
    0.22f,  0.0f, 0.0f,   0.f, 0.f, 0.f
  };

  glGenBuffers(1, &mainTriangleVBO);
  glGenVertexArrays(1, &mainTriangleVAO);
  glBindVertexArray(mainTriangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mainTriangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mainTriangleVertexData), mainTriangleVertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  glGenBuffers(1, &secondaryTriangleVBO);
  glGenVertexArrays(1, &secondaryTriangleVAO);
  glBindVertexArray(secondaryTriangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, secondaryTriangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondaryTriangleVertexData), secondaryTriangleVertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  glGenBuffers(1, &innerTriangleVBO);
  glGenVertexArrays(1, &innerTriangleVAO);
  glBindVertexArray(innerTriangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, innerTriangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(innerTriangleVertexData), innerTriangleVertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  glGenBuffers(1, &fadedTriangleVBO);
  glGenVertexArrays(1, &fadedTriangleVAO);
  glBindVertexArray(fadedTriangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, fadedTriangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fadedTriangleVertexData), fadedTriangleVertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  float leftLineVertexData[] = {

    -0.18f, 0.11f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -0.22f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.20f, 0.09f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom-right
  };

  unsigned int leftLineIndices[] = {
    0, 1, 2,
    0, 2, 3
  };

  Rectangle leftRect(
    leftLineVertexData, sizeof(leftLineVertexData),
    leftLineIndices, sizeof(leftLineIndices)
  );

  constexpr int rectsNr = 6;
  Rectangle rightRectangles[rectsNr];

  float rainbowColors[6][3] = {
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.5f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.4f, 1.0f},
    {0.6f, 0.0f, 1.0f}
  };

  float startVertexRefX = fadedTriangleVertexData[6] + 0.023;
  float endVertexRefX = fadedTriangleVertexData[12] + 0.023;
  float startVertexRefY = fadedTriangleVertexData[7];
  float endVertexRefY = fadedTriangleVertexData[13];
  const float deltaWidth = std::abs(startVertexRefX - endVertexRefX);
  const float deltaHeight = std::abs(startVertexRefY - endVertexRefY);
  const float widthOffssetPerRect = deltaWidth / rectsNr; 
  const float heightOffsetPerRect = deltaHeight / rectsNr; 

  for(int i=0; i<rectsNr; i++) {
    float angleOffset = 0.18;
    float upperLeftVertexPositionX = startVertexRefX + (widthOffssetPerRect * i);
    float upperLeftVertexPositionY = startVertexRefY - (heightOffsetPerRect * i);
    float upperRightVertexPositionX = 1;
    float upperRightVertexPositionY = startVertexRefY - (heightOffsetPerRect * i) - angleOffset;

    float bottomLeftVertexPositionX = startVertexRefX + (widthOffssetPerRect * (i + 1));
    float bottomLeftVertexPositionY = startVertexRefY - (heightOffsetPerRect * (i + 1));
    float bottomRightVertexPositionX = 1;
    float bottomRightVertexPositionY = startVertexRefY - (heightOffsetPerRect * (i + 1)) - angleOffset;

    float red   = rainbowColors[i][0];
    float green = rainbowColors[i][1];
    float blue  = rainbowColors[i][2];

    float colorfulRect[24] = {
      upperLeftVertexPositionX, upperLeftVertexPositionY, 1.0f, red, green, blue, 
      upperRightVertexPositionX, upperRightVertexPositionY, 1.0f, red, green, blue, 
      bottomLeftVertexPositionX, bottomLeftVertexPositionY, 1.0f, red, green, blue, 
      bottomRightVertexPositionX, bottomRightVertexPositionY, 1.0f, red, green, blue, 
    };

    unsigned int rectIndices[] = {
      0, 1, 2,
      1, 2, 3
    };

    rightRectangles[i] = Rectangle(
      colorfulRect, sizeof(colorfulRect),
      rectIndices, sizeof(rectIndices)
    );
  }

  float redRectVertexData[] = {

    -0.18f, 0.11f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -0.22f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.20f, 0.09f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom-right
  };

  unsigned int redRectVertexIndices[] = {
    0, 1, 2,
    0, 2, 3
  };

  auto pinkFloydDir = std::filesystem::path(PROJECT_ROOT) / "src" / "shader" / "pink-floyd";
  Shader mainTriangleShader((pinkFloydDir/"main_triangle.vs").c_str(),(pinkFloydDir/"main_triangle.fs").c_str());
  
  while(!glfwWindowShouldClose(window)) {
    if(inputProcessor.has_value()) {
      inputProcessor.value()(window);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    mainTriangleShader.use();

    glBindVertexArray(mainTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(secondaryTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(innerTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(fadedTriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    leftRect.draw();

    for(const auto& rect : rightRectangles) {
      rect.draw();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}