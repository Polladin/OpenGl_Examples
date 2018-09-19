// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"

#include <cmath>



/***********************************************************
*     Derive my_application from sb7::application
***********************************************************/
class my_application : public sb7::application
{
public:
  // Our rendering function
  void render(double currentTime)
  {
    //// Simply clear the window with red
    //static const GLfloat red[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    //glClearBufferfv(GL_COLOR, 0, red);


    //const GLfloat color[] = {   0.0f
    //                          , (float)cos(currentTime) * 0.5f + 0.5f
    //                          , (float)sin(currentTime) * 0.5f + 0.5f
    //                          , 1.0f };

    //glClearBufferfv(GL_COLOR, 0, color);

    //const GLfloat color[] = { (float)sin(currentTime) * 0.5f +
    //  0.5f,
    //  (float)cos(currentTime) * 0.5f +
    //  0.5f,
    //  0.0f, 1.0f };
    const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, color);
    // Use the program object we created earlier for rendering
    glUseProgram(rendering_program);

    // 
    GLfloat attrib[] = { (float)sin(currentTime) * 0.3f, (float)cos(currentTime) * 0.3f, 0.0f, 0.0f };

    // Update the value of input attribute 0
    glVertexAttrib4fv(0, attrib);

    const GLfloat color_point[] = { 1.0f, 0.2f, 0.0f, 1.0f };
    glVertexAttrib4fv( 1, color_point );

    // Scale
    glVertexAttrib1f( 2, 0.5 );

    // Draw one point
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays( GL_LINE_LOOP, 0, 100 );
  }


  void startup()
  {
    rendering_program = shader::Shaders().create_shader_program(); // compile_shaders();
    //glCreateVertexArrays(1, &vertex_array_object);
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glPointSize(10);
  }
  void shutdown()
  {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
    glDeleteVertexArrays(1, &vertex_array_object);
  }

  GLuint compile_shaders(void)
  {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;
    // Source code for vertex shader
    static const GLchar * vertex_shader_source[] =
    {
      "#version 420 core                                                          \n"
      "layout (location = 0) in vec4 offset;                                      \n"
      "layout(location = 1) in vec4 color;                                        \n"
      "out vec4 vs_color;                                                         \n"
      "void main(void)                                                            \n"
      "{                                                                          \n"
      "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),           \n"
      "                                   vec4(-0.25, -0.25, 0.5, 1.0),           \n"
      "                                   vec4( 0.25,  0.25, 0.5, 1.0));          \n"
      "                                                                           \n"
      "    gl_Position = vertices[gl_VertexID] + offset;                          \n"
      "vs_color = color;                                                          \n"
      "}                                                                          \n"
    };

    static const GLchar * vertex_shader_point_source[] =
    {
      "#version 420 core                                                          \n"
      "layout (location = 0) in vec4 offset;                                      \n"
      "layout(location = 1) in vec4 color;                                        \n"
      "layout(location = 2) in float scale;                                        \n"
      "layout(location = 3) in vec4[4] pointTable"
      "out vec4 vs_color;                                                         \n"
      "void main(void)                                                            \n"
      "{                                                                          \n"
      "    const vec4 vertices[] = vec4[](vec4( 0.0, 1.0 * scale, 0.0, 1.0),      \n"
      "                                   vec4( 1.0 * scale, 0.0, 0.0, 1.0),      \n"
      "                                   vec4( 0.0, -1.0 * scale, 0.0, 1.0),     \n"
      "                                   vec4( -1.0 * scale, 0.0, 0.0, 1.0));    \n"
      "                                                                           \n"
      "    gl_Position = pointTable[gl_VertexID] + offset;                        \n"
      "vs_color = color;                                                          \n"
      "}                                                                          \n"
    };

    // Source code for fragment shader
    static const GLchar * fragment_shader_source[] =
    {
      "#version 420 core \n"
      " \n"
      "in vec4 vs_color; \n"
      "out vec4 color; \n"
      " \n"
      "void main(void) \n"
      "{ \n"
      " color = vs_color; \n"
      "} \n"
    };
    // Create and compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glShaderSource( vertex_shader, 1, vertex_shader_point_source, NULL );
    glCompileShader(vertex_shader);
    // Create and compile fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    // Create program, attach shaders to it, and link it
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    // Delete the shaders as the program has them now
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
  }


private:
  GLuint rendering_program;
  GLuint vertex_array_object;
};


// Our one and only instance of
//DECLARE_MAIN(my_application);

int main(int argc, const char ** argv)          
{                                               
  my_application *app = new my_application;
  app->run(app);                                  
  delete app;                                     
  return 0;                                       
}
