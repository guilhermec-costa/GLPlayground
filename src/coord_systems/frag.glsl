#version 330 core

in vec2 UV;
in vec3 COLOR;
out vec4 FragColor;

uniform sampler2D TEXTURE1;
uniform sampler2D TEXTURE2;
uniform float time;

void main() {
  FragColor = mix(texture(TEXTURE1, UV), texture(TEXTURE2, UV), 0.8);
}