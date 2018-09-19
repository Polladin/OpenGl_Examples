// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"
#include "cube_sample.h"

#include "vmath.h"

#include <cmath>


// Set up vertex data (and buffer(s)) and attribute pointers
GLfloat vertices[] = {
    // Positions          // Colors           // Texture Coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
};
GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
};


/***********************************************************
*     Derive my_application from sb7::application
***********************************************************/
class my_application : public sb7::application
{
public:

  GLuint          vao, VAO, VBO, EBO;
  GLuint          buffer;
  GLint           mv_location;
  GLint           proj_location;
  GLuint          texture;
  GLuint          texture2;

  float           aspect;
  vmath::mat4     proj_matrix;

  struct TextureParams
  {
      int width, height;
  };

  TextureParams tex1;

  void init()
  {
    static const char title[] = "OpenGL SuperBible - Spinny Cube";

    sb7::application::init();

    memcpy( info.title, title, sizeof( title ) );
  }

  // Our rendering function
  void render(double currentTime)
  {

    static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    static const GLfloat one = 1.0f;

    // Clear the colorbuffer
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );


    // Bind Texture
    //glBindTexture( GL_TEXTURE_2D, texture );

    // Activate shader
    glUseProgram( rendering_program );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glUniform1i( glGetUniformLocation( rendering_program, "ourTexture1" ), 0 );
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, texture2 );
    glUniform1i( glGetUniformLocation( rendering_program, "ourTexture2" ), 1 );

    glBindVertexArray( VAO );

    //for (int i = 0; i < 6; ++i)
    glDrawElements( GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0 );

    glBindVertexArray( 0 );

    //// Swap the screen buffers
    //glfwSwapBuffers( window );

  }

  void onResize( int w, int h )
  {
    sb7::application::onResize( w, h );

    aspect = (float)w / (float)h;
    proj_matrix = vmath::perspective( 50.0f, aspect, 0.1f, 1000.0f );
  }

  void startup()
  {
    rendering_program = shader::Shaders().create_shader_program();

    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

    glBindVertexArray( VAO );

    //std::vector<GLfloat> cubeSample = create_cube( 0.2f, {0.0f, 0.0f, 0.0f} );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    //glBufferData( GL_ARRAY_BUFFER, cubeSample.size() * sizeof( cubeSample[0] ), cubeSample.data(), GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );
    // Color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 1 );
    // TexCoord attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 ); // Unbind VAO

    shader::Shaders().load_texture( texture, "/container.jpg" );
    shader::Shaders().load_texture( texture2, "/awesomeface.png" );
  }

  void shutdown()
  {
    glDeleteVertexArrays( 1, &vao );
    glDeleteProgram( rendering_program );
    glDeleteBuffers( 1, &buffer );

    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
    glfwTerminate();
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
