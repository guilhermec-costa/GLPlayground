#version 330 core

in vec3 interpolatedColor;
out vec4 FragColor;

void main() {
  FragColor = vec4(interpolatedColor, 1.0); 
}