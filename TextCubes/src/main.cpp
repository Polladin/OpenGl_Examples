// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"
#include "cube_sample.h"

#include "vmath.h"

#include <thread>
#include <chrono>
#include <cmath>


class Jack
{
public:
    struct Slot
    {
        float xAngle { 0.0f };
        float xStep  { 500.0f };
    };



    Jack( unsigned countSlots )
    {
        m_slots.resize( countSlots );
    }

    void step(double deltaTime)
    {
        ++m_steps;

        timeSpent += deltaTime;

        for (int i = 0; i < 5; ++i)
            if ( timeSpent > 2 * (i + 1) )
                m_slots[i].xStep = 100.0f;

        for ( auto & el : m_slots )
            el.xAngle += el.xStep * deltaTime;
    }

public:

    double timeSpent{0.0};
    unsigned m_steps{0};
    std::vector<Slot> m_slots;
};


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
  GLint           elem_color;

  Jack jack{5};

  double prevTime;

  std::vector<GLfloat> cubeCoords;

  float           aspect;
  vmath::mat4     proj_matrix;



  void init()
  {
    static const char title[] = "OpenGL SuperBible - Spinny Cube";

	cubeCoords = create_cube( 0.5f, { -0.25f, 0.25f, -0.25f } );

    sb7::application::init();

    memcpy( info.title, title, sizeof( title ) );
  }

  // Our rendering function
  void render(double currentTime)
  {
	  static std::array<GLfloat, 3> cl{ 0.4f, 1.0f, 0.5f };
      static std::array<GLfloat, 3> cl2{ 0.4f, 0.2f, 0.5f };

      double deltaTime = currentTime - prevTime;
      prevTime = currentTime;
      std::cout << "Delta time : " << deltaTime << std::endl;
      if ( deltaTime < 100 )
        jack.step( deltaTime );

	  // Color
	  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	  glClear( GL_COLOR_BUFFER_BIT );

	  glUseProgram( rendering_program );

	  // Dynamic Objects
	  glBindVertexArray( vao );

	  
	  glBufferData( GL_ARRAY_BUFFER, sizeof( cubeCoords[ 0 ] ) * cubeCoords.size(), cubeCoords.data(), GL_DYNAMIC_DRAW );
	  
      float scaleFactor = 0.5;

      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

      vmath::mat4 mv_matrix = vmath::translate( -0.8f, 0.0f, 0.0f )
                              * vmath::scale( 1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor );

      for ( int i = 0; i < 5; ++i )
      {
          vmath::mat4 new_mv_matrix =   vmath::translate( 0.4f * i, 0.0f, 0.0f )
                                      * mv_matrix
                                      * vmath::rotate( jack.m_slots[i].xAngle, vmath::vec3( 1.0f, 0.0f, 0.0f ) );
                                      //* vmath::rotate( ( /*(float)currentTime */ 20.0f * i ), vmath::vec3( 1.0f, 0.0f, 0.0f ) )
                                      //* vmath::rotate( ( /*(float)currentTime */ 30.0f * i ), vmath::vec3( 0.0f, 1.0f, 0.0f ) );

          glUniformMatrix4fv( mv_location, 1, GL_FALSE, new_mv_matrix );
          glDrawArrays( GL_TRIANGLES, 0, cubeCoords.size() / 3 );
      }

	  glBindVertexArray( 0 );

      std::this_thread::sleep_for(std::chrono::microseconds(16667));
  }

  void startup()
  {
    rendering_program = shader::Shaders().create_shader_program();

    mv_location = glGetUniformLocation( rendering_program, "mv_matrix" );
    proj_location = glGetUniformLocation( rendering_program, "proj_matrix" );
	elem_color = glGetUniformLocation( rendering_program, "color_in" );

    aspect = (float)info.windowWidth / (float)info.windowHeight;
    proj_matrix = vmath::perspective( 25.0f, aspect, 0.1f, 1000.0f );

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );
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

#include <iomanip>

int main(int argc, const char ** argv)          
{                          
    //std::vector<GLfloat> cubeSamples = create_cube( 0.5f, { 0.0f, 0.0f, 0.0f } );

    //std::cout << "Size : " << cubeSamples.size() << std::endl;

    //for ( int i = 0; i < 6; ++i ) //cubeSamples.size() / 3
    //{
    //    std::cout   << std::setw(10) << cubeSamples[ i * 3      ] << ", "
    //                << std::setw(10) << cubeSamples[ i * 3  + 1 ] << ", " 
    //                << std::setw(10) << cubeSamples[ i * 3  + 2 ] << "\n";
    //}

    //std::cin.get();

  my_application *app = new my_application;
  app->run(app);                                  
  delete app;                                     
  return 0;                                       
}
