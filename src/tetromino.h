// m_tetromino.cpp
// Implements logic around m_tetromino

#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <vector>

typedef std::vector<std::vector<int>> grid_t;

enum class TetrominoType : uint8_t { I, J, L, O, S, T, Z };
enum class Direction : uint8_t { LEFT, RIGHT, DOWN };

class Tetromino {
 public:
  Tetromino(TetrominoType type);

  // Check if m_tetromino can move
  bool canMoveDown(const grid_t& grid) const;
  bool canMoveRight(const grid_t& grid) const;
  bool canMoveLeft(const grid_t& grid) const;

  // Collisions have to be checked before moving
  void moveDown();
  void moveLeft();
  void moveRight();
  void hardDrop(const grid_t& grid);

  // Abstraction for moving if can (returns true if moved)
  bool moveIfCan(Direction dir, const grid_t& grid);

  void rotate(int angle, const grid_t& grid);

  void setPosition(int x, int y);
  void setStartPosition();
  std::vector<int> getPosition();
  std::array<std::array<int, 2>, 4> getBlocks() const;

  bool collidesWith(const std::array<std::array<int, 2>, 4>& testBlocks,
                    const grid_t& grid) const;

  void draw(SDL_Renderer* renderer);

 private:
  TetrominoType m_type;
  SDL_Color m_color;

  std::array<std::array<int, 2>, 4> m_blocks;
  int m_x, m_y;             // Absolute position of Tetromino

  std::pair<int, int> getPivot() const;

  void setColor();
  void setShape();
};