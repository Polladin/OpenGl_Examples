
#include "figure.h"

const int Figure_4::rotationMatrix[ 4 ][ 2 ]
{
  { 1, -1 },{ -1, -1 },{ -1, 1 },{ 1, 1 }
};

int Figure_3::rotationMatrix[ 4 ][ 4 ][ 2 ]
{
    { { 1, 1 }, { 0, 0 }, { -1, -1 }, { 0, 2 } }
  , { { 1, -1 }, { 0, 0 }, { -1, 1 }, { 2, 0 } }
  , { { -1, -1 },{ 0, 0 },{ 1, 1 },{ 0, -2 } }
  , { { -1, 1 },{ 0, 0 },{ 1, -1 },{ -2, 0 } }
};



Figure::Figure( std::pair<int, int> sz, std::vector<std::pair<int, int>> frag, const std::vector<std::unique_ptr<Figure>> & allFiguresIn )
  : fragments { frag }
  , size { sz }
  , allFigures { allFiguresIn }
{

}


/********************************
*     STEP_*
********************************/

bool Figure::step( StepType type )
{
  //if ( actionInTurnComplete )
  //  return false;

  bool isStepExecuted { false };

  switch ( type )
  {
  case Figure::STEP_LEFT:
    isStepExecuted = step_left();
    break;
  case Figure::STEP_RIGHT:
    isStepExecuted = step_right();
    break;
  case Figure::STEP_DOWN:
    isStepExecuted = step_down();
    break;
  }

  actionInTurnComplete = isStepExecuted;

  return isStepExecuted;
}

bool Figure::next_turn()
{
  if ( !is_down_available() )
  {
    isActive = false;
    return false;
  }

  step_down();

  actionInTurnComplete = false;

  return true;
}


bool Figure::step_down()
{

  if ( !is_down_available() )
    return false;

  for ( auto & fragment : fragments )
    fragment.second -= 1;

  return true;
}

bool Figure::step_left()
{

  if ( !is_left_available() )
    return false;

  for ( auto & fragment : fragments )
    fragment.first -= 1;

  return true;
}

bool Figure::step_right()
{

  if ( !is_right_available() )
    return false;

  for ( auto & fragment : fragments )
    fragment.first += 1;

  return true;
}


/********************************
*     IS_AVAILABLE_*
********************************/
bool Figure::is_field_used( const std::pair<int, int> & fragmentToCheck ) const
{
  for ( const auto & figure : allFigures )
  {
    // skip themself
    if ( figure.get() == this )
      continue;

    for ( const auto & fragment : figure->fragments )
    {
      if ( fragmentToCheck == fragment )
        return true;
    }
  }

  return false;
}

bool Figure::is_down_available() const
{

  for ( auto fragmentToCheck : fragments )
  {
    // if 0 (down)
    if ( fragmentToCheck.second == 0 )
      return false;

    fragmentToCheck.second -= 1;

    if ( is_field_used( fragmentToCheck ) )
      return false;
  }

  return true;
}

bool Figure::is_left_available() const
{

  for ( auto fragmentToCheck : fragments )
  {
    // if 0 (down)
    if ( fragmentToCheck.first == 0 )
      return false;

    fragmentToCheck.first -= 1;

    if ( is_field_used( fragmentToCheck ) )
      return false;
  }

  return true;
}

bool Figure::is_right_available() const
{

  for ( auto fragmentToCheck : fragments )
  {
    // if 0 (down)
    if ( fragmentToCheck.first + 1 == size.first )
      return false;

    fragmentToCheck.first += 1;

    if ( is_field_used( fragmentToCheck ) )
      return false;
  }

  return true;
}


bool Figure::has_conflict( std::vector<std::pair<int, int>> fragmentsToCheck) const
{
  for ( auto fragmentToCheck : fragmentsToCheck )
  {
    if ( fragmentToCheck.first < 0 || fragmentToCheck.first >= size.first )
      return true;
    if ( fragmentToCheck.second < 0 || fragmentToCheck.second >= size.second )
      return true;

    if ( is_field_used( fragmentToCheck ) )
      return true;
  }

  return false;
}

bool Figure::has_conflict() const
{
  return has_conflict( fragments );
}


/**************************************************
*         ROTATE
***************************************************/

bool Figure_1::rotate( RotateType )
{
  //if ( actionInTurnComplete )
  //  return false;

  actionInTurnComplete = orientation == ORIENTATION::LYING ? rotate_right()
                                                           : rotate_left();

  return actionInTurnComplete;
}


bool Figure_1::rotate_left()
{
  auto fragmentsToCheck = fragments;

  fragmentsToCheck[ 0 ].first -= 2;
  fragmentsToCheck[ 0 ].second -= 2;
  fragmentsToCheck[ 1 ].first -= 1;
  fragmentsToCheck[ 1 ].second -= 1;
  fragmentsToCheck[ 3 ].first += 1;
  fragmentsToCheck[ 3 ].second += 1;

  if ( has_conflict( fragmentsToCheck ) )
    return false;

  fragments = std::move(fragmentsToCheck);

  orientation = ORIENTATION::LYING;
  return true;
}

bool Figure_1::rotate_right()
{
  auto fragmentsToCheck = fragments;

  fragmentsToCheck[ 0 ].first += 2;
  fragmentsToCheck[ 0 ].second += 2;
  fragmentsToCheck[ 1 ].first += 1;
  fragmentsToCheck[ 1 ].second += 1;
  fragmentsToCheck[ 3 ].first -= 1;
  fragmentsToCheck[ 3 ].second -= 1;

  if ( has_conflict( fragmentsToCheck ) )
    return false;

  fragments = std::move( fragmentsToCheck );

  orientation = ORIENTATION::STANDING;

  return true;
}


bool Figure_3::rotate( RotateType )
{
  //if ( actionInTurnComplete )
  //  return false;

  auto fragmentsToCheck = fragments;

  for ( int i = 0; i < 4; ++i )
  {
    fragmentsToCheck[ i ].first += rotationMatrix[ rotationIdx ][ i ][ 0 ];
    fragmentsToCheck[ i ].second += rotationMatrix[ rotationIdx ][ i ][ 1 ];
  }

  if ( has_conflict( fragmentsToCheck ) )
    return false;

  fragments = std::move( fragmentsToCheck );

  actionInTurnComplete = true;

  rotationIdx = ( rotationIdx + 1 ) % 4;

  return true;
}


bool Figure_4::rotate( RotateType )
{

  auto fragmentsToCheck = fragments;

  fragmentsToCheck[ 0 ].first  += rotationMatrix[ rotationIdx[ 0 ] ][ 0 ];
  fragmentsToCheck[ 0 ].second += rotationMatrix[ rotationIdx[ 0 ] ][ 1 ];
  fragmentsToCheck[ 1 ].first  += rotationMatrix[ rotationIdx[ 1 ] ][ 0 ];
  fragmentsToCheck[ 1 ].second += rotationMatrix[ rotationIdx[ 1 ] ][ 1 ];
  fragmentsToCheck[ 3 ].first  += rotationMatrix[ rotationIdx[ 2 ] ][ 0 ];
  fragmentsToCheck[ 3 ].second += rotationMatrix[ rotationIdx[ 2 ] ][ 1 ];

  if ( has_conflict( fragmentsToCheck ) )
    return false;

  fragments = std::move( fragmentsToCheck );

  for ( int i = 0; i < 3; ++i )
    rotationIdx[ i ] = ( rotationIdx[ i ] + 1 ) % 4;

  actionInTurnComplete = true;

  return true;
}