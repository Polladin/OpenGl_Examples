// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"
#include "cube_sample.h"

#include "vmath.h"

#include <thread>
#include <chrono>
#include <cmath>

GLfloat cube_texcoords[] = {
  0.0, 0.0,
  0.0, 1.0,
  1.0, 0.0,
  1.0, 1.0
};

float vertices[] = {
  0.5f, -0.5f, 0.0f,  // bottom right
  0.5f,  0.5f, 0.0f,  // top right
  -0.5f, -0.5f, 0.0f,  // bottom left
  -0.5f,  0.5f, 0.0f   // top left 
};

/***********************************************************
*     Derive my_application from sb7::application
***********************************************************/
class my_application : public sb7::application
{

  enum BufferObjects
  {
      VAO = 0
    , VBO
    , VBO_2
    , VBE
    , TEXTURE

    , LAST_BUFFER_OBJECTS_ELEM
  };

  enum IntBuffers
  {
      MV_LOCATION

    , LAST_INT_BUFFER_OBJECTS_ELEM
  };

  GLuint glBuffers[ LAST_BUFFER_OBJECTS_ELEM ];
  GLint  glIntBuffers[ LAST_INT_BUFFER_OBJECTS_ELEM ];

  int enableRotate = 0;
  float rotateAngle = 0.0f;
  float scaleFactor = 1.0f;
  float depthFactor = 0.0f;

public:

  void init()
  {
    static const char title[] = "OpenGL SuperBible - Spinny Cube";

    sb7::application::init();

    memcpy( info.title, title, sizeof( title ) );
  }

  // Our rendering function
  void render(double currentTime)
  {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    //glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
    glUseProgram( rendering_program );

      glBindVertexArray( glBuffers[ VAO ] );

      glBindTexture( GL_TEXTURE_2D, glBuffers[ TEXTURE ] );

      vmath::mat4 proj_matrix = vmath::perspective( 60.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 100.0f );

      if ( enableRotate == 1 )
        rotateAngle += 0.5;

      vmath::mat4 mv_rotate_mat = vmath::rotate( rotateAngle, vmath::vec3( 1.0f, 0.0f, 0.0f ) )
                                  * vmath::rotate( rotateAngle, vmath::vec3( 0.0f, 1.0f, 0.0f ) );

      vmath::mat4 scaleMat = vmath::scale( 1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor );

      mv_rotate_mat = scaleMat * vmath::translate( 0.0f, 0.0f, 0.0f + depthFactor ) * mv_rotate_mat;

      // Draw CUBE
      draw_cube( mv_rotate_mat );


    std::this_thread::sleep_for(std::chrono::microseconds(16667));
  }

  void draw_cube( const vmath::mat4 transformation )
  {
    std::array<float, 2> translateFactor = { 0.5f, -0.5f };
    std::array<float, 3> axisFactor = { 1.0f, 0.0f, 0.0f };
    std::array<float, 3> angleFactor = { 0.0f, 90.0f, 90.0f };

    for ( int axisNum = 0; axisNum < 3; ++axisNum )
    {
      for ( int j = 0; j < 2; ++j )
      {
        vmath::mat4 mv_matrix =   vmath::translate(  0.0f + axisFactor[ ( axisNum + 2 ) % 3 ] * translateFactor[ j ]
                                                   , 0.0f + axisFactor[ ( axisNum + 1 ) % 3 ] * translateFactor[ j ]
                                                   , 0.0f + axisFactor[ ( axisNum     ) % 3 ] * translateFactor[ j ] )
                                * vmath::rotate( angleFactor[ axisNum ], vmath::vec3(  axisFactor[ ( axisNum + 1 ) % 3 ]
                                                                                     , axisFactor[ ( axisNum + 2 ) % 3 ]
                                                                                     , 0.0f ) );

        glUniformMatrix4fv( glIntBuffers[ MV_LOCATION ], 1, GL_FALSE, transformation * mv_matrix );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
      }
    }
  }

  /*virtual*/
  void onKey( int key, int action ) override
  {
    if ( ( key == GLFW_KEY_SPACE || key == GLFW_KEY_DOWN ) && action == GLFW_PRESS )
    {
      if ( enableRotate == 0 )
        enableRotate = 1;
      else
        enableRotate = 0;
    }
    if ( ( key == GLFW_KEY_Z ) && action == GLFW_PRESS )
    {
      scaleFactor -= 0.1;
    }
    if ( ( key == GLFW_KEY_A ) && action == GLFW_PRESS )
    {
      scaleFactor += 0.1;
    }
    if ( ( key == GLFW_KEY_X ) && action == GLFW_PRESS )
    {
      depthFactor -= 0.1;
    }
    if ( ( key == GLFW_KEY_S ) && action == GLFW_PRESS )
    {
      depthFactor += 0.1;
    }
  }


  void startup()
  {
    rendering_program = shader::Shaders().create_shader_program();

    // Create VAO
    glGenVertexArrays( 1, &glBuffers[ VAO ] );

    // Create VBO
    glGenBuffers( 2, &glBuffers[ VBO ] );

    // ----------------- Filling VBO --------------------

    // Bind VBO with a rectangle
    glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO ] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO_2 ] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( cube_texcoords ), cube_texcoords, GL_STATIC_DRAW );

    // -------------------- Filling VAO -------------------------------
    glBindVertexArray( glBuffers[ VAO ] );
      // Bind VBO
      glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO ] );

      // Attribute 0
      glVertexAttribPointer( 0, 3 /* number components per vertex */, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void *)0 );
      glEnableVertexAttribArray( 0 );

      // Bind VBO 2
      glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO_2 ] );

      // Attribute 1
      glVertexAttribPointer( 1, 2 /* number components per vertex */, GL_FLOAT, GL_FALSE, 2 * sizeof( float ), (void *)0 );
      glEnableVertexAttribArray( 1 );

      // Unbind VBO
      glBindBuffer( GL_ARRAY_BUFFER, 0 );
    // Unbind VAO
    glBindVertexArray( 0 );

    glPointSize( 5 );

    shader::Shaders().load_texture( glBuffers[ TEXTURE ], "/OpenGL_Tutorial_Texture.jpg" );

    glBindTexture( GL_TEXTURE_2D, glBuffers[ TEXTURE ] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // UNIFORM
    glIntBuffers[ MV_LOCATION ] = glGetUniformLocation( rendering_program, "mv_matrix" );

    glEnable( GL_DEPTH_TEST );

  }
  void shutdown()
  {
    glDeleteVertexArrays( 1, &glBuffers[ VAO ] );
    glDeleteBuffers( 1, &glBuffers[ VBO ] );
    glDeleteBuffers( 1, &glBuffers[ TEXTURE ] );
    glDeleteProgram( rendering_program );
    //glDeleteTextures( 1, &texture_id );
  }

private:

  GLuint rendering_program;
  GLuint vertex_array_object;

};


// Our one and only instance of
//DECLARE_MAIN(my_application);

#include <iomanip>

int main(int argc, const char ** argv)          
{                         
  my_application *app = new my_application;
  app->run(app);                                  
  delete app;                                     
  return 0;                                       
}
