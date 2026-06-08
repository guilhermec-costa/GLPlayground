#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

uniform float offset;

out vec3 Color;
out vec2 Uv;

void main() {
  gl_Position = vec4(pos.x + offset, pos.yz, 1.0);
  Color = color;
  Uv = uv;
}