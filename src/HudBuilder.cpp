#include "HudBuilder.h"

HudBuilder& HudBuilder::setHudPosition(int x, int y) {
    m_hudX = x;
    m_hudY = y;
    return *this;
}

Hud HudBuilder::build() const {
    return Hud(m_hudX, m_hudY);
}