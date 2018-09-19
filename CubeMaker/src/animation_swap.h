#pragma once

#include <vector>
#include <array>


struct Digit
{
  enum T_STATES
  {
      STAGE_1
    , STAGE_2 
    , STAGE_3
  };

  Digit(int i_value, int i_startPos)
    : value { i_value }
    , startPos { i_startPos }
  {}

  int value;
  int startPos;
  int endPos;

  float height;
  float v;

  std::array<float, 3> currentOffset { 0.0, 0.0, 0.0 };
  T_STATES currentState { STAGE_1 };

  bool active{ false };
};


class AnimationSwap
{
public:

  AnimationSwap() {}

  AnimationSwap(std::vector<int> value, float i_degetWidth)
    : digitWidth { i_degetWidth }
  {
    for ( int i = 0; i < value.size(); ++i )
      digits.push_back( Digit( value[i], i) );
  }

  void configure( int startPos, int endPos, float height, float v )  
  {
    for ( auto & el : digits )
    {
      if ( el.startPos == startPos )
      {
        el.endPos = endPos;
        el.height = height;
        el.v = v;
        el.active = true;
      }
    }
    //digits[ startPos ].endPos = endPos;
    //digits[ startPos ].height = height;
    //digits[ startPos ].v = v;
    //digits[ startPos ].active = true;
  }

  void animate()
  {
    for ( int i = 0; i < digits.size(); ++i )
    {
      if ( !digits[ i ].active )
        continue;

      switch( digits[ i ].currentState )
      {
      case Digit::STAGE_1:

        if ( digits[ i ].height > 0 )
        {
          digits[ i ].currentOffset[ 1 ] += digits[ i ].v;

          if ( digits[ i ].currentOffset[ 1 ] >= digits[ i ].height )
            digits[ i ].currentState = Digit::STAGE_2;
        }
        else
        {
          digits[ i ].currentOffset[ 1 ] -= digits[ i ].v;

          if ( digits[ i ].currentOffset[ 1 ] <= digits[ i ].height )
            digits[ i ].currentState = Digit::STAGE_2;
        }

        break;

      case Digit::STAGE_2:

        if ( digits[ i ].startPos < digits[ i ].endPos )
        {
          digits[ i ].currentOffset[ 0 ] += digits[ i ].v;

          if ( digits[ i ].currentOffset[ 0 ] >= ( digits[ i ].endPos - digits[ i ].startPos ) * digitWidth  )
            digits[ i ].currentState = Digit::STAGE_3;
        }
        else
        {
          digits[ i ].currentOffset[ 0 ] -= digits[ i ].v;

          if ( digits[ i ].currentOffset[ 0 ] <= ( digits[ i ].endPos - digits[ i ].startPos ) * digitWidth )
            digits[ i ].currentState = Digit::STAGE_3;
        }

        break;

      case Digit::STAGE_3:

        if ( digits[ i ].height < 0 )
        {
          digits[ i ].currentOffset[ 1 ] += digits[ i ].v;

          if ( digits[ i ].currentOffset[ 1 ] >= 0.0f )
          {
            digits[ i ].startPos = digits[ i ].endPos;
            digits[ i ].currentOffset = { 0.0f, 0.0f, 0.0f };

            digits[ i ].currentState = Digit::STAGE_1;
            digits[ i ].active = false;
          }
        }
        else
        {
          digits[ i ].currentOffset[ 1 ] -= digits[ i ].v;

          if ( digits[ i ].currentOffset[ 1 ] <= 0.0f )
          {
            digits[ i ].startPos = digits[ i ].endPos;
            digits[ i ].currentOffset = { 0.0f, 0.0f, 0.0f };

            digits[ i ].currentState = Digit::STAGE_1;
            digits[ i ].active = false;
          }
        }

        break;
      }

    }
  }

public:
  std::vector<Digit> digits;

  float digitWidth;
};