
#include "sb7.h"

#include <vector>
#include <array>

static const GLfloat vertex_positions[] =
{
  -0.25f,  0.25f, -0.25f,
  -0.25f, -0.25f, -0.25f,
  0.25f, -0.25f, -0.25f,

  0.25f, -0.25f, -0.25f,
  0.25f,  0.25f, -0.25f,
  -0.25f,  0.25f, -0.25f,

  0.25f, -0.25f, -0.25f,
  0.25f, -0.25f,  0.25f,
  0.25f,  0.25f, -0.25f,

  0.25f, -0.25f,  0.25f,
  0.25f,  0.25f,  0.25f,
  0.25f,  0.25f, -0.25f,

  0.25f, -0.25f,  0.25f,
  -0.25f, -0.25f,  0.25f,
  0.25f,  0.25f,  0.25f,

  -0.25f, -0.25f,  0.25f,
  -0.25f,  0.25f,  0.25f,
  0.25f,  0.25f,  0.25f,

  -0.25f, -0.25f,  0.25f,
  -0.25f, -0.25f, -0.25f,
  -0.25f,  0.25f,  0.25f,

  -0.25f, -0.25f, -0.25f,
  -0.25f,  0.25f, -0.25f,
  -0.25f,  0.25f,  0.25f,

  -0.25f, -0.25f,  0.25f,
  0.25f, -0.25f,  0.25f,
  0.25f, -0.25f, -0.25f,

  0.25f, -0.25f, -0.25f,
  -0.25f, -0.25f, -0.25f,
  -0.25f, -0.25f,  0.25f,

  -0.25f,  0.25f, -0.25f,
  0.25f,  0.25f, -0.25f,
  0.25f,  0.25f,  0.25f,

  0.25f,  0.25f,  0.25f,
  -0.25f,  0.25f,  0.25f,
  -0.25f,  0.25f, -0.25f
};



std::vector<GLfloat> create_cube(GLfloat width, std::array<GLfloat, 3> leftUpCorner)
{
    std::vector<GLfloat> index1 = { 0.0f, width, width, 0.0f };
    std::vector<GLfloat> index2 = { 0.0f, 0.0f, width, width };
    std::vector<GLfloat> index3 = { 0.0f, width };
    std::vector<GLfloat> index4 = { 1.0f, 0.0f, 0.0f };

    std::vector<GLfloat> result;


    for ( int k = 0; k < 3; ++k )
    {
        for ( int j = 0; j < 2; ++j )
        {
            for ( int i = 0; i < 4; ++i )
            {
                result.push_back( leftUpCorner[ 0 ] + index1[ i ] * index4[   k                       ] + index2[ i ] * index4[ ( k + 2 ) % index4.size() ] + index3[ j ] * index4[ ( k + 1 ) % index4.size() ] ); // X
                result.push_back( leftUpCorner[ 1 ] - index1[ i ] * index4[ ( k + 1 ) % index4.size() ] - index2[ i ] * index4[ ( k     ) % index4.size() ] - index3[ j ] * index4[ ( k + 2 ) % index4.size() ] ); // Y
                result.push_back( leftUpCorner[ 2 ] + index1[ i ] * index4[ ( k + 2 ) % index4.size() ] + index2[ i ] * index4[ ( k + 1 ) % index4.size() ] + index3[ j ] * index4[ ( k     ) % index4.size() ] ); // Z
            }
        }
    }

    return result;
}