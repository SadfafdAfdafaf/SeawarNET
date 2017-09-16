#version 130
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_Texture;

varying vec3 N;
varying vec3 V;
varying vec3 L;
out vec2 Textcord;

uniform int dx;
uniform int dy;

const vec3 lightPos = vec3(100.0,250.0,100.0);

void main(void) {
  vec3 pos = in_Position * 5.0;
  vec3 norm = normalize(gl_NormalMatrix * in_Normal);
  pos = pos + vec3(dx, 2, dy);
  
  gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
  
  V = vec3(gl_ModelViewMatrix * vec4(pos, 1.0)); 
  L = vec3(gl_ModelViewMatrix * vec4(lightPos, 1.0)); 
  N = norm;
  Textcord = in_Texture;
}