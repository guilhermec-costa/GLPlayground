#version 330 core

in vec2 Uv;
out vec4 FragColor;

uniform sampler2D churrosTex;

void main() {
  FragColor = texture(churrosTex, Uv);
}