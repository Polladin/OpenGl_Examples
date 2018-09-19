// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"
#include "cube_sample.h"
#include "circle.h"

#include "vmath.h"

#include <thread>
#include <chrono>
#include <cmath>


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
    , EBO
    , VBE
    , TEXTURE
    , TEXTURE_2
    , TEXTURE_3

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
  float scaleFactor = 0.7f;
  float depthFactor = -2.0f;

  static const unsigned CIRCLE_RESOLUTION = 40;
  static const unsigned PROTECTOR_RESOLUTION = 100;

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
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
    if ( enableRotate == 1 )
      rotateAngle += 0.2f;

    vmath::mat4 proj_matrix = vmath::perspective( 60.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 100.0f );

    vmath::mat4 mv_rotate_mat = vmath::rotate( rotateAngle * 0.0f, vmath::vec3( 0.0f, 1.0f, 0.0f ) );
    vmath::mat4 scaleMat = vmath::scale( 1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor );
    mv_rotate_mat = proj_matrix * scaleMat * vmath::translate( 0.0f, 0.0f, 0.0f + depthFactor ) * mv_rotate_mat;

    glUseProgram( rendering_program );

    glUniformMatrix4fv( glIntBuffers[ MV_LOCATION ], 1, GL_FALSE, mv_rotate_mat 
                                                                  * vmath::rotate( -rotateAngle * 10, vmath::vec3( 1.0f, 0.0f, 0.0f ) )
                                                                  * vmath::rotate( -rotateAngle * 10, vmath::vec3( 0.0f, 0.0f, 1.0f ) ) );

      glBindVertexArray( glBuffers[ VAO ] );

      for ( auto circleDepth : { 0.2f, -0.2f } )
      {
        auto vexCircle = CircleCoords::get_circle_vertex( CIRCLE_RESOLUTION, circleDepth );
        auto texCircle = CircleCoords::get_circle_texture_coords( CIRCLE_RESOLUTION );
        auto idxCircle = CircleCoords::get_circle_index( CIRCLE_RESOLUTION );

        // Fill VBO
        glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vexCircle[ 0 ] ) * vexCircle.size(), vexCircle.data(), GL_DYNAMIC_DRAW );

        // Fill Tex idx (VBO_2)
        glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO_2 ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( texCircle[ 0 ] ) * texCircle.size(), texCircle.data(), GL_DYNAMIC_DRAW );

        // Fill EBO
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( idxCircle[ 0 ] ) * idxCircle.size(), idxCircle.data(), GL_DYNAMIC_DRAW );

        glBindTexture( GL_TEXTURE_2D, glBuffers[ TEXTURE_3 ] );

        glDrawElements( GL_TRIANGLES, CIRCLE_RESOLUTION * 3, GL_UNSIGNED_INT, 0 );
      }

      // ----------------- Protector ----------------------

      auto vaxProtector = CircleCoords::get_protector_vertex( PROTECTOR_RESOLUTION, 0.2f );
      auto texProtector = CircleCoords::get_protector_texture_coords( PROTECTOR_RESOLUTION, 0.05f);
      
      // Fill VBO
      glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO ] );
      glBufferData( GL_ARRAY_BUFFER, sizeof( vaxProtector[ 0 ] ) * vaxProtector.size(), vaxProtector.data(), GL_DYNAMIC_DRAW );

      // Fill Tex idx (VBO_2)
      glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO_2 ] );
      glBufferData( GL_ARRAY_BUFFER, sizeof( texProtector[ 0 ] ) * texProtector.size(), texProtector.data(), GL_DYNAMIC_DRAW );

      glBindTexture( GL_TEXTURE_2D, glBuffers[ TEXTURE ] );

      glDrawArrays( GL_TRIANGLE_STRIP, 0, PROTECTOR_RESOLUTION * 2 + 2 );

    std::this_thread::sleep_for(std::chrono::microseconds(16667));
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
      depthFactor -= 0.5;
    }
    if ( ( key == GLFW_KEY_S ) && action == GLFW_PRESS )
    {
      depthFactor += 0.5;
    }
  }


  void startup()
  {
    auto vexCircle = CircleCoords::get_circle_vertex( CIRCLE_RESOLUTION, 0.2 );
    auto texCircle = CircleCoords::get_circle_texture_coords( CIRCLE_RESOLUTION );
    auto idxCircle = CircleCoords::get_circle_index( CIRCLE_RESOLUTION );

    for ( int i = 0; i < texCircle.size() / 2; ++i )
    {
      std::cout << "x: " << texCircle[ i * 2 ] << "y: " << texCircle[ i * 2 + 1 ] << std::endl; // "z: " << texCircle[ i * 3 + 2 ] << std::endl;
    }


    rendering_program = shader::Shaders().create_shader_program();

    // Create VAO
    glGenVertexArrays( 1, &glBuffers[ VAO ] );

    // Create VBO(_2) and EBO
    glGenBuffers( 3, &glBuffers[ VBO ] );

    // ----------------- Initializing VBO --------------------

    // Init VBO
    glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO ] );
    glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );

    // Init VBO_2
    glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO_2 ] );
    glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );

    // Init EBO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glBuffers[ EBO ] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );

    // -------------------- Filling VAO -------------------------------
    glBindVertexArray( glBuffers[ VAO ] );
      // Bind VBO and EBO
      glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO ] );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, glBuffers[ EBO ] );

      // Attribute 0
      glVertexAttribPointer( 0, 3 /* number components per vertex */, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void *)0 );
      glEnableVertexAttribArray( 0 );

      // Bind VBO 2
      glBindBuffer( GL_ARRAY_BUFFER, glBuffers[ VBO_2 ] );

      // Attribute 1
      glVertexAttribPointer( 1, 2 /* number components per vertex */, GL_FLOAT, GL_FALSE, 2 * sizeof( float ), (void *)0 );
      glEnableVertexAttribArray( 1 );

      // Unbind VBE and VBO
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
      glBindBuffer( GL_ARRAY_BUFFER, 0 );
    // Unbind VAO
    glBindVertexArray( 0 );

    glPointSize( 5 );

    shader::Shaders().load_texture( glBuffers[ TEXTURE ], "/tex9.png" );
    shader::Shaders().load_texture( glBuffers[ TEXTURE_2 ], "/tex2.jpg" );
    shader::Shaders().load_texture( glBuffers[ TEXTURE_3 ], "/tex4.png" );

    for ( int i = 0; i < 3; ++i )
    {
      glBindTexture( GL_TEXTURE_2D, glBuffers[ TEXTURE + i ] );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }

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
