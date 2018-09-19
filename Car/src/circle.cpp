
#include <cmath>

#include "circle.h"

const float PI = 3.14159265359;
const float PI_2 = 6.28318530718;


std::vector<float> CircleCoords::get_circle_vertex( unsigned resolution, float depth )
{
  std::vector<float> res;

  res.push_back( 0.0f ); // X0
  res.push_back( 0.0f ); // Y0
  res.push_back( 0.0f ); // Z0

  for ( int i = 0; i < resolution + 1; ++i )
  {
    float angle = PI_2 * i / ( resolution );

    res.push_back( std::sin( angle ) ); // X
    res.push_back( std::cos( angle ) ); // Y
    res.push_back( depth ); // Z
  }

  return res;
}


std::vector<float> CircleCoords::get_circle_texture_coords( unsigned resolution )
{
  std::vector<float> res;

  res.push_back( 0.5f ); // X0
  res.push_back( 0.5f ); // Y0

  for ( int i = 0; i < resolution + 1; ++i )
  {
    float angle = PI_2 * i / ( resolution );

    res.push_back( std::sin( angle ) * 0.5 + 0.5 ); // X
    res.push_back( std::cos( angle ) * 0.5 + 0.5 ); // Y
  }

  return res;
}


std::vector<unsigned> CircleCoords::get_circle_index( unsigned resolution )
{
  std::vector<unsigned> res;

  for ( int i = 0; i < resolution; ++i )
  {
    res.push_back( 0 ); // Center
    res.push_back( i + 1 );
    res.push_back( i + 2 );
  }

  return res;
}


std::vector<float> CircleCoords::get_protector_vertex( unsigned resolution, float width )
{
  std::vector<float> res;

  for ( int i = 0; i < resolution + 1; ++i )
  {
    float angle = PI_2 * i / ( resolution );
    float xCoord = std::sin( angle );
    float yCoord = std::cos( angle );

    res.push_back( xCoord ); // X
    res.push_back( yCoord ); // Y
    res.push_back( -width ); // Z

    res.push_back( xCoord ); // X
    res.push_back( yCoord ); // Y
    res.push_back( width ); // Z
  }

  return res;
}


std::vector<float> CircleCoords::get_protector_texture_coords( unsigned resolution, float texStep )
{
  std::vector<float> res;

  for ( int i = 0; i < resolution + 1; ++i )
  {
    res.push_back( 0.0f ); // X
    res.push_back( 0.0f + i * texStep ); // Y

    res.push_back( 1.0f ); // X
    res.push_back( 0.0f + i * texStep ); // Y
  }

  return res;
}


