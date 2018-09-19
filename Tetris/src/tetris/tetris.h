#pragma once

#include "figure.h"

#include <vector>
#include <memory>
#include <random>


class Tetris
{
  enum FieldState
  {
      UNFILLED = 0
    , FRAGMENT
  };

public:

  Tetris( std::pair<int, int> newSize) { reset( newSize ); }

  void reset( std::pair<int, int> newSize );

  void action( Figure::StepType direction );
  void rotate( Figure::RotateType rotateType );
  
  bool time_step();

  const std::vector<std::unique_ptr<Figure>> * get_figures() const { return &figures; }

  int get_count_fragments() const;

private:

  std::unique_ptr<Figure> next_figure();

  std::vector<std::vector<char>> get_all_fields() const;

  std::vector<int> get_lines_to_remove() const;

  void remove_filled_lines();

  void push_remain_lines(std::vector<int> removedLines);

private:

  std::vector<std::unique_ptr<Figure>> figures;

  Figure * activeFigure { nullptr };
  
  std::pair<int, int> size;

  bool theGameWasEnded{ false };

  std::random_device rd;
  std::uniform_int_distribution<> dis{ 0, 3 };

};