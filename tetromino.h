#pragma once
#include <SDL2/SDL.h>
#include "constants.h"

enum class TetrominoType { I, J, L, O, S, T, Z };

class Tetromino {
public:
    Tetromino(TetrominoType type);

    void draw(SDL_Renderer* renderer);
    void setPosition(int startX, int startY);
    void moveDown(int step);
    
private:
    TetrominoType type;
    SDL_Color color;
    int blocks[4][2]; // Relative positions of the blocks
    int x, y; // Absolute position of Tetromino

    void setColor();
    void setShape();
};
