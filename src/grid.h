#pragma once
#include <vector>

#include "tetromino.h"
#include "constants.h"
#include "renderer.h"

class Grid {
public:
	Grid();

	bool isCellOccupied(int x, int y) const;
	void placeTetromino(const Tetromino& tetromino);
	bool checkFullRows();
	void clearRow(int row);
	void shiftDown(int row);
	void reset();
	void draw(Renderer& renderer) const;

	const std::vector<std::vector<int>>& getGrid() const { return m_grid; }

private:
	std::vector<std::vector<int>> m_grid;
	std::vector<std::vector<SDL_Color>> m_gridOfColors;
	int m_width;
	int m_height;
	int m_blockSize;
};
