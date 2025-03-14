#include "Grid.h"

Grid::Grid() : m_grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0)) {}

bool Grid::isCellOccupied(int x, int y) const {
  if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
    return true;  // Out-of-bounds cells are considered occupied
  }
  return m_grid[y][x] != 0;
}

void Grid::placeTetromino(const Tetromino& tetromino) {
  for (const auto& cell : tetromino.getBlocks()) {
    int x = cell[0];
    int y = cell[1];
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
      m_grid[y][x] = 1;  // Mark the grid cell as occupied
    }
  }
}

bool Grid::checkFullRows() {
  bool cleared = false;
  for (int y = 0; y < GRID_HEIGHT; ++y) {
    bool fullRow = true;
    for (int x = 0; x < GRID_WIDTH; ++x) {
      if (m_grid[y][x] == 0) {
        fullRow = false;
        break;
      }
    }
    if (fullRow) {
      clearRow(y);
      cleared = true;
    }
  }
  return cleared;
}

void Grid::clearRow(int row) {
  for (int x = 0; x < GRID_WIDTH; ++x) {
    m_grid[row][x] = 0;
  }
  shiftDown(row);
}

void Grid::shiftDown(int row) {
  for (int y = row; y > 0; --y) {
    m_grid[y] = m_grid[y - 1];
  }
  m_grid[0] = std::vector<int>(GRID_WIDTH, 0);  // Empty top row
}

void Grid::reset() {
  m_grid.assign(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));
}
