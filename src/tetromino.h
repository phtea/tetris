#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <vector>

typedef std::vector<std::vector<int>> grid_t;
typedef std::array<std::array<int, 2>, 4> blocks_t;

enum class TetrominoType : uint8_t { I, J, L, O, S, T, Z };
enum class Direction : uint8_t { LEFT, RIGHT, DOWN };

class Tetromino {
 public:
  Tetromino(TetrominoType type);

  bool canMove(Direction dir, const grid_t& grid) const;
  void move(Direction dir);
  void hardDrop(const grid_t& grid);
  bool rotate(int angle, const grid_t& grid);
  void setPosition(int x, int y);
  void setStartPosition();
  blocks_t getBlocks() const;
  std::array<int, 2> getPosition() const;
  void draw(SDL_Renderer* renderer) const;

 private:
  TetrominoType m_type;
  SDL_Color m_color;
  blocks_t m_blocks;
  int m_x, m_y;

  std::array<int, 2> Tetromino::getPivot() const;

  void setColor();
  void setShape();
  bool collidesWith(const blocks_t& testBlocks, const grid_t& grid) const;
};
