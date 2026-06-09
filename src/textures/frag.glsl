#version 330 core

in vec3 Color;
in vec2 Uv;
out vec4 FragColor;

// sampler2D stores the texture unit index to sample from.
// When texture(...) is called, OpenGL samples texels from the
// texture currently bound to that texture unit.
uniform sampler2D woodWall;
uniform sampler2D happyFace;

void main() {
  // texture(sampler argument, uv argument): outputs the texel color (VEC4)
  // FragColor = texture(woodWall) * vec4(Color, 1.0);  // mix colors
  FragColor = mix(texture(woodWall, Uv),texture(happyFace, Uv),0.3);
}