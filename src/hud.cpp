// hud.cpp
#include "hud.h"
#include <iostream>

Hud::Hud()
    : m_nextTetrominoX(15), m_nextTetrominoY(5) {
    // Position of the "Next" label and tetromino
}

Hud::~Hud() {}

void Hud::update(Renderer& renderer, const Tetromino& nextTetromino) {
    // Here we could render the score, level, and linesCleared (optional)
    // For now, we just render the next tetromino
    renderNextTetromino(renderer, nextTetromino);
}

void Hud::render(Renderer& renderer) {
    // You could render additional information like score, level, etc. here
    // For now, we just need to display the next tetromino
}

void Hud::renderNextTetromino(Renderer& renderer, const Tetromino& tetromino) {
    // Render the "Next" label not yet!
    SDL_Color labelColor = { 255, 255, 255, 255 }; // White color
    renderer.setDrawColor(labelColor); // Set color to white
    renderer.drawText("Next:", m_nextTetrominoX, m_nextTetrominoY - 20); // Assuming you have a method for text

    // Draw the blocks for the next tetromino
    auto blocks = tetromino.getBlocks();
    SDL_Color blockColor = tetromino.getColor();

    for (const auto& block : blocks) {
        int x = m_nextTetrominoX + block[0];  // Adjust this based on your block size
        int y = m_nextTetrominoY + block[1];  // Adjust this based on your block size
        renderer.drawBlock(x, y, blockColor);  // Use the Renderer to draw the block
    }
}
