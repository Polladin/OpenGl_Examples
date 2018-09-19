// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"
#include "cube_sample.h"

#include "vmath.h"

#include <cmath>

#include "tetris\tetris.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "tetris\opengl_wrapper.h"

float vertices[] = {
  // positions         // colors
  0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
  -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
  0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

};

GLfloat border[] =
{
  -0.8f , -0.8f, 1.0f, 1.0f, 1.0f,
  -0.8f ,  0.8f, 1.0f, 1.0f, 1.0f,
   0.4f ,  0.8f, 1.0f, 1.0f, 1.0f,
   0.4f , -0.8f, 1.0f, 1.0f, 1.0f
};

TetrisOpenGLWrapper tetris;
std::mutex tetrisMutex;
std::atomic_bool isStoped{ false };
std::atomic<int> delayMS{400};
std::condition_variable cond_var;
std::mutex mCondVar;


void step_in_tetris()
{
  while ( !isStoped )
  {
    {
      std::lock_guard<std::mutex> lockTetris( tetrisMutex );
      tetris.step();
    }

    std::unique_lock<std::mutex> lk( mCondVar );
    cond_var.wait_for( lk, std::chrono::milliseconds( delayMS.load() ) );
    //std::this_thread::sleep_for( std::chrono::milliseconds( delayMS.load() ) );
  }
}


/***********************************************************
*     Derive my_application from sb7::application
***********************************************************/
class my_application : public sb7::application
{
public:

  GLuint          vao;
  GLuint          vaoStatic;
  GLuint          buffer;
  GLint           mv_location;
  GLint           proj_location;
  GLint           isBorder;

  float           aspect;
  vmath::mat4     proj_matrix;

  void init()
  {
    static const char title[] = "OpenGL SuperBible - Spinny Cube";

    sb7::application::init();

    memcpy( info.title, title, sizeof( title ) );
  }

  void render(double currentTime)
  {    
    glUseProgram( rendering_program );

    // Color
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    // Static objects
    glBindVertexArray( vaoStatic );
    glDrawArrays( GL_LINE_LOOP, 0, 4);

    // Dynamic Objects
    glBindVertexArray( vao );

    tetrisMutex.lock();
    auto vex = tetris.get_vertex_and_colors();
    auto elems = tetris.get_elemts_vector();
    tetrisMutex.unlock();

    glBufferData( GL_ARRAY_BUFFER        , sizeof( vex[ 0 ] )   * vex.size()  , vex.data()  , GL_DYNAMIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( elems[ 0 ] ) * elems.size(), elems.data(), GL_DYNAMIC_DRAW );

    glUniform1i( isBorder, 1 );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDrawElements( GL_TRIANGLES, elems.size(), GL_UNSIGNED_INT, NULL );

    tetrisMutex.lock();
    vex = tetris.get_vertex_and_colors(true);
    elems = tetris.get_elemts_vector();
    tetrisMutex.unlock();
    glBufferData( GL_ARRAY_BUFFER, sizeof( vex[ 0 ] )   * vex.size(), vex.data(), GL_DYNAMIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( elems[ 0 ] ) * elems.size(), elems.data(), GL_DYNAMIC_DRAW );

    glUniform1i( isBorder, 0 );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements( GL_TRIANGLES, elems.size(), GL_UNSIGNED_INT, NULL );
  }

  /*virtual*/ 
  void onKey( int key, int action ) override
  {
    if ( ( key == GLFW_KEY_Z ) && action == GLFW_PRESS )
    {
      delayMS.store( 10 );
      cond_var.notify_all();
    }
    else if ( ( key == GLFW_KEY_Z ) && action == GLFW_RELEASE )
    {
      delayMS.store( 400 );
    }
    if ( (key == GLFW_KEY_SPACE || key == GLFW_KEY_DOWN) && action == GLFW_PRESS )
    {
      delayMS.store( 50 );
      cond_var.notify_all();
    }
    else if ( (key == GLFW_KEY_SPACE || key == GLFW_KEY_DOWN ) && action == GLFW_RELEASE )
    {
      delayMS.store( 400 );
    }
    else if ( (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS )
    {
      std::lock_guard<std::mutex> lockTetris( tetrisMutex );
      tetris.step_left();
    }
    else if ( ( key == GLFW_KEY_RIGHT || key == GLFW_KEY_D ) && action == GLFW_PRESS )
    {
      std::lock_guard<std::mutex> lockTetris( tetrisMutex );
      tetris.step_right();
    }
    else if ( ( key == GLFW_KEY_B ) && action == GLFW_PRESS )
    {
      std::lock_guard<std::mutex> lockTetris( tetrisMutex );
      tetris.rotate_right();
    }
  }

  void startup()
  {
    rendering_program = shader::Shaders().create_shader_program();

    // ----------------------  Static Objects
    glGenVertexArrays( 1, &vaoStatic );
    
    GLuint vboStatic;
    glGenBuffers( 1, &vboStatic );

    glBindVertexArray( vaoStatic );

    glBindBuffer( GL_ARRAY_BUFFER, vboStatic );
    glBufferData( GL_ARRAY_BUFFER, sizeof(border), border, GL_DYNAMIC_DRAW );

    // position attribute
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );
    // color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void*)( 2 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    // --------------------------- Dynamic Objects
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &buffer );
    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );      

    // position attribute
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );
    // color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void*)( 2 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    GLuint ebo;
    glGenBuffers( 1, &ebo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW );

    isBorder = glGetUniformLocation( rendering_program, "isBorder" );

    // Draw as lines
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }

  void shutdown()
  {
    glDeleteVertexArrays( 1, &vao );
    glDeleteProgram( rendering_program );
    glDeleteBuffers( 1, &buffer );
    isStoped = true;
  }

private:
  GLuint rendering_program;
  GLuint vertex_array_object;
};

int main(int argc, const char ** argv)          
{                                  

  std::thread tetris_process ( step_in_tetris );

  my_application *app = new my_application;
  app->run(app);                                  
  delete app;                      

  tetris_process.join();

  return 0;
}
