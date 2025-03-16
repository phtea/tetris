#pragma once

#include <SDL3/SDL.h>
#include "tetromino.h"
#include "renderer.h"

class Hud {
public:
    Hud();
    ~Hud();

    void update(Renderer& renderer, const Tetromino& nextTetromino);
    void render(Renderer& renderer);

private:
    int m_nextTetrominoX;
    int m_nextTetrominoY;

    // New method to render the next tetromino
    void renderNextTetromino(Renderer& renderer, const Tetromino& tetromino);
};