#version 130
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_Texture;
uniform float timer;
varying vec3 N;
varying vec3 V;
varying vec3 L;
out vec2 Textcord;

uniform sampler2D myTexture;

const vec3 lightPos = vec3(100.0,250.0,100.0);

vec3 get_vert_pos(in float x, in float z) {
  float y = texture2D(myTexture, vec2(x, z + timer * 0.8) * 0.01).y +
            texture2D(myTexture, vec2(x + timer * 0.6, z) * 0.01).y;
  
  return vec3(x, y * 2.0, z);
}

vec3 get_normal(in vec3 b, in float dx, in float dz) {
    return cross(normalize(get_vert_pos(b.x + dx, b.z) - b), normalize(get_vert_pos(b.x, b.z + dz) - b));
}

void main(void) {
  vec3 pos = get_vert_pos(in_Position.x, in_Position.z);
  vec3 norm = get_normal(pos, -0.5, 0.5);
    
  gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
      
  V = vec3(gl_ModelViewMatrix * vec4(pos, 1.0));
  N = vec3(gl_NormalMatrix * norm);
  L = vec3(gl_ModelViewMatrix * vec4(lightPos, 1.0));
  Textcord = in_Texture;
}