#include "tetromino.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>

#include "constants.h"

Tetromino::Tetromino(TetrominoType type) : m_type(type) {
  setColor();
  setShape();
}

void Tetromino::setColor() {
  switch (m_type) {
    case TetrominoType::I:
      m_color = {0, 255, 255, 255};
      break;  // Cyan
    case TetrominoType::J:
      m_color = {0, 0, 255, 255};
      break;  // Blue
    case TetrominoType::L:
      m_color = {255, 165, 0, 255};
      break;  // Orange
    case TetrominoType::O:
      m_color = {255, 255, 0, 255};
      break;  // Yellow
    case TetrominoType::S:
      m_color = {0, 255, 0, 255};
      break;  // Green
    case TetrominoType::T:
      m_color = {128, 0, 128, 255};
      break;  // Purple
    case TetrominoType::Z:
      m_color = {255, 0, 0, 255};
      break;  // Red
  }
}

// Define block positions for each Tetromino shape
void Tetromino::setShape() {
  switch (m_type) {
    case TetrominoType::I:
      m_blocks[0][0] = 0;
      m_blocks[0][1] = 1;
      m_blocks[1][0] = 1;
      m_blocks[1][1] = 1;
      m_blocks[2][0] = 2;
      m_blocks[2][1] = 1;
      m_blocks[3][0] = 3;
      m_blocks[3][1] = 1;
      break;
    case TetrominoType::J:
      m_blocks[0][0] = 0;
      m_blocks[0][1] = 0;
      m_blocks[1][0] = 0;
      m_blocks[1][1] = 1;
      m_blocks[2][0] = 1;
      m_blocks[2][1] = 1;
      m_blocks[3][0] = 2;
      m_blocks[3][1] = 1;
      break;
    case TetrominoType::L:
      m_blocks[0][0] = 2;
      m_blocks[0][1] = 0;
      m_blocks[1][0] = 0;
      m_blocks[1][1] = 1;
      m_blocks[2][0] = 1;
      m_blocks[2][1] = 1;
      m_blocks[3][0] = 2;
      m_blocks[3][1] = 1;
      break;
    case TetrominoType::O:
      m_blocks[0][0] = 1;
      m_blocks[0][1] = 0;
      m_blocks[1][0] = 2;
      m_blocks[1][1] = 0;
      m_blocks[2][0] = 1;
      m_blocks[2][1] = 1;
      m_blocks[3][0] = 2;
      m_blocks[3][1] = 1;
      break;
    case TetrominoType::S:
      m_blocks[0][0] = 1;
      m_blocks[0][1] = 0;
      m_blocks[1][0] = 2;
      m_blocks[1][1] = 0;
      m_blocks[2][0] = 0;
      m_blocks[2][1] = 1;
      m_blocks[3][0] = 1;
      m_blocks[3][1] = 1;
      break;
    case TetrominoType::T:
      m_blocks[0][0] = 1;
      m_blocks[0][1] = 0;
      m_blocks[1][0] = 0;
      m_blocks[1][1] = 1;
      m_blocks[2][0] = 1;
      m_blocks[2][1] = 1;
      m_blocks[3][0] = 2;
      m_blocks[3][1] = 1;
      break;
    case TetrominoType::Z:
      m_blocks[0][0] = 0;
      m_blocks[0][1] = 0;
      m_blocks[1][0] = 1;
      m_blocks[1][1] = 0;
      m_blocks[2][0] = 1;
      m_blocks[2][1] = 1;
      m_blocks[3][0] = 2;
      m_blocks[3][1] = 1;
      break;
  }
}

void Tetromino::setPosition(int x, int y) {
  m_x = x;
  m_y = y;
}

void Tetromino::setStartPosition() {
  m_x = SCREEN_WIDTH / 2 - BLOCK_SIZE;
  m_y = 0;
}

std::vector<int> Tetromino::getPosition() { return {m_x, m_y}; }

bool Tetromino::canMoveDown(const grid_t& grid) const {
  std::array<std::array<int, 2>, 4> testBlocks = m_blocks;
  for (auto& block : testBlocks) {
    block[1] += 1;
  }

  return !collidesWith(testBlocks, grid);
}

void Tetromino::moveDown(int amount) { m_y += amount; }

bool Tetromino::canMoveLeft(const grid_t& grid) const {
  std::array<std::array<int, 2>, 4> testBlocks = m_blocks;
  for (auto& block : testBlocks) {
    block[0] -= 1;
  }

  return !collidesWith(testBlocks, grid);
}

