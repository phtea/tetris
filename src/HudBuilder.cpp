#include "HudBuilder.h"

HudBuilder& HudBuilder::setPosition(int x, int y) {
    m_hudX = x;
    m_hudY = y;
    return *this;
}

HudBuilder& HudBuilder::setScale(float scale) {
    m_hudScale = scale;
    return *this;
}

Hud HudBuilder::build() const {
    return Hud(m_hudX, m_hudY, m_hudScale);
}