// hud.cpp
#include "hud.h"
#include <iostream>

// TODO: change this awful hardcode
Hud::Hud()
    : m_nextTetrominoX(12), m_nextTetrominoY(3) {
    // Position of the "Next" label and tetromino
}

Hud::~Hud() {}

void Hud::update(Renderer& renderer, const std::queue<Tetromino>& tetrominos, int count) {
    // Here we could render the score, level, and linesCleared (optional)
    // For now, we just render the next tetromino
    renderNextTetromino(renderer, tetrominos, count);
}

void Hud::render(Renderer& renderer) {
    // You could render additional information like score, level, etc. here
    // For now, we just need to display the next tetromino
}

void Hud::renderNextTetromino(Renderer& renderer, std::queue<Tetromino> tetrominos, int count) {
    // Draw "Next:" label at a grid-based position
    renderer.drawText("Next:", m_nextTetrominoX, m_nextTetrominoY);

    int yOffset = 2; // Offset to space out the tetrominos visually

    for (size_t i = 0; i < count && !tetrominos.empty(); ++i) {
        Tetromino t = tetrominos.front();
        tetrominos.pop();

        SDL_Color blockColor = t.getColor();
        auto blocks = t.getBlocks();

        for (const auto& block : blocks) {
            int x = m_nextTetrominoX + block[0];
            int y = m_nextTetrominoY + block[1] + yOffset;
            renderer.drawBlock(x, y, blockColor);
        }

        yOffset += 3; // Move each next tetromino down visually
    }
}
