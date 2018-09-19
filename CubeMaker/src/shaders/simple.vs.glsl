#version 420 core                                               
                                                              
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 mv_matrix;

out vec2 TexCoord;

void main(void) 
{
  gl_Position = mv_matrix * vec4(aPos, 1.0);
  TexCoord = vec2(1.0f, 1.0f) - aTexCoord;
}                                               