
#include "opengl_wrapper.h"

#include <iterator>     // std::back_inserter
#include <algorithm>    // std::copy


std::vector<GLfloat> TetrisOpenGLWrapper::get_vertex_and_colors(bool useSlide) const
{
  std::vector<GLfloat> result;

  float   slideX{ 0.0f }
        , scaleX{ 1.0f }
        , slideY{ 0.0f }
        , scaleY{ 1.0f } ;


  if ( useSlide )
  {
    slideX = 0.005;
    slideY = 0.005;

    scaleX = 0.9f;
    scaleY = 0.9f;
  }


  //const auto * figures = engine.get_figures();
  //auto it = figures->begin();
  //auto ite = figures->end();

  for ( const auto & figures : *(engine.get_figures()) )
  {
    for ( const auto fragment : figures->fragments )
    {
      // p1.x
      result.push_back( OFFSET_X + slideX + fragment.first * RECT_WIDTH - 1.0f );
      // p1.y
      result.push_back( OFFSET_Y + slideY + fragment.second * RECT_WIDTH - 1.0f );
      // color
      std::copy( DEFAULT_COLOR.begin(), DEFAULT_COLOR.end(), std::back_inserter( result ) );

      // p2.x
      result.push_back( OFFSET_X + fragment.first * RECT_WIDTH + RECT_WIDTH - slideX - 1.0f );
      // p2.y
      result.push_back( OFFSET_Y + slideY + fragment.second * RECT_WIDTH - 1.0f );
      // color
      std::copy( DEFAULT_COLOR.begin(), DEFAULT_COLOR.end(), std::back_inserter( result ) );

      // p3.x
      result.push_back( OFFSET_X + fragment.first * RECT_WIDTH + RECT_WIDTH - slideX - 1.0f );
      // p3.y
      result.push_back( OFFSET_Y + fragment.second * RECT_WIDTH + RECT_WIDTH - slideY - 1.0f );
      // color
      std::copy( DEFAULT_COLOR.begin(), DEFAULT_COLOR.end(), std::back_inserter( result ) );

      // p4.x
      result.push_back( OFFSET_X + slideX + fragment.first * RECT_WIDTH - 1.0f );
      // p4.y
      result.push_back( OFFSET_Y + fragment.second * RECT_WIDTH + RECT_WIDTH - slideY - 1.0f );
      // color
      std::copy( DEFAULT_COLOR.begin(), DEFAULT_COLOR.end(), std::back_inserter( result ) );
    }
  }

  return result;

}


std::vector<GLuint> TetrisOpenGLWrapper::get_elemts_vector() const
{
  std::vector<GLuint> result;

  int countFragments = engine.get_count_fragments();

  for ( int i = 0; i < countFragments; ++i )
  {
    result.push_back( i * 4 );
    result.push_back( i * 4 + 1 );
    result.push_back( i * 4 + 2 );
    result.push_back( i * 4 + 2 );
    result.push_back( i * 4 + 3 );
    result.push_back( i * 4 );
  }

  return result;
}


