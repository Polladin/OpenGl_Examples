
#include <cmath>

#include "slot_elements.h"

const float PI = 3.14159265359;
const float PI_2 = 6.28318530718;


float radToDeg(float rad)
{
  return rad * 360 / PI_2;
}


SlotElements::SlotElements( float radius, float width, unsigned countSlots, unsigned slotResolution )
  : m_radius { radius }
  , m_width { width }
  , m_countSlots { countSlots }
  , m_slotResolution { slotResolution }
{

}


std::vector<float> SlotElements::get_slot( unsigned slotNum )
{
  std::vector<float> res;

  for ( unsigned i = 0; i < m_slotResolution + 1; ++i )
  {
    float stepRad = ( PI_2 * i ) / ( m_slotResolution * m_countSlots ) + PI_2 * slotNum / m_countSlots;

    res.push_back( -m_width ); // X
    res.push_back( std::sin( stepRad ) * 0.5 );// Y
    res.push_back( std::cos( stepRad ) * 0.5 ); // Z

    auto lastPos = res.size();
    res.push_back( m_width ); // X
    res.push_back( res[ lastPos - 2 ] ); // Y
    res.push_back( res[ lastPos - 1 ] ); // Z
  }

  return res;
}


std::vector<float> SlotElements::get_text_slot( unsigned slotNum )
{
  std::vector<float> res;

  for ( unsigned i = 0; i < m_slotResolution + 1; ++i )
  {
    res.push_back( 0 ); // X
    res.push_back( (float)i / m_slotResolution ); // Y

    res.push_back( 1 ); // X
    res.push_back( (float)i / m_slotResolution ); // Y
  }

  return res;
}