#version 420 core                                                
                                                                 
out vec4 color;                 
in vec3 ourColor;                                           
                                                                 
uniform vec3 color_in;                            
                                                                 
void main(void)                                                  
{                                                                
    color = vec4(ourColor, 1.0f);                                         
}                                                                