#include "tetromino.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

Tetromino::Tetromino(TetrominoType type) : type(type) {
	setColor();
	setShape();
}

void Tetromino::setColor() {
	switch (type) {
		case TetrominoType::I: color = {0, 255, 255, 255}; break; // Cyan
		case TetrominoType::J: color = {0, 0, 255, 255}; break;   // Blue
		case TetrominoType::L: color = {255, 165, 0, 255}; break; // Orange
		case TetrominoType::O: color = {255, 255, 0, 255}; break; // Yellow
		case TetrominoType::S: color = {0, 255, 0, 255}; break;   // Green
		case TetrominoType::T: color = {128, 0, 128, 255}; break; // Purple
		case TetrominoType::Z: color = {255, 0, 0, 255}; break;   // Red
	}
}

// Define block positions for each Tetromino shape
void Tetromino::setShape() {
    switch (type) {
        case TetrominoType::I:
            blocks[0][0] = 0; blocks[0][1] = 1;
            blocks[1][0] = 1; blocks[1][1] = 1;
            blocks[2][0] = 2; blocks[2][1] = 1;
            blocks[3][0] = 3; blocks[3][1] = 1;
            break;
        case TetrominoType::J:
            blocks[0][0] = 0; blocks[0][1] = 0;
            blocks[1][0] = 0; blocks[1][1] = 1;
            blocks[2][0] = 1; blocks[2][1] = 1;
            blocks[3][0] = 2; blocks[3][1] = 1;
            break;
        case TetrominoType::L:
            blocks[0][0] = 2; blocks[0][1] = 0;
            blocks[1][0] = 0; blocks[1][1] = 1;
            blocks[2][0] = 1; blocks[2][1] = 1;
            blocks[3][0] = 2; blocks[3][1] = 1;
            break;
        case TetrominoType::O:
            blocks[0][0] = 1; blocks[0][1] = 0;
            blocks[1][0] = 2; blocks[1][1] = 0;
            blocks[2][0] = 1; blocks[2][1] = 1;
            blocks[3][0] = 2; blocks[3][1] = 1;
            break;
        case TetrominoType::S:
            blocks[0][0] = 1; blocks[0][1] = 0;
            blocks[1][0] = 2; blocks[1][1] = 0;
            blocks[2][0] = 0; blocks[2][1] = 1;
            blocks[3][0] = 1; blocks[3][1] = 1;
            break;
        case TetrominoType::T:
            blocks[0][0] = 1; blocks[0][1] = 0;
            blocks[1][0] = 0; blocks[1][1] = 1;
            blocks[2][0] = 1; blocks[2][1] = 1;
            blocks[3][0] = 2; blocks[3][1] = 1;
            break;
        case TetrominoType::Z:
            blocks[0][0] = 0; blocks[0][1] = 0;
            blocks[1][0] = 1; blocks[1][1] = 0;
            blocks[2][0] = 1; blocks[2][1] = 1;
            blocks[3][0] = 2; blocks[3][1] = 1;
            break;
    }
}

void Tetromino::setPosition(int startX, int startY) {
	x = startX;
	y = startY;
}

void Tetromino::moveDown(int step) {
	y += step;
}

// Draw the Tetromino using its shape definition
void Tetromino::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int i = 0; i < 4; i++) {
        SDL_Rect block = {
            x + blocks[i][0] * BLOCK_SIZE,
            y + blocks[i][1] * BLOCK_SIZE,
            BLOCK_SIZE, BLOCK_SIZE
        };
        SDL_RenderFillRect(renderer, &block);
    }
}
