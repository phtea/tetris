#pragma once
#include <vector>

#include "Mino.h"
#include "Renderer.h"

class Grid {
  public:
    Grid();

    [[nodiscard]] bool isCellOccupied(int x, int y) const;
    void placeTetromino(const Mino &tetromino);
    int checkFullRows();
    void clearRow(int row);
    void shiftDown(int row);
    void clear();
    void draw(Renderer &renderer) const;

    [[nodiscard]] const std::vector<std::vector<int>> &getGrid() const { return m_grid; }

  private:
    std::vector<std::vector<int>> m_grid;
    std::vector<std::vector<SDL_Color>> m_gridOfColors;
    int m_width;
    int m_height;
};
