#pragma once

#include <vector>

std::vector<std::pair<int, int>> insertion_sort( std::vector<int> arr )
{
  std::vector<std::pair<int, int>> res;

  for ( int i = 1; i < arr.size(); ++i )
  {
    for ( int j = i; j > 0; --j )
    {
      if ( arr[ j ] < arr[ j - 1 ] )
      {
        std::swap( arr[ j - 1 ], arr[ j ] );
        res.emplace_back( j - 1, j );
      }
    }
  }

  return res;
}