#version 130
in  vec3 in_Pos;
out vec3 V;

void main(void) {
  
  mat4 mv = gl_ModelViewMatrix;
  mv[3].xyz = vec3(0.0);
  gl_Position = gl_ProjectionMatrix * mv * vec4(in_Pos, 1.0);
  V = in_Pos;

}