#pragma once

#include <vector>


class CircleCoords
{
public:

  // Circle
  static std::vector<float> get_circle_vertex( unsigned resolution, float depth );

  static std::vector<float> get_circle_texture_coords( unsigned resolution );

  static std::vector<unsigned> get_circle_index( unsigned resolution );


  // Protector
  static std::vector<float> get_protector_vertex( unsigned resolution, float width );

  static std::vector<float> get_protector_texture_coords( unsigned resolution, float texStep );


};