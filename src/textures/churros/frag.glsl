#version 330 core

in vec2 Uv;
out vec4 FragColor;

uniform sampler2D happyTex;
uniform sampler2D woodTex;
uniform float brightness;

void main() {
  FragColor = mix(texture(woodTex, Uv), texture(happyTex, Uv), 0.5) * brightness; 
}