#include "Grid.h"

Grid::Grid()
	: m_grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0)),
	m_gridOfColors(GRID_HEIGHT, std::vector<SDL_Color>(GRID_WIDTH, {0, 0, 0, 255})),
	m_width(GRID_WIDTH),
	m_height(GRID_HEIGHT),
	m_blockSize(BLOCK_SIZE) {
}

bool Grid::isCellOccupied(int x, int y) const {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return true;  // Out-of-bounds cells are considered occupied
	}
	return m_grid[y][x] != 0;
}

void Grid::placeTetromino(const Tetromino& tetromino) {
	for (const auto& cell : tetromino.getBlocks()) {
		int x = cell[0];
		int y = cell[1];
		if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
			m_grid[y][x] = 1;  // Mark the grid cell as occupied
			m_gridOfColors[y][x] = tetromino.getColor();
		}
	}
}

bool Grid::checkFullRows() {
	bool cleared = false;
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
			cleared = true;
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

void Grid::reset() { m_grid.assign(m_height, std::vector<int>(m_width, 0)); }

void Grid::draw(Renderer& renderer) const {
	// Draw occupied blocks
	for (int y = 0; y < GRID_HEIGHT; ++y) {
		for (int x = 0; x < m_width; ++x) {
			if (m_grid[y][x] == 1) {  // If the cell is occupied
				renderer.drawBlock(x * m_blockSize, y * m_blockSize, m_gridOfColors[y][x]);
			}
		}
	}

	// Set grid line color
	SDL_Color gridColor = { 50, 50, 50, 255 };  // Light gray for grid lines
	renderer.setDrawColor(gridColor);

	// Draw vertical grid lines
	for (int x = 0; x <= m_width; ++x) {
		int screenX = x * m_blockSize;
		renderer.drawLine(screenX, 0, screenX, GRID_HEIGHT * m_blockSize);
	}

	// Draw horizontal grid lines
	for (int y = 0; y <= GRID_HEIGHT; ++y) {
		int screenY = y * m_blockSize;
		renderer.drawLine(0, screenY, m_width * m_blockSize, screenY);
	}

}