void Tetromino::moveLeft(int amount) { m_x -= amount; }

bool Tetromino::canMoveRight(const grid_t& grid) const {
  std::array<std::array<int, 2>, 4> testBlocks = m_blocks;
  for (auto& block : testBlocks) {
    block[0] += 1;
  }

  return !collidesWith(testBlocks, grid);
}

void Tetromino::moveRight(int amount) { m_x += amount; }

void Tetromino::hardDrop(int amount, const grid_t& grid) {
  while (canMoveDown(grid)) {
    moveDown(amount);
  }
}

bool Tetromino::moveIfCan(Direction dir, int amount, const grid_t& grid) {
  switch (dir) {
    case Direction::DOWN:
      if (canMoveDown(grid)) {
        moveDown(amount);
        return true;
      }
      break;
    case Direction::LEFT:
      if (canMoveLeft(grid)) {
        moveLeft(amount);
        return true;
      }
      break;
    case Direction::RIGHT:
      if (canMoveRight(grid)) {
        moveRight(amount);
        return true;
      }
      break;
  }
  return false;
}

// Function to determine the pivot based on Tetromino m_type
std::pair<int, int> Tetromino::getPivot() const {
  // Default pivot
  int pivotX = m_blocks[0][0];
  int pivotY = m_blocks[0][1];

  switch (m_type) {
    case TetrominoType::S:
      pivotX = m_blocks[3][0];
      pivotY = m_blocks[3][1];
      break;
    case TetrominoType::I:
      pivotX = m_blocks[1][0];  // I piece rotates around the middle block
      pivotY = m_blocks[1][1];
      break;
    case TetrominoType::L:
    case TetrominoType::J:
    case TetrominoType::T:
    case TetrominoType::Z:
      pivotX = m_blocks[2][0];
      pivotY = m_blocks[2][1];
      break;
    case TetrominoType::O:
      break;  // it doesn't rotate
  }

  return {pivotX, pivotY};  // Return the pivot point as a pair
}

void Tetromino::rotate(int angle, const grid_t& grid) {
  if (m_type == TetrominoType::O) return;  // O piece doesn't rotate

  auto [pivotX, pivotY] = getPivot();

  int newRotationState =
      (m_rotationState + (angle == 90 ? 1 : (angle == -90 ? 3 : 2))) % 4;

  std::array<std::array<int, 2>, 4> rotatedBlocks;
  for (size_t i = 0; i < 4; i++) {
    int relX = m_blocks[i][0] - pivotX;
    int relY = m_blocks[i][1] - pivotY;

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
    std::copy(rotatedBlocks.begin(), rotatedBlocks.end(), m_blocks.begin());
    m_rotationState = newRotationState;
  }
}

// Draw the Tetromino using its shape definition
void Tetromino::draw(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);

  for (int i = 0; i < 4; i++) {
    SDL_FRect block = {m_x + m_blocks[i][0] * BLOCK_SIZE,
                       m_y + m_blocks[i][1] * BLOCK_SIZE, BLOCK_SIZE,
                       BLOCK_SIZE};
    SDL_RenderFillRect(renderer, &block);
  }
#ifdef DEBUG
  // Draw pivot block in white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  auto [pivotX, pivotY] = getPivot();
  SDL_FRect pivotBlock = {m_x + pivotX * BLOCK_SIZE, m_y + pivotY * BLOCK_SIZE,
                          BLOCK_SIZE, BLOCK_SIZE};
  SDL_RenderFillRect(renderer, &pivotBlock);
#endif
}

bool Tetromino::collidesWith(
    const std::array<std::array<int, 2>, 4>& testBlocks,
    const grid_t& grid) const {
  for (const auto& block : testBlocks) {
    int blockX = block[0] + m_x / BLOCK_SIZE;
    int blockY = block[1] + m_y / BLOCK_SIZE;

    // Check out-of-bounds
    if (blockX < 0 || blockX >= BOARD_WIDTH || blockY < 0 ||
        blockY >= BOARD_HEIGHT) {
      return true;
    }

    // Check m_grid collision
    if (grid[blockY][blockX] != 0) {
      return true;
    }
  }
  return false;
}

std::array<std::array<int, 2>, 4> Tetromino::getBlocks() const {
  std::array<std::array<int, 2>, 4> blockPositions;

  for (int i = 0; i < 4; i++) {
    int absX = m_x + m_blocks[i][0] * BLOCK_SIZE;
    int absY = m_y + m_blocks[i][1] * BLOCK_SIZE;
    blockPositions[i] = {absX, absY};
  }

  return blockPositions;
}
