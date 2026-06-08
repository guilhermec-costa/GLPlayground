#version 330 core

in vec3 Color;
in vec2 Uv;
out vec4 FragColor;

uniform sampler2D Texture;

void main() {
  FragColor = texture(Texture, Uv); 
}