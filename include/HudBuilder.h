#pragma once

#include "Hud.h"

class HudBuilder {
  public:
    HudBuilder &setPosition(int x, int y);
    HudBuilder &setScale(float scale);
    Hud build() const;

  private:
    int m_hudX = 0;
    int m_hudY = 0;
    float m_hudScale = 0.5f;
};
