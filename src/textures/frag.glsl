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

  // this inverts the uv in the Y axis
  vec2 invertedUv = vec2(Uv.x, 1.0 - Uv.y); 

  /*e.g pre wrapper
    1.0 - 0.8 = 0.2
    1.0 - 2.1 = -1.1
  */

  /*e.g post wrapper
    fract(0.2) = 0.2 - floor(0.2) -> 0.2 - 0
    fract(-1.1) = -1.1 - floor(-1.1) -> -1.1 - (-2) = -1.1 + 2 = 0.9
  */

  FragColor = mix(texture(woodWall, Uv),texture(happyFace, invertedUv),0.3);
}