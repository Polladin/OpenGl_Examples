// Include the "sb7.h" header file


#include "sb7.h"
#include "gl_common\shader_functions.h"
#include "cube_sample.h"

#include "vmath.h"
#include "sb7ktx.h"

#include <cmath>



/***********************************************************
*     Derive my_application from sb7::application
***********************************************************/
class my_application : public sb7::application
{
public:

	GLuint          render_vao;
	struct
	{
		GLint       mvp;
		GLint       offset;
	} uniforms;

	GLuint          tex_wall;
	GLuint          tex_ceiling;
	GLuint          tex_floor;

  void init()
  {
    static const char title[] = "OpenGL SuperBible - Spinny Cube";

    sb7::application::init();

    memcpy( info.title, title, sizeof( title ) );
  }

  // Our rendering function
  void render(double currentTime)
  {
	  static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	  float t = (float)currentTime;

	  glViewport( 0, 0, info.windowWidth, info.windowHeight );
	  glClearBufferfv( GL_COLOR, 0, black );

	  glUseProgram( rendering_program );

	  vmath::mat4 proj_matrix = vmath::perspective( 60.0f,
		  (float)info.windowWidth / (float)info.windowHeight,
		  0.1f, 100.0f );

	  //glUniform1f( uniforms.offset, t * 0.003f );

	  int i;
	  GLuint textures[] = { tex_wall, tex_floor, tex_wall, tex_ceiling };
	  for ( i = 0; i < 4; i++ )
	  {
		  vmath::mat4 mv_matrix = vmath::rotate( 90.0f * (float)i, vmath::vec3( 0.0f, 0.0f, 1.0f ) ) *
			  vmath::translate( -0.5f, 0.0f, -10.0f ) *
			  vmath::rotate( 90.0f, 0.0f, 1.0f, 0.0f ) *
			  vmath::scale( 30.0f, 0.9f, 1.0f );
		  vmath::mat4 mvp = proj_matrix * mv_matrix;

		  glUniformMatrix4fv( uniforms.mvp, 1, GL_FALSE, mvp );

		  glBindTexture( GL_TEXTURE_2D, textures[ i ] );
		  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	  }
  }

  void startup()
  {
    rendering_program = shader::Shaders().create_shader_program();

	uniforms.mvp = glGetUniformLocation( rendering_program, "mvp" );
	uniforms.offset = glGetUniformLocation( rendering_program, "offset" );

	glGenVertexArrays( 1, &render_vao );
	glBindVertexArray( render_vao );

	tex_wall = sb7::ktx::file::load( TEXTURE_FOLDER "/ktx/brick.ktx" );
	tex_ceiling = sb7::ktx::file::load( TEXTURE_FOLDER "/ktx/ceiling.ktx" );
	tex_floor = sb7::ktx::file::load( TEXTURE_FOLDER "/ktx/floor.ktx" );

	GLuint textures[] = { tex_floor, tex_wall, tex_ceiling };

	for ( int i = 0; i < 3; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, textures[ i ] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}

	glBindVertexArray( render_vao );


  }
  void shutdown()
  {
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
