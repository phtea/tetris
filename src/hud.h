#pragma once

#include "renderer.h"
#include "tetromino.h"
#include <queue>

class Hud {
public:
    Hud(int hudX, int hudY);
	Hud() = delete;
    ~Hud();

    void update(Renderer& renderer, const std::queue<Tetromino>& tetrominos, int count, const Tetromino& bufferTetromino);
    void render(Renderer& renderer);
    void move(int deltaX, int deltaY);
    void setShowNext(bool show);
    void setShowHold(bool show);

private:
    void renderNextTetromino(Renderer& renderer, std::queue<Tetromino> tetrominos, int count);
    void renderBufferTetromino(Renderer& renderer, const Tetromino& bufferTetromino);

    void renderTetromino(Renderer& renderer, const std::string& label, std::queue<Tetromino> tetrominos, int count);

    int m_hudX;
    int m_hudY;
    int m_elementSpacing;
    float m_hudScale;
    int m_currentElementPos;
    bool m_showNext;
    bool m_showHold;
};