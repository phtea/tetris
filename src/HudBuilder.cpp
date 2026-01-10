#include "HudBuilder.h"
#include "CustomTypes.h"

HudBuilder &HudBuilder::setPosition(const Position &pos) {
    m_pos = pos;
    return *this;
}

HudBuilder &HudBuilder::setScale(float scale) {
    m_hudScale = scale;
    return *this;
}

Hud HudBuilder::build() const { return {m_pos, m_hudScale}; }
