#include "Mino.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <unordered_map>
#include "constants.h"
#include <cassert>

blocks_t Mino::applyRotation(int newRotation) {
	return shapes.at(m_type)[newRotation];
}

Mino::Mino(MinoType type) : m_type(type), m_rotationState(0) {
	if (m_type == MinoType::NONE) return;
	setColor();
	setShape(m_rotationState);
	setStartPosition();
}

void Mino::setColor() {
	static const std::unordered_map<MinoType, SDL_Color> colorMap = {
		{MinoType::I, {0, 255, 255, 255}},
		{MinoType::J, {0, 0, 255, 255}},
		{MinoType::L, {255, 165, 0, 255}},
		{MinoType::O, {255, 255, 0, 255}},
		{MinoType::S, {0, 255, 0, 255}},
		{MinoType::T, {128, 0, 128, 255}},
		{MinoType::Z, {255, 0, 0, 255}}
	};
	// careful! we get the color without checking if it exists
	m_color = colorMap.at(m_type);
}

void Mino::setShape(int newRotation) {
	m_blocks = shapes.at(m_type)[newRotation];
}

bool Mino::canMove(Direction dir, const grid_t& grid) const {
	std::array<std::array<int, 2>, 4> testBlocks = m_blocks;
	for (auto& block : testBlocks) {
		if (dir == Direction::LEFT) block[0]--;
		if (dir == Direction::RIGHT) block[0]++;
		if (dir == Direction::DOWN) block[1]++;
	}
	return !collidesWithGrid(testBlocks, grid);
}

void Mino::move(Direction dir) {
	if (dir == Direction::LEFT) m_X--;
	if (dir == Direction::RIGHT) m_X++;
	if (dir == Direction::DOWN) m_Y++;
}

void Mino::hardDrop(const grid_t& grid) {
	while (canMove(Direction::DOWN, grid)) {
		move(Direction::DOWN);
	}
}

// try to rotate piece N times
// returns true on success
bool Mino::rotate(int rotations, const grid_t& grid) {
	if (m_type == MinoType::O) return false;  // O piece doesn't rotate

	// Calculate new rotation state
	int newRotation = (m_rotationState + rotations + 4) % 4;

	// get180 is stable!
	// get90 is buggy
	auto wallKicks = (rotations == 2) ? get180WallKicks(m_rotationState, newRotation, m_type) : getWallKicks(m_rotationState, newRotation, m_type);

	blocks_t rotatedBlocks = applyRotation(newRotation);
	// Loop through the wall kick offsets
	for (const auto& offset : wallKicks) {
		auto testBlocks = rotatedBlocks;
		for (auto& block : testBlocks) {
			block[0] += offset[0];
			block[1] -= offset[1]; // using minus to invert y to our grid (0 is top!)
		}

		if (!collidesWithGrid(testBlocks, grid)) {
			std::cout << "used offset: " << offset[0] << " " << offset[1] << std::endl;
			m_blocks = rotatedBlocks;
			m_X += offset[0];
			m_Y -= offset[1];
			m_rotationState = newRotation;
			return true;
		}
	}

	return false;
}

void Mino::setOriginalRotationState() {
	m_rotationState = 0; // original rotation state
	setShape(m_rotationState);
}

void Mino::setPosition(int x, int y) {
	m_X = x;
	m_Y = y;
}

void Mino::setStartPosition() {
	m_X = GRID_WIDTH / 2 - 2;
	m_Y = 0;
}

blocks_t Mino::getRelativeBlocks() const {
	blocks_t relativeBlocks;
	for (size_t i = 0; i < 4; i++) {
		relativeBlocks[i] = { m_X + m_blocks[i][0], m_Y + m_blocks[i][1] };
	}
	return relativeBlocks;
}

void Mino::draw(Renderer& renderer) const {
	for (const auto& block : getRelativeBlocks()) {
		renderer.drawBlock(block[0], block[1], m_color);
	}
}

bool Mino::collidesWithGrid(const blocks_t& testBlocks, const grid_t& grid) const {
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

constexpr std::array<std::array<int, 2>, 12> Mino::getWallKicks(int from, int to, MinoType type) {
	std::array<std::array<int, 2>, 12> paddedWallKicks = {}; // Initialize with zeros

	int index = 0;
	// Compute index based on (from, to) rotation pair
	if (from == 0 && to == 1) { index = 0; }
	if (from == 1 && to == 0) { index = 1; }
	if (from == 1 && to == 2) { index = 2; }
	if (from == 2 && to == 1) { index = 3; }
	if (from == 2 && to == 3) { index = 4; }
	if (from == 3 && to == 2) { index = 5; }
	if (from == 3 && to == 0) { index = 6; }
	if (from == 0 && to == 3) { index = 7; }

	const auto& selectedKicks = (type == MinoType::I) ? iKicks[index] : otherKicks[index];

	for (size_t i = 0; i < selectedKicks.size(); i++) {
		paddedWallKicks[i] = selectedKicks[i];
	}

	return paddedWallKicks;
}

constexpr std::array<std::array<int, 2>, 12> Mino::get180WallKicks(int currentRotation, int newRotation, MinoType type) {
	return (type == MinoType::I) ? iBlock180KickTable[newRotation] : otherBlock180KickTable[newRotation];
}

// MAYBE IT'S WRONG
// and it should be
// 0 - ORIGINAL, 1 - RIGHT, 2 - FLIPPED, 3 - LEFT
const std::unordered_map<MinoType, std::array<std::array<std::array<int, 2>, 4>, 4>> Mino::shapes = {
	{MinoType::I, {{
		{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},  // ORIGINAL
		{{{2, 0}, {2, 1}, {2, 2}, {2, 3}}},  // RIGHT (same as ORIGINAL)
		{{{0, 2}, {1, 2}, {2, 2}, {3, 2}}},  // FLIPPED (same as LEFT)
		{{{1, 0}, {1, 1}, {1, 2}, {1, 3}}},  // LEFT
	}}},
	{MinoType::J, {{
		{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}},  // ORIGINAL
		{{{2, 0}, {1, 0}, {1, 1}, {1, 2}}},  // RIGHT
		{{{0, 1}, {1, 1}, {2, 1}, {2, 2}}},  // FLIPPED
		{{{0, 2}, {1, 2}, {1, 1}, {1, 0}}},  // LEFT
	}}},
	{MinoType::L, {{
		{{{0, 1}, {1, 1}, {2, 1}, {2, 0}}},  // ORIGINAL
		{{{1, 0}, {1, 1}, {1, 2}, {2, 2}}},  // RIGHT
		{{{0, 2}, {0, 1}, {1, 1}, {2, 1}}},  // FLIPPED
		{{{0, 0}, {1, 0}, {1, 1}, {1, 2}}},  // LEFT
	}}},
	{MinoType::O, {{
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // ORIGINAL
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // RIGHT (same as ORIGINAL)
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // FLIPPED (same as ORIGINAL)
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // LEFT (same as ORIGINAL)
	}}},
	{MinoType::S, {{
		{{{0, 1}, {1, 1}, {1, 0}, {2, 0}}},  // ORIGINAL
		{{{1, 0}, {1, 1}, {2, 1}, {2, 2}}},  // RIGHT (same as ORIGINAL)
		{{{0, 2}, {1, 2}, {1, 1}, {2, 1}}},  // FLIPPED (same as LEFT)
		{{{0, 0}, {0, 1}, {1, 1}, {1, 2}}},  // LEFT
	}}},
	{MinoType::T, {{
		{{{0, 1}, {1, 1}, {1, 0}, {2, 1}}},  // ORIGINAL
		{{{1, 0}, {1, 1}, {2, 1}, {1, 2}}},  // RIGHT
		{{{0, 1}, {1, 1}, {1, 2}, {2, 1}}},  // FLIPPED
		{{{0, 1}, {1, 0}, {1, 1}, {1, 2}}},  // LEFT
	}}},
	{MinoType::Z, {{
		{{{0, 0}, {1, 0}, {1, 1}, {2, 1}}},  // ORIGINAL
		{{{1, 2}, {1, 1}, {2, 1}, {2, 0}}},  // RIGHT (same as ORIGINAL)
		{{{0, 1}, {1, 1}, {1, 2}, {2, 2}}},  // FLIPPED (same as LEFT)
		{{{0, 2}, {0, 1}, {1, 1}, {1, 0}}},  // LEFT
	}}}
};