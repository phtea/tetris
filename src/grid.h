#pragma once
#include <vector>

#include "tetromino.h"

constexpr int GRID_WIDTH = 10;
constexpr int GRID_HEIGHT = 20;

class Grid {
 public:
  Grid();

  bool isCellOccupied(int x, int y) const;
  void placeTetromino(const Tetromino& tetromino);
  bool checkFullRows();
  void clearRow(int row);
  void shiftDown(int row);
  void reset();

  const std::vector<std::vector<int>>& getGrid() const { return m_grid; }

 private:
  std::vector<std::vector<int>> m_grid;
};
