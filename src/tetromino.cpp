#include "tetromino.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <unordered_map>

#include <algorithm>

#include "constants.h"

Tetromino::Tetromino(TetrominoType type) : m_type(type), m_x(0), m_y(0) {
	setColor();
	setShape();
}

void Tetromino::setColor() {
	static const std::unordered_map<TetrominoType, SDL_Color> colorMap = {
		{TetrominoType::I, {0, 255, 255, 255}},
		{TetrominoType::J, {0, 0, 255, 255}},
		{TetrominoType::L, {255, 165, 0, 255}},
		{TetrominoType::O, {255, 255, 0, 255}},
		{TetrominoType::S, {0, 255, 0, 255}},
		{TetrominoType::T, {128, 0, 128, 255}},
		{TetrominoType::Z, {255, 0, 0, 255}}
	};
	// careful! we get the color without checking if it exists
	m_color = colorMap.at(m_type);
}

void Tetromino::setShape() {
	static const std::array<std::array<int, 2>, 4> shapes[] = {
		{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},  // I
		{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}},  // J
		{{{2, 0}, {0, 1}, {1, 1}, {2, 1}}},  // L
		{{{1, 0}, {2, 0}, {1, 1}, {2, 1}}},  // O
		{{{1, 0}, {2, 0}, {0, 1}, {1, 1}}},  // S
		{{{1, 0}, {0, 1}, {1, 1}, {2, 1}}},  // T
		{{{0, 0}, {1, 0}, {1, 1}, {2, 1}}}   // Z
	};
	m_blocks = shapes[static_cast<int>(m_type)];
}

bool Tetromino::canMove(Direction dir, const grid_t& grid) const {
	std::array<std::array<int, 2>, 4> testBlocks = m_blocks;
	for (auto& block : testBlocks) {
		if (dir == Direction::LEFT) block[0]--;
		if (dir == Direction::RIGHT) block[0]++;
		if (dir == Direction::DOWN) block[1]++;
	}
	return !collidesWith(testBlocks, grid);
}

void Tetromino::move(Direction dir) {
	if (dir == Direction::LEFT) m_x -= BLOCK_SIZE;
	if (dir == Direction::RIGHT) m_x += BLOCK_SIZE;
	if (dir == Direction::DOWN) m_y += BLOCK_SIZE;
}

void Tetromino::hardDrop(const grid_t& grid) {
	while (canMove(Direction::DOWN, grid)) {
		move(Direction::DOWN);
	}
}

bool Tetromino::rotate(int angle, const grid_t& grid) {
	if (m_type == TetrominoType::O) return false;  // O piece doesn't rotate

	auto [pivotX, pivotY] = getPivot();
	std::array<std::array<int, 2>, 4> rotatedBlocks;

	// Normalize angle to -90, 90, or 180
	int cosTheta = (angle == 180) ? -1 : 0;
	int sinTheta = (angle == 90) ? 1 : (angle == -90 ? -1 : 0);

	for (size_t i = 0; i < 4; i++) {
		int relX = m_blocks[i][0] - pivotX;
		int relY = m_blocks[i][1] - pivotY;

		// Apply rotation matrix:
		// x' = x * cos(θ) - y * sin(θ)
		// y' = x * sin(θ) + y * cos(θ)
		rotatedBlocks[i][0] = pivotX + (relX * cosTheta - relY * sinTheta);
		rotatedBlocks[i][1] = pivotY + (relX * sinTheta + relY * cosTheta);
	}

	if (!collidesWith(rotatedBlocks, grid)) {
		m_blocks = rotatedBlocks;
		return true;
	}
	return false;
}

std::array<int, 2> Tetromino::getPivot() const {
	return (m_type == TetrominoType::I) ? m_blocks[1] : m_blocks[2];
}

void Tetromino::setPosition(int x, int y) {
	m_x = x;
	m_y = y;
}

void Tetromino::setStartPosition() {
	m_x = (GRID_WIDTH / 2 - 2) * BLOCK_SIZE;
	m_y = 0;
}

std::array<std::array<int, 2>, 4> Tetromino::getBlocks() const {
	std::array<std::array<int, 2>, 4> blockPositions;
	for (int i = 0; i < 4; i++) {
		blockPositions[i] = { m_x / BLOCK_SIZE + m_blocks[i][0],
							 m_y / BLOCK_SIZE + m_blocks[i][1] };
	}
	return blockPositions;
}

std::array<int, 2> Tetromino::getPosition() const { return { m_x, m_y }; }

void Tetromino::draw(Renderer& renderer) const {
	for (const auto& block : getBlocks()) {
		renderer.drawBlock(block[0] * BLOCK_SIZE, block[1] * BLOCK_SIZE, m_color);
	}
}

SDL_Color Tetromino::getColor() const { return m_color; }

bool Tetromino::collidesWith(
	const blocks_t& testBlocks,
	const grid_t& grid) const {
	for (const auto& block : testBlocks) {
		int x = block[0] + m_x / BLOCK_SIZE;
		int y = block[1] + m_y / BLOCK_SIZE;
		if (y < 0) continue;
		if (x < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT || grid[y][x] != 0) {
			return true;
		}
	}
	return false;
}
