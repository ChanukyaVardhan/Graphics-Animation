#version 130

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 texCoord;

out vec3 fNormal;
out vec4 fPosition;
out vec4 glpos;
out vec4 light_component;

uniform mat4 uModelViewMatrix;

varying vec2 tex;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  tex = texCoord;
  glpos = gl_Position;
  fNormal = vNormal;
  fPosition = vPosition;
}