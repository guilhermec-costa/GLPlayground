#pragma once

#include <cstddef>

class Rectangle {
public:
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int verticesNr;

public:
  Rectangle();
  Rectangle(float* vertexData,size_t vertexDataS, unsigned int* indices, size_t indicesS);
  void draw() const;
  ~Rectangle() {};
};