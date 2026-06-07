#include <algorithm>
#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rectangle.h"

Rectangle::Rectangle() {};

Rectangle::Rectangle(float* vertexData,size_t vertexDataS, unsigned int* indices, size_t indicesS) {
  this->verticesNr = std::max(0, static_cast<int>((indicesS/sizeof(unsigned int))));
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->EBO);
  glGenVertexArrays(1, &this->VAO);

  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexDataS, vertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesS, indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
} 

void Rectangle::draw() const  {
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glDrawElements(GL_TRIANGLES, this->verticesNr, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}