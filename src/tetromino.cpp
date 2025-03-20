#include "tetromino.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <unordered_map>

#include "constants.h"

blocks_t Tetromino::applyRotation(int newRotation) {
	return shapes.at(m_type)[newRotation];
}

Tetromino::Tetromino(TetrominoType type) : m_type(type), m_rotationState(0) {
	if (m_type == TetrominoType::NONE) return;
	setColor();
	setShape(m_rotationState);
	setStartPosition();
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

void Tetromino::setShape(int newRotation) {
	m_blocks = shapes.at(m_type)[newRotation];
}

bool Tetromino::canMove(Direction dir, const grid_t& grid) const {
	std::array<std::array<int, 2>, 4> testBlocks = m_blocks;
	for (auto& block : testBlocks) {
		if (dir == Direction::LEFT) block[0]--;
		if (dir == Direction::RIGHT) block[0]++;
		if (dir == Direction::DOWN) block[1]++;
	}
	return !collidesWithGrid(testBlocks, grid);
}

void Tetromino::move(Direction dir) {
	if (dir == Direction::LEFT) m_X--;
	if (dir == Direction::RIGHT) m_X++;
	if (dir == Direction::DOWN) m_Y++;
}

void Tetromino::hardDrop(const grid_t& grid) {
	while (canMove(Direction::DOWN, grid)) {
		move(Direction::DOWN);
	}
}

// try to rotate piece N times
// returns true on success
bool Tetromino::rotate(int rotations, const grid_t& grid) {
	if (m_type == TetrominoType::O) return false;  // O piece doesn't rotate

	// Calculate new rotation state
	int newRotation = (m_rotationState + rotations + 4) % 4;

	auto wallKicks = (rotations == 2) ? get180WallKicks(newRotation, m_type) : getWallKicks(newRotation, m_type);

	// Loop through the wall kick offsets
	for (const auto& offset : wallKicks) {
		blocks_t rotatedBlocks = applyRotation(newRotation);

		for (auto& block : rotatedBlocks) {
			block[0] += offset[0];
			block[1] += offset[1];
		}

		if (!collidesWithGrid(rotatedBlocks, grid)) {
			m_blocks = rotatedBlocks;
			m_rotationState = newRotation;
			return true;
		}
	}

	return false;
}

void Tetromino::setOriginalRotationState() {
	m_rotationState = 0; // original rotation state
	setShape(m_rotationState);
}

void Tetromino::setPosition(int x, int y) {
	m_X = x;
	m_Y = y;
}

void Tetromino::setStartPosition() {
	m_X = GRID_WIDTH / 2 - 2;
	m_Y = 0;
}

std::array<std::array<int, 2>, 4> Tetromino::getRelativeBlocks() const {
	std::array<std::array<int, 2>, 4> blockPositions;
	for (int i = 0; i < 4; i++) {
		blockPositions[i] = { m_X + m_blocks[i][0],
							 m_Y + m_blocks[i][1] };
	}
	return blockPositions;
}

blocks_t Tetromino::getBlocks() const {
	return m_blocks;
}

void Tetromino::draw(Renderer& renderer) const {
	for (const auto& block : getRelativeBlocks()) {
		renderer.drawBlock(block[0], block[1], m_color);
	}
}

bool Tetromino::collidesWithGrid(const blocks_t& testBlocks, const grid_t& grid) const {
	for (const auto& block : testBlocks) {
		int x = block[0] + m_X;
		int y = block[1] + m_Y;
		if (y < 0) continue;
		if (x < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT || grid[y][x] != 0) {
			return true;
		}
	}
	return false;
}

constexpr std::array<std::array<int, 2>, 12> Tetromino::getWallKicks(int newRotation, TetrominoType type) {
	std::array<std::array<int, 2>, 12> paddedWallKicks = {}; // Initialize with zeros

	const auto& selectedKicks = (type == TetrominoType::I) ? iKicks[newRotation] : otherKicks[newRotation];

	for (size_t i = 0; i < selectedKicks.size(); i++) {
		paddedWallKicks[i] = selectedKicks[i];
	}

	return paddedWallKicks;
}

constexpr std::array<std::array<int, 2>, 12> Tetromino::get180WallKicks(int newRotation, TetrominoType type) {
	return (type == TetrominoType::I) ? iBlock180KickTable[newRotation] : otherBlock180KickTable[newRotation];
}

// MAYBE IT'S WRONG
// and it should be
// 0 - ORIGINAL, 1 - RIGHT, 2 - FLIPPED, 3 - LEFT
const std::unordered_map<TetrominoType, std::array<std::array<std::array<int, 2>, 4>, 4>> Tetromino::shapes = {
	{TetrominoType::I, {{
		{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},  // ORIGINAL
		{{{2, 0}, {2, 1}, {2, 2}, {2, 3}}},  // RIGHT (same as ORIGINAL)
		{{{0, 2}, {1, 2}, {2, 2}, {3, 2}}},  // FLIPPED (same as LEFT)
		{{{1, 0}, {1, 1}, {1, 2}, {1, 3}}},  // LEFT
	}}},
	{TetrominoType::J, {{
		{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}},  // ORIGINAL
		{{{2, 0}, {1, 0}, {1, 1}, {1, 2}}},  // RIGHT
		{{{0, 1}, {1, 1}, {2, 1}, {2, 2}}},  // FLIPPED
		{{{0, 2}, {1, 2}, {1, 1}, {1, 0}}},  // LEFT
	}}},
	{TetrominoType::L, {{
		{{{0, 1}, {1, 1}, {2, 1}, {2, 0}}},  // ORIGINAL
		{{{1, 0}, {1, 1}, {1, 2}, {2, 2}}},  // RIGHT
		{{{0, 2}, {0, 1}, {1, 1}, {2, 1}}},  // FLIPPED
		{{{0, 0}, {1, 0}, {1, 1}, {1, 2}}},  // LEFT
	}}},
	{TetrominoType::O, {{
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // ORIGINAL
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // RIGHT (same as ORIGINAL)
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // FLIPPED (same as ORIGINAL)
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // LEFT (same as ORIGINAL)
	}}},
	{TetrominoType::S, {{
		{{{0, 1}, {1, 1}, {1, 0}, {2, 0}}},  // ORIGINAL
		{{{1, 0}, {1, 1}, {2, 1}, {2, 2}}},  // RIGHT (same as ORIGINAL)
		{{{0, 2}, {1, 2}, {1, 1}, {2, 1}}},  // FLIPPED (same as LEFT)
		{{{0, 0}, {0, 1}, {1, 1}, {1, 2}}},  // LEFT
	}}},
	{TetrominoType::T, {{
		{{{0, 1}, {1, 1}, {1, 0}, {2, 1}}},  // ORIGINAL
		{{{1, 0}, {1, 1}, {2, 1}, {1, 2}}},  // RIGHT
		{{{0, 1}, {1, 1}, {1, 2}, {2, 1}}},  // FLIPPED
		{{{0, 1}, {1, 0}, {1, 1}, {1, 2}}},  // LEFT
	}}},
	{TetrominoType::Z, {{
		{{{0, 0}, {1, 0}, {1, 1}, {2, 1}}},  // ORIGINAL
		{{{1, 2}, {1, 1}, {2, 1}, {2, 0}}},  // RIGHT (same as ORIGINAL)
		{{{0, 1}, {1, 1}, {1, 2}, {2, 2}}},  // FLIPPED (same as LEFT)
		{{{0, 2}, {0, 1}, {1, 1}, {1, 0}}},  // LEFT
	}}}
};