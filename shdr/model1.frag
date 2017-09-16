#version 130
precision highp float;

out vec4 gl_FragColor;
varying vec3 N;
varying vec3 V;
varying vec3 L;
in vec2 Textcord;

uniform samplerCube cube2;
uniform sampler2D Texture;

const vec4 specColor = vec4(1.0, 1.0, 1.0, 1.0);
const float shininess = 1024.0;
const float n1 = 1.00029;
const float n2 = 1.33;

vec2 reflectance(in vec3 d, in vec3 n, in vec3 rd, in float n1, in float n2) {
 float i = abs(dot(d, n));
 float t = abs(dot(rd, n));
    
 vec2 r = vec2((n1 * i - n2 * t) / (n1 * i + n2 * t), (n1 * t - n2 * i) / (n1 * t + n2 * i));
 return r * r;
}


void main(void) {
  vec3 normal = normalize(N);
  vec3 lightDir = normalize(L - V);

  float lambertian = max(dot(lightDir,normal), 0.0);
  float specular = 0.0;
  
  vec3 viewDir = normalize(-V);
  vec3 halfDir = normalize(lightDir + viewDir);
  float specAngle = max(dot(halfDir, normal), 0.0);
  specular = pow(specAngle, shininess);
  
  vec3 d = normalize(V);
  vec3 rfl = reflect(d, normal);
  vec3 rfr = refract(d, normal, n1 / n2);
  vec2 frenr = reflectance(rfl, normal, rfr, n1, n2);
  float krefl = (frenr.x + frenr.y) * 0.5;
  vec4 diff = lambertian * texture2D(Texture, Textcord);
  vec4 spec = specular * specColor;
  vec4 refl = textureCube(cube2, rfl);
  vec4 colorLinear = mix(diff, refl, krefl) + spec;
  colorLinear.a = 1.0;
  //gl_FragColor = texture2D(Texture, Textcord);
  gl_FragColor = colorLinear; 
}