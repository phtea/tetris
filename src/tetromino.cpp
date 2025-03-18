#include "tetromino.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <unordered_map>

#include "constants.h"

const std::unordered_map<TetrominoType, std::array<std::array<std::array<int, 2>, 4>, 4>> Tetromino::shapes = {
	{TetrominoType::I, {{
		{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},  // ORIGINAL
		{{{1, 0}, {1, 1}, {1, 2}, {1, 3}}},  // LEFT
		{{{2, 0}, {2, 1}, {2, 2}, {2, 3}}},  // RIGHT (same as ORIGINAL)
		{{{0, 2}, {1, 2}, {2, 2}, {3, 2}}}   // FLIPPED (same as LEFT)
	}}},
	{TetrominoType::J, {{
		{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}},  // ORIGINAL
		{{{0, 2}, {1, 2}, {1, 1}, {1, 0}}},  // LEFT
		{{{2, 0}, {1, 0}, {1, 1}, {1, 2}}},  // RIGHT
		{{{0, 1}, {1, 1}, {2, 1}, {2, 2}}}   // FLIPPED
	}}},
	{TetrominoType::L, {{
		{{{0, 1}, {1, 1}, {2, 1}, {2, 0}}},  // ORIGINAL
		{{{0, 0}, {1, 0}, {1, 1}, {1, 2}}},  // LEFT
		{{{1, 0}, {1, 1}, {1, 2}, {2, 2}}},  // RIGHT
		{{{0, 2}, {0, 1}, {1, 1}, {2, 1}}}   // FLIPPED
	}}},
	{TetrominoType::O, {{
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // ORIGINAL
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // LEFT (same as ORIGINAL)
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}},  // RIGHT (same as ORIGINAL)
		{{{1, 1}, {1, 0}, {2, 0}, {2, 1}}}   // FLIPPED (same as ORIGINAL)
	}}},
	{TetrominoType::S, {{
		{{{0, 1}, {1, 1}, {1, 0}, {2, 0}}},  // ORIGINAL
		{{{0, 0}, {0, 1}, {1, 1}, {1, 2}}},  // LEFT
		{{{1, 0}, {1, 1}, {2, 1}, {2, 2}}},  // RIGHT (same as ORIGINAL)
		{{{0, 2}, {1, 2}, {1, 1}, {2, 1}}}   // FLIPPED (same as LEFT)
	}}},
	{TetrominoType::T, {{
		{{{0, 1}, {1, 1}, {1, 0}, {2, 1}}},  // ORIGINAL
		{{{0, 1}, {1, 0}, {1, 1}, {1, 2}}},  // LEFT
		{{{1, 0}, {1, 1}, {2, 1}, {1, 2}}},  // RIGHT
		{{{0, 1}, {1, 1}, {1, 2}, {2, 1}}}   // FLIPPED
	}}},
	{TetrominoType::Z, {{
		{{{0, 0}, {1, 0}, {1, 1}, {2, 1}}},  // ORIGINAL
		{{{0, 2}, {0, 1}, {1, 1}, {1, 0}}},  // LEFT
		{{{1, 2}, {1, 1}, {2, 1}, {2, 0}}},  // RIGHT (same as ORIGINAL)
		{{{0, 1}, {1, 1}, {1, 2}, {2, 2}}}   // FLIPPED (same as LEFT)
	}}}
};

Tetromino::Tetromino(TetrominoType type) : m_type(type), m_rotationState(RotationState::ORIGINAL) {
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

void Tetromino::setShape(RotationState state) {
	m_blocks = shapes.at(m_type)[static_cast<int>(state)];
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

bool Tetromino::rotate(int angle, const grid_t& grid) {
	if (m_type == TetrominoType::O) return false;  // O piece doesn't rotate

	RotationState newState;
	switch (angle) {
	case 90:
		newState = (m_rotationState == RotationState::ORIGINAL) ? RotationState::RIGHT :
			(m_rotationState == RotationState::RIGHT) ? RotationState::FLIPPED :
			(m_rotationState == RotationState::FLIPPED) ? RotationState::LEFT :
			RotationState::ORIGINAL;
		break;
	case -90:
		newState = (m_rotationState == RotationState::ORIGINAL) ? RotationState::LEFT :
			(m_rotationState == RotationState::LEFT) ? RotationState::FLIPPED :
			(m_rotationState == RotationState::FLIPPED) ? RotationState::RIGHT :
			RotationState::ORIGINAL;
		break;
	case 180:
		newState = (m_rotationState == RotationState::ORIGINAL) ? RotationState::FLIPPED :
			(m_rotationState == RotationState::FLIPPED) ? RotationState::ORIGINAL :
			(m_rotationState == RotationState::LEFT) ? RotationState::RIGHT :
			RotationState::LEFT;
		break;
	default:
		return false;
	}

	std::array<std::array<int, 2>, 4> rotatedBlocks = shapes.at(m_type)[static_cast<int>(newState)];

	std::array<std::array<int, 2>, 5> kickOffsets{ {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}} };

	for (const auto& offset : kickOffsets) {
		std::array<std::array<int, 2>, 4> kickedBlocks = rotatedBlocks;
		for (auto& block : kickedBlocks) {
			block[0] += offset[0];
			block[1] += offset[1];
		}
		if (!collidesWithGrid(kickedBlocks, grid)) {
			// No collisions with the kick offset, update the blocks and position
			m_rotationState = newState;
			m_blocks = kickedBlocks;
			return true;
		}
	}

	// If all kicks fail, the rotation is not possible
	return false;
}

void Tetromino::setOriginalRotationState() {
	m_rotationState = RotationState::ORIGINAL;
	setShape(m_rotationState);
}

std::array<int, 2> Tetromino::getPivot() const {
	return m_blocks[0];
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

std::array<int, 2> Tetromino::getPosition() const { return { m_X, m_Y }; }

void Tetromino::draw(Renderer& renderer) const {
	for (const auto& block : getRelativeBlocks()) {
		renderer.drawBlock(block[0], block[1], m_color);
	}
}

SDL_Color Tetromino::getColor() const { return m_color; }

TetrominoType Tetromino::getType() const {
	return m_type;
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
