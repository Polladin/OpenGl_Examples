#version 420 core                                               
                                                              
in vec4 position;                                
out vec3 ourColor;                 
                                                              
uniform mat4 mv_matrix;                                       
uniform mat4 proj_matrix;                                     
                                                              
void main(void)                                               
{                                                             
    //gl_Position = proj_matrix * mv_matrix * position;
	gl_Position = mv_matrix * position;

	if (gl_VertexID % 2 == 0)
		ourColor = vec3(0.4f, 1.0f, 0.5f);
	else
		ourColor = vec3(0.4f, 0.2f, 0.5f);
}                                                             