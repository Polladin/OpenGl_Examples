#pragma once

#include <vector>
#include <memory>

class Figure
{
public:

  Figure(  std::pair<int, int> sz
         , std::vector<std::pair<int, int>> frag
         , const std::vector<std::unique_ptr<Figure>> & allFiguresIn );
  virtual ~Figure() {}

  enum StepType
  {
      STEP_LEFT
    , STEP_RIGHT
    , STEP_DOWN
  };

  enum RotateType
  {
      ROTATE_LEFT
    , ROTATE_RIGHT
  };

  bool step( StepType type);

  bool next_turn();

  bool has_conflict() const;
  bool has_conflict( std::vector<std::pair<int, int>> fragmentsToCheck ) const;

  virtual bool rotate( RotateType) { return false; }

private:

  bool step_down();
  bool step_left();
  bool step_right();

  bool is_down_available() const;
  bool is_left_available() const;
  bool is_right_available() const;

  bool is_field_used( const std::pair<int, int> & fragment ) const;

public:

  bool isActive {true};
  bool actionInTurnComplete {false};

  std::vector<std::pair<int, int>> fragments;

  std::pair<int, int> size;

private:

  const std::vector<std::unique_ptr<Figure>> & allFigures;

};


/*
*    Figure : ****
*            fragments: (0) .. (3)
*/
class Figure_1 : public Figure
{
  enum ORIENTATION
  {
      LYING
    , STANDING
  };

public:
  Figure_1( std::pair<int, int> sz, const std::vector<std::unique_ptr<Figure>> & allFiguresIn )
    : Figure( sz
             , {  { sz.first / 2 - 2, sz.second - 1 }
                , { sz.first / 2 - 1, sz.second - 1 }
                , { sz.first / 2    , sz.second - 1 }
                , { sz.first / 2 + 1, sz.second - 1 } }
             , allFiguresIn )
  {}

  /*virtual*/ 
  bool rotate( RotateType ) override;

private:

  bool rotate_left();
  bool rotate_right();

  ORIENTATION orientation{ ORIENTATION::LYING };

};

/*
*    Figure : **
*             **
*/
class Figure_2 : public Figure
{
public:
  Figure_2( std::pair<int, int> sz, const std::vector<std::unique_ptr<Figure>> & allFiguresIn )
    : Figure( sz
             , {  { sz.first / 2 - 1, sz.second - 1 }
                , { sz.first / 2    , sz.second - 1 }
                , { sz.first / 2 - 1, sz.second - 2 }
                , { sz.first / 2    , sz.second - 2 } }
             , allFiguresIn )
  {}
};

/*
*    Figure : ***
*             *
*            fragments: (0) .. (2)
*                       (3)
*/
class Figure_3 : public Figure
{

public:
  Figure_3( std::pair<int, int> sz, const std::vector<std::unique_ptr<Figure>> & allFiguresIn )
    : Figure(  sz
             , {  { sz.first / 2 - 1, sz.second - 1 }
                , { sz.first / 2    , sz.second - 1 }
                , { sz.first / 2 + 1, sz.second - 1 }
                , { sz.first / 2 - 1, sz.second - 2 } }
             , allFiguresIn )
  {}

  /*virtual*/
  bool rotate( RotateType ) override;

private:

  int rotationIdx{ 0 };

  static int rotationMatrix[ 4 ][ 4 ][ 2 ];
};


/*
*    Figure :  *
*             ***
*            fragments:    (0)
*                       (1)(2)(3)
*/
class Figure_4 : public Figure
{
  static const int rotationMatrix[ 4 ][ 2 ];

public:
  Figure_4( std::pair<int, int> sz, const std::vector<std::unique_ptr<Figure>> & allFiguresIn )
    : Figure( sz
             , {  { sz.first / 2    , sz.second - 1 }
                , { sz.first / 2 - 1, sz.second - 2 }
                , { sz.first / 2    , sz.second - 2 }
                , { sz.first / 2 + 1, sz.second - 2 } }
            , allFiguresIn )
  {}

  /*virtual*/
  bool rotate( RotateType ) override;

private:

  int rotationIdx[ 3 ]{0, 3, 1}; // for fragments: 0, 1, 3
};
