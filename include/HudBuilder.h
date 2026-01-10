#pragma once

#include "CustomTypes.h"
#include "Hud.h"

class HudBuilder {
  public:
    HudBuilder &setPosition(const Position &pos);
    HudBuilder &setScale(float scale);
    [[nodiscard]] Hud build() const;

  private:
    Position m_pos{0, 0};
    float m_hudScale = 0.5F;
};
