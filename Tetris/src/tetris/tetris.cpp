
#include "tetris.h"

#include <iostream>
#include <algorithm>


void Tetris::reset(std::pair<int,int> newSize)
{
  size = newSize;
  figures.clear();
  theGameWasEnded = false;

  figures.push_back( std::move(next_figure()) );
  activeFigure = figures.back().get();
}


void Tetris::action( Figure::StepType direction )
{
  activeFigure->step( direction );
}

void Tetris::rotate( Figure::RotateType rotateType )
{
  activeFigure->rotate( rotateType );
}


bool Tetris::time_step()
{
  if ( theGameWasEnded )
    return false;

  if ( !activeFigure->next_turn() )
  {
    // check for out lines
    remove_filled_lines();

    figures.push_back( next_figure() );
    activeFigure = figures.back().get();

    theGameWasEnded = activeFigure->has_conflict();

    if ( theGameWasEnded )
      std::cout << "----------------- Game The End ---------------------" << std::endl;
  }

  return !theGameWasEnded;
}


std::unique_ptr<Figure> Tetris::next_figure()
{
  std::mt19937 gen( rd() );

  //std::vector<std::pair<int, int>> lowLineFilled;
  //lowLineFilled.resize(size.first + 2);
  //for ( int i = 0; i < lowLineFilled.size(); ++i )
  //  lowLineFilled[i] = std::make_pair(i, 5);
  //lowLineFilled[ size.first ] = std::make_pair( dis( gen ), 6 );
  //lowLineFilled[ size.first + 1 ] = std::make_pair( dis( gen ), 6 );

  //return std::unique_ptr<Figure>( new Figure{ size, lowLineFilled, figures } );

  switch ( dis(gen) )
  {
  case 0:
    return std::unique_ptr<Figure>( new Figure_1 { size, figures } );
  case 1:
    return std::unique_ptr<Figure>( new Figure_2 { size, figures } );
  case 2:
    return std::unique_ptr<Figure>( new Figure_3 { size, figures } );
  case 3:                                        
    return std::unique_ptr<Figure>( new Figure_4 { size, figures } );
  }

  throw std::string( "Random generator Error" );
}


int Tetris::get_count_fragments() const
{
  int res = 0;

  for ( auto & figure : figures )
    res += static_cast<int>(figure->fragments.size());

  return res;
}


std::vector<std::vector<char>> Tetris::get_all_fields() const
{
  std::vector<std::vector<char>> result;

  result.resize( size.first );
  for ( int i = 0; i < result.size(); ++i )
    result[ i ].resize(size.second, FieldState::UNFILLED);

  for ( const auto & figure : figures )
  {
    for ( const auto & fragment : figure->fragments )
    {
      result[ fragment.first ][ fragment.second ] = FieldState::FRAGMENT;
    }
  }

  return result;
}


std::vector<int> Tetris::get_lines_to_remove() const
{
  auto theField = get_all_fields();
  std::vector<int> result;

  for ( int yLine = 0; yLine < size.second; ++yLine )
  {
    int countFilled = 0;

    for ( int xLine = 0; xLine < size.first; ++xLine )
    {
      if ( theField[ xLine ][ yLine ] == FieldState::FRAGMENT )
        ++countFilled;
    }

    if ( countFilled == size.first )
      result.push_back(yLine);
  }

  return result;
}


void Tetris::remove_filled_lines()
{
  std::vector<int> linesToRemove = get_lines_to_remove();

  for ( auto & figure : figures )
  {
    auto itStartToRemove = std::remove_if(  figure->fragments.begin()
                                          , figure->fragments.end()
                                          , [ & ]( const auto & fragment ){ return std::find(  linesToRemove.begin()
                                                                                             , linesToRemove.end()
                                                                                             , fragment.second ) != linesToRemove.end();
                                                                  } );

    figure->fragments.erase( itStartToRemove, figure->fragments.end() );
  }

  // Push down remain lines
  push_remain_lines( linesToRemove );
}


void Tetris::push_remain_lines( std::vector<int> removedLines )
{
  // push lines
  for ( int yLine : removedLines )
  {
    for ( auto & figure : figures )
    {
      for ( auto & fragment : figure->fragments )
      {
        if ( fragment.second > yLine )
          --fragment.second;
      }
    }
  }

  // Purge figures
  auto itFiguresToDelete = std::remove_if(  figures.begin()
                                          , figures.end()
                                          , [ & ]( const auto &figure ) { return figure->fragments.empty(); } );
  
  figures.erase( itFiguresToDelete, figures.end() );
}