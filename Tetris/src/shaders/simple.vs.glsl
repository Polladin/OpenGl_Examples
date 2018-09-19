#version 420 core                                               
                                                              
layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1                                  
                                                  
out vec3 ourColor; // output a color to the fragment shader

uniform int isBorder;   
                                                              
void main(void)                                               
{                                                             
    gl_Position = vec4(aPos, 0.0, 1.0);
    
    if (isBorder == 1)
        ourColor = vec3(0.0f, 0.0f, 0.0f);
    else
        ourColor = aColor; // set ourColor to the input color we got from the vertex data
}                                                             