#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <vector>
#include "renderer.h"
#include "custom_types.h"

typedef std::vector<std::vector<int>> grid_t;
typedef std::array<std::array<int, 2>, 4> blocks_t;

class Tetromino {
public:
	Tetromino(TetrominoType type);

	bool canMove(Direction dir, const grid_t& grid) const;
	void move(Direction dir);
	void hardDrop(const grid_t& grid);
	bool rotate(int angle, const grid_t& grid);
	void setOriginalRotationState();

	void setPosition(int x, int y);
	void setStartPosition();
	
	// Gets fixed blocks for the tetromino
	blocks_t getBlocks() const;
	// Gets the blocks relative to grid
	blocks_t getRelativeBlocks() const;

	std::array<int, 2> getPosition() const;
	SDL_Color getColor() const;
	TetrominoType getType() const;

	void draw(Renderer& renderer) const;

private:
	TetrominoType m_type;
	SDL_Color m_color;
	blocks_t m_blocks;

	int m_X, m_Y; // grid-based positions! (not pixel positions)

	std::array<int, 2> Tetromino::getPivot() const;

	void setColor();
	void setShape();
	bool collidesWith(const blocks_t& testBlocks, const grid_t& grid) const;
};
