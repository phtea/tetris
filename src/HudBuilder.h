#pragma once

#include "hud.h"

class HudBuilder {
public:
    HudBuilder& setHudPosition(int x, int y);
    Hud build() const;

private:
    int m_hudX = 0;
    int m_hudY = 0;
};
