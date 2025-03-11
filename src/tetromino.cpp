#include "tetromino.h"
#include "constants.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <vector>
#include <iostream>


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

void Tetromino::setStartPosition() {
	x = SCREEN_WIDTH/2 - BLOCK_SIZE;
	y = 0;
}

std::vector<int> Tetromino::getPosition() {
	return {x, y};
}

bool Tetromino::moveDown(int amount, const std::vector<std::vector<int>>& grid) {
    std::array<std::array<int, 2>, 4> testBlocks = blocks;
    for (auto& block : testBlocks) {
        block[1] += 1;
    }

    if (!collidesWith(testBlocks, grid)) {
        y += amount;
		return true;
    }

	return false;
}

bool Tetromino::moveLeft(int amount, const std::vector<std::vector<int>>& grid) {
    std::array<std::array<int, 2>, 4> testBlocks = blocks;
    for (auto& block : testBlocks) {
        block[0] -= 1;
    }
    
    if (!collidesWith(testBlocks, grid)) {
        x -= amount;
		return true;
    }

	return false;
}

bool Tetromino::moveRight(int amount, const std::vector<std::vector<int>>& grid) {
    std::array<std::array<int, 2>, 4> testBlocks = blocks;
    for (auto& block : testBlocks) {
        block[0] += 1;
    }

    if (!collidesWith(testBlocks, grid)) {
        x += amount;
		return true;
    }

	return false;
}

// Function to determine the pivot based on Tetromino type
std::pair<int, int> Tetromino::getPivot() const {
	// Default pivot
	int pivotX = blocks[0][0];
	int pivotY = blocks[0][1];


	switch (type) {
		case TetrominoType::S:
			pivotX = blocks[3][0];
			pivotY = blocks[3][1];
			break;
		case TetrominoType::I:
			pivotX = blocks[1][0]; // I piece rotates around the middle block
			pivotY = blocks[1][1];
			break;
		case TetrominoType::L:
		case TetrominoType::J:
		case TetrominoType::T:
		case TetrominoType::Z:
			pivotX = blocks[2][0];
			pivotY = blocks[2][1];
			break;
		case TetrominoType::O:
			break; // it doesn't rotate
	}

	return {pivotX, pivotY}; // Return the pivot point as a pair
}


void Tetromino::rotate(int angle, const std::vector<std::vector<int>>& grid) {
    if (type == TetrominoType::O) return; // O piece doesn't rotate

    auto [pivotX, pivotY] = getPivot();

    int newRotationState = (rotationState + (angle == 90 ? 1 : (angle == -90 ? 3 : 2))) % 4;

	std::array<std::array<int, 2>, 4> rotatedBlocks;
    for (size_t i = 0; i < 4; i++) {
        int relX = blocks[i][0] - pivotX;
        int relY = blocks[i][1] - pivotY;

        if (angle == 90) {
            rotatedBlocks[i][0] = pivotX - relY;
            rotatedBlocks[i][1] = pivotY + relX;
        } else if (angle == -90) {
            rotatedBlocks[i][0] = pivotX + relY;
            rotatedBlocks[i][1] = pivotY - relX;
        } else if (angle == 180) {
            rotatedBlocks[i][0] = pivotX - relX;
            rotatedBlocks[i][1] = pivotY - relY;
        }
    }

    // Use collidesWith to check for rotation validity
    if (!collidesWith(rotatedBlocks, grid)) {
		std::copy(rotatedBlocks.begin(), rotatedBlocks.end(), blocks.begin());
        rotationState = newRotationState;
    }
}

// Draw the Tetromino using its shape definition
void Tetromino::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	for (int i = 0; i < 4; i++) {
		SDL_FRect block = {
			x + blocks[i][0] * BLOCK_SIZE,
			y + blocks[i][1] * BLOCK_SIZE,
			BLOCK_SIZE, BLOCK_SIZE
		};
		SDL_RenderFillRect(renderer, &block);
	}
#ifdef DEBUG
	// Draw pivot block in white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	auto [pivotX, pivotY] = getPivot();
	SDL_FRect pivotBlock = {
		x + pivotX * BLOCK_SIZE,
		y + pivotY * BLOCK_SIZE,
		BLOCK_SIZE, BLOCK_SIZE
	};
	SDL_RenderFillRect(renderer, &pivotBlock);
#endif
}

// TODO: URGENT FIX THIS!!
bool Tetromino::collidesWith(const std::array<std::array<int, 2>, 4>& testBlocks,
							const std::vector<std::vector<int>>& grid) const {
    for (const auto& block : testBlocks) {
        int blockX = block[0] + x / BLOCK_SIZE;
        int blockY = block[1] + y / BLOCK_SIZE;


#ifdef DEBUG
		std::cout << "b[0]=" << block[0] << ", b[1]=" << block[1] << "\n"; 
		std::cout << "blockX=" << blockY << ", blockY=" << blockY << "\n"; 
#endif // DEBUG

        // Check out-of-bounds
        if (blockX < 0 || blockX >= BOARD_WIDTH || blockY < 0 || blockY >= BOARD_HEIGHT) {
            return true;
        }

        // Check grid collision
        if (grid[blockY][blockX] != 0) {
            return true;
        }
    }
    return false;
}

std::array<std::array<int, 2>, 4> Tetromino::getBlocks() const {
	std::array<std::array<int, 2>, 4> blockPositions;

	for (int i = 0; i < 4; i++) {
		int absX = x + blocks[i][0] * BLOCK_SIZE;
		int absY = y + blocks[i][1] * BLOCK_SIZE;
		blockPositions[i] = {absX, absY};
	}

	return blockPositions;
}
