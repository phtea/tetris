#pragma once

#include "Mino.h"
#include "Renderer.h"
#include <queue>
#include "CustomTypes.h"

class Hud {
  public:
    Hud(const Position& pos, float scale);
    Hud() = delete;
    ~Hud() = default;

    void update(Renderer &renderer, int nextCount);
    void draw(Renderer &renderer, const std::queue<Mino> &minos, const Mino &bufferMino, int level, int score);
    void move(int deltaX, int deltaY);
    void setShowNext(bool show);
    void setShowHold(bool show);
		void showStat(Renderer &renderer, const std::string& statLabel, int stat);

    // New method for drawing HUD borders
    void drawBorders(Renderer &renderer) const;

  private:
    void renderNextTetromino(Renderer &renderer, std::queue<Mino> minos, int count);
    void renderBufferTetromino(Renderer &renderer, const Mino &bufferMino);
    void renderTetromino(Renderer &renderer, const std::string &label, std::queue<Mino> minos, int count);

    int m_hudX;
    int m_hudY;
    float m_elementSpacing;
    float m_hudScale;
    int m_currentElementPos;

    int m_NextCount = 1;

    bool m_showNext;
    bool m_showHold;

    bool m_hudBordersEnabled; // New member variable to control HUD borders
};
