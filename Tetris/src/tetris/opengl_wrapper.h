
#include "tetris.h"

#include "sb7.h"



class TetrisOpenGLWrapper
{
  const int FILED_SIZE_X { 24 };
  const int FILED_SIZE_Y { 32 };
  const float RECT_WIDTH{ 0.05f };

  const float OFFSET_X{ 0.2f };
  const float OFFSET_Y{ 0.2f };

  const std::vector<GLfloat> DEFAULT_COLOR{ 0.1f, 0.9f, 0.4f };

public:

  TetrisOpenGLWrapper() 
    : engine( {FILED_SIZE_X, FILED_SIZE_Y} )
  {
  
  }

  std::vector<GLfloat> get_vertex_and_colors( bool useSlide = false ) const;

  std::vector<GLuint> get_elemts_vector() const;

  void step() { engine.time_step(); }
  void step_left() { engine.action( Figure::StepType::STEP_LEFT ); }
  void step_right() { engine.action( Figure::StepType::STEP_RIGHT ); }

  void rotate_right() { engine.rotate( Figure::ROTATE_RIGHT ); }
  void rotate_left()  { engine.rotate( Figure::ROTATE_LEFT ); }

private:

  Tetris engine;

};