// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"
#include "cube_sample.h"

#include "vmath.h"

#include <cmath>



/***********************************************************
*     Derive my_application from sb7::application
***********************************************************/
class my_application : public sb7::application
{
public:

  GLuint          vao;
  GLuint          buffer;
  GLint           mv_location;
  GLint           proj_location;

  float           aspect;
  vmath::mat4     proj_matrix;

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

    glViewport( 0, 0, info.windowWidth, info.windowHeight );
    glClearBufferfv( GL_COLOR, 0, green );
    glClearBufferfv( GL_DEPTH, 0, &one );

    glUseProgram( rendering_program );

    glUniformMatrix4fv( proj_location, 1, GL_FALSE, proj_matrix );

    //for ( int i = 0; i < 24; ++i )
    //{

    //  float f = i + (float)currentTime * 0.3f;

    //  vmath::mat4 mv_matrix = vmath::translate( 0.0f, 0.0f, -20.0f ) *
    //    vmath::translate( sinf( 2.1f * f ) * 0.5f,
    //      cosf( 1.7f * f ) * 0.5f,
    //      sinf( 1.3f * f ) * cosf( 1.5f * f ) * 2.0f ) *
    //    vmath::rotate( (float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f ) *
    //    vmath::rotate( (float)currentTime * 21.0f, 1.0f, 0.0f, 0.0f );

    //  //vmath::mat4 mv_matrix = vmath::translate( 0.25f, 0.25f, -8.0f )
    //  //                      * vmath::rotate( (float)currentTime * 15.0f, 1.0f, 1.0f, 0.0f );

    //  glUniformMatrix4fv( mv_location, 1, GL_FALSE, mv_matrix );
    //  glDrawArrays( GL_TRIANGLES, 0, 36 );
    //}

    int i;
    for ( i = 0; i < 24; i++ )
    {
      float f = (float)i + (float)currentTime * 0.3f;
      vmath::mat4 mv_matrix = vmath::translate( 0.0f, 0.0f, -6.0f ) *
        vmath::rotate( (float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f ) *
        vmath::rotate( (float)currentTime * 21.0f, 1.0f, 0.0f, 0.0f ) *
        vmath::translate( sinf( 2.1f * f ) * 2.0f,
          cosf( 1.7f * f ) * 2.0f,
          sinf( 1.3f * f ) * cosf( 1.5f * f ) * 2.0f );
      glUniformMatrix4fv( mv_location, 1, GL_FALSE, mv_matrix );
      glDrawArrays( GL_TRIANGLES, 0, 36 );
    }


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

    mv_location = glGetUniformLocation( rendering_program, "mv_matrix" );
    proj_location = glGetUniformLocation( rendering_program, "proj_matrix" );

    aspect = (float)info.windowWidth / (float)info.windowHeight;
    proj_matrix = vmath::perspective( 25.0f, aspect, 0.1f, 1000.0f );

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER,
      sizeof( vertex_positions ),
      vertex_positions,
      GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 0 );

    //glEnable( GL_CULL_FACE );
    //glFrontFace( GL_CW );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }
  void shutdown()
  {
    glDeleteVertexArrays( 1, &vao );
    glDeleteProgram( rendering_program );
    glDeleteBuffers( 1, &buffer );
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
