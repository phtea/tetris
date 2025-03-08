#include "tetromino.h"
#include "constants.h"
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

void Tetromino::setPosition(int newX, int newY) {
	x = newX;
	y = newY;
}

std::vector<int> Tetromino::getPosition() {
	return {x, y};
}

void Tetromino::moveDown(int blockSize) {
	y += blockSize;
}

void Tetromino::moveLeft(int blockSize) {
	x -= blockSize;
}

void Tetromino::moveRight(int blockSize) {
	x += blockSize;
}

void Tetromino::rotate() {
	// rotate around the first block (this will work for most shapes)
	// we will first find the relative position of each block, and rotate them around a pivot
	int pivotX = blocks[1][0];
	int pivotY = blocks[1][1];

	for (int i = 0; i<4; i++) {
		// get relative position of each block from the pivot
		int relativeX = blocks[i][0] - pivotX;
		int relativeY = blocks[i][1] - pivotY;

		// rotate the block 90 degrees counterclockwise
		blocks[i][0] = pivotX - relativeY;
		blocks[i][1] = pivotY - relativeX;
	}
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

bool Tetromino::collidesWith(Direction direction) {
    for (int i = 0; i < 4; i++) {
        int blockX = x + blocks[i][0] * BLOCK_SIZE;
        int blockY = y + blocks[i][1] * BLOCK_SIZE;

        // Left wall collision
        if (direction == Direction::LEFT && blockX <= 0) {
            return true;
        }
        
        // Right wall collision
        if (direction == Direction::RIGHT && blockX + BLOCK_SIZE >= SCREEN_WIDTH) {
            return true;
        }

        // Bottom wall collision
        if (direction == Direction::DOWN && blockY + BLOCK_SIZE >= SCREEN_HEIGHT) {
            return true;
        }
    }
    return false;
}

std::vector<std::array<int, 2>> Tetromino::getBlocks() const {
    std::vector<std::array<int, 2>> blockPositions;
    
    for (int i = 0; i < 4; i++) {
        int absX = x + blocks[i][0] * BLOCK_SIZE;
        int absY = y + blocks[i][1] * BLOCK_SIZE;
        blockPositions.push_back({absX, absY});
    }

    return blockPositions;
}
