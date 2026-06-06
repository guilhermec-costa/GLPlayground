#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform float offset;

out vec3 interpolatedColor;
void main() {
  gl_Position = vec4(pos.x + offset, pos.yz, 1.0);
  interpolatedColor = color;
}