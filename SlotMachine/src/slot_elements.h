#pragma once

#include <vector>


class SlotElements
{
public:

  SlotElements(float radius, float width, unsigned countSlots, unsigned slotResolution);

  std::vector<float> get_slot(unsigned slotNum);

  std::vector<float> get_text_slot( unsigned slotNum );

private:

  float m_radius;
  float m_width;
  unsigned m_countSlots;
  unsigned m_slotResolution;

};