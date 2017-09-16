#version 130
precision highp float;

out vec4 gl_FragColor;
in vec3 V;

uniform samplerCube cube;

void main(void) {
  gl_FragColor = textureCube(cube,  normalize(V)); 
}