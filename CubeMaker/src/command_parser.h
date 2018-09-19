#pragma once

#include "vmath.h"

#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>


class CommandParser
{
public:

  CommandParser() {}
  
  virtual vmath::mat4 apply( const vmath::mat4 & mat ) const = 0;

  virtual void name() const = 0;
};


class TranslateCommand : public CommandParser
{
public:

  TranslateCommand() {}

  TranslateCommand( float i_x, float i_y, float i_z )
    : x { i_x }
    , y { i_y }
    , z { i_z }
  { }

  virtual vmath::mat4 apply( const vmath::mat4 & mat ) const override
  {
    return mat * vmath::translate( x, y, z );
  }

  virtual void name() const override { std::cout << "Translate : " << x << " " << y << " " << z << std::endl; }

private:

  float x, y, z;

};

class RotateCommand : public CommandParser
{
public:

  RotateCommand() {}

  RotateCommand( float i_x, float i_y, float i_z )
    : x{ i_x }
    , y{ i_y }
    , z{ i_z }
  { }

  virtual vmath::mat4 apply( const vmath::mat4 & mat ) const override
  {
    return mat *  vmath::rotate( x, vmath::vec3( 1.0f, 0.0f, 0.0f ) )
               *  vmath::rotate( y, vmath::vec3( 0.0f, 1.0f, 0.0f ) )
               *  vmath::rotate( z, vmath::vec3( 0.0f, 0.0f, 1.0f ) );
  }

  virtual void name() const override { std::cout << "Rotate : " << x << " " << y << " " << z << std::endl; }

private:

  float x, y, z;
};

class ScaleCommand : public CommandParser
{
public:

  ScaleCommand() {}

  ScaleCommand( float i_x, float i_y, float i_z )
    : x{ i_x }
    , y{ i_y }
    , z{ i_z }
  { }

  virtual vmath::mat4 apply( const vmath::mat4 & mat ) const override
  {
    return mat *  vmath::scale( x, y, z );
  }

  virtual void name() const override { std::cout << "Scale : " << x << " " << y << " " << z << std::endl; }

private:

  float x, y, z;
};


static std::shared_ptr< CommandParser > command_factory( std::string cmd, float x, float y, float z )
{
  if ( cmd == "Tr" )
  {
    return std::shared_ptr< CommandParser >( new TranslateCommand( x, y, z ) );
  }
  else if ( cmd == "R" )
  {
    return std::shared_ptr< CommandParser >( new RotateCommand( x, y, z ) );
  }
  else if ( cmd == "S" )
  {
    return std::shared_ptr< CommandParser >( new ScaleCommand( x, y, z ) );
  }

  throw std::string( "Wrong Command" );
}


//---------------------------------------------------------------------------------------
//                                    PARSER
//---------------------------------------------------------------------------------------



class Parser
{
public:

  using t_parser_commands = std::vector< std::vector< std::shared_ptr< CommandParser > > >;

  static t_parser_commands parse( const std::vector<std::string> strCommands )
  {
    Parser::t_parser_commands cmds;

    for ( const auto & command : strCommands )
    {
      std::istringstream istr( command );
      std::string nameCmd;
      std::vector < std::shared_ptr< CommandParser > > lineCmd;

      while ( istr >> nameCmd )
      {
        // get 3 float values
        float x, y, z;
        istr >> x >> y >> z;

        lineCmd.push_back( command_factory( nameCmd, x, y, z ) );
      }

      cmds.push_back( lineCmd );
    }

    return cmds;
  }

};