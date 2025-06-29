#pragma once

#include "Renderer.h"
#include "Mino.h"
#include <queue>

class Hud {
public:
    Hud(int hudX, int hudY, float scale);
    Hud() = delete;
    ~Hud();

    void update(Renderer& renderer, int nextCount);
    void draw(Renderer& renderer, const std::queue<Mino>& tetrominos, const Mino& bufferTetromino);
    void move(int deltaX, int deltaY);
    void setShowNext(bool show);
    void setShowHold(bool show);

    // New method for drawing HUD borders
    void drawBorders(Renderer& renderer);

private:
    void renderNextTetromino(Renderer& renderer, std::queue<Mino> tetrominos, int count);
    void renderBufferTetromino(Renderer& renderer, const Mino& bufferTetromino);
    void renderTetromino(Renderer& renderer, const std::string& label, std::queue<Mino> tetrominos, int count);

    int m_hudX;
    int m_hudY;
    int m_elementSpacing;
    float m_hudScale;
    int m_currentElementPos;

    int m_NextCount = 1;

    bool m_showNext;
    bool m_showHold;

    bool m_hudBordersEnabled; // New member variable to control HUD borders
};
