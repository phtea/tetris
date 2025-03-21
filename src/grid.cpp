#include "Grid.h"

// TODO: add 2 extra rows for spawning pieces!
Grid::Grid() :
	m_grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0)),
	m_gridOfColors(GRID_HEIGHT, std::vector<SDL_Color>(GRID_WIDTH, { 0, 0, 0, 255 })),
	m_width(GRID_WIDTH),
	m_height(GRID_HEIGHT) {
}

bool Grid::isCellOccupied(int x, int y) const {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return true;  // Out-of-bounds cells are considered occupied
	}
	return m_grid[y][x] != 0;
}

void Grid::placeTetromino(const Mino& tetromino) {
	for (const auto& cell : tetromino.getRelativeBlocks()) {
		int x = cell[0];
		int y = cell[1];
		if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
			m_grid[y][x] = 1;  // Mark the grid cell as occupied
			m_gridOfColors[y][x] = tetromino.getColor();
		}
	}
}

// clears rows
// returns how many rows were cleared
int Grid::checkFullRows() {
	int cleared = 0;
	for (int y = 0; y < m_height; ++y) {
		bool fullRow = true;
		for (int x = 0; x < m_width; ++x) {
			if (m_grid[y][x] == 0) {
				fullRow = false;
				break;
			}
		}
		if (fullRow) {
			clearRow(y);
			cleared++;
		}
	}
	return cleared;
}

void Grid::clearRow(int row) {
	for (int x = 0; x < m_width; ++x) {
		m_grid[row][x] = 0;
	}
	shiftDown(row);
}

void Grid::shiftDown(int row) {
	for (int y = row; y > 0; --y) {
		m_grid[y] = m_grid[y - 1];
		m_gridOfColors[y] = m_gridOfColors[y - 1];
	}
	m_grid[0] = std::vector<int>(m_width, 0);  // Empty top row
}

void Grid::clear() { m_grid.assign(m_height, std::vector<int>(m_width, 0)); }

void Grid::draw(Renderer& renderer) const {
	// Draw occupied blocks
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			if (m_grid[y][x] == 1) {  // If the cell is occupied
				renderer.drawBlock(x, y, m_gridOfColors[y][x]);
			}
		}
	}

	// Draw grid lines
	SDL_Color gridColor = { 50, 50, 50, 255 };
	renderer.setDrawColor(gridColor);
	renderer.drawGrid(m_width, m_height);
}
