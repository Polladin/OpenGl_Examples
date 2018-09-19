#version 420 core                                                
                                                                 
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture;

void main(void) 
{
	//FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
	FragColor = texture(texture, TexCoord);

}                              