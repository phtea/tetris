#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <vector>
#include "renderer.h"
#include "custom_types.h"
#include <unordered_map>

typedef std::vector<std::vector<int>> grid_t;
typedef std::array<std::array<int, 2>, 4> blocks_t;

class Mino {
public:
	Mino(MinoType type);

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

	std::array<int, 2> getPosition() const { return { m_X, m_Y }; }
	SDL_Color getColor() const { return m_color; }
	MinoType getType() const { return m_type; }

	void draw(Renderer& renderer) const;

private:
	void setColor();
	bool collidesWithGrid(const blocks_t& testBlocks, const grid_t& grid) const;

	// rotation logic
	void setShape(int newRotation);
	constexpr static std::array<std::array<int, 2>, 12> getWallKicks(int newRotation, MinoType type);
	constexpr static std::array<std::array<int, 2>, 12> get180WallKicks(int newRotation, MinoType type);
	blocks_t applyRotation(int newRotation);

private:
	MinoType m_type;
	SDL_Color m_color;
	blocks_t m_blocks;
	int m_rotationState = 0;
	static const std::unordered_map<MinoType, std::array<std::array<std::array<int, 2>, 4>, 4>> shapes;

	int m_X, m_Y; // grid-based positions! (not pixel positions)

};


constexpr std::array<std::array<std::array<int, 2>, 5>, 4> iKicks = { {
	{{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}},
	{{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}},
	{{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}},
	{{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}},
} };

constexpr std::array<std::array<std::array<int, 2>, 5>, 4> otherKicks = { {
	{{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},		// 1>>0─┐
	{{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},	// 0>>1─┘
	{{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},		// 1>>2─┐
	{{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},		// 2>>1─┘
} };

constexpr std::array<std::array<std::array<int, 2>, 12>, 4> otherBlock180KickTable = { {
	{{ {{0, 0}}, {{-1, 0}}, {{-2, 0}}, {{-1, -1}}, {{-2, -1}}, {{1, 0}}, {{2, 0}}, {{1, -1}}, {{2, -1}}, {{0, 1}}, {{-3, 0}}, {{3, 0}}}},	// 2>>0─┐
	{{ {{0, 0}}, {{0, 1}}, {{0, 2}}, {{1, 1}}, {{1, 2}}, {{0, -1}}, {{0, -2}}, {{1, -1}}, {{1, -2}}, {{-1, 0}}, {{0, 3}}, {{0, -3}}}},		// 3>>1─┼┐
	{{ {{0, 0}}, {{1, 0}}, {{2, 0}}, {{1, 1}}, {{2, 1}}, {{-1, 0}}, {{-2, 0}}, {{-1, 1}}, {{-2, 1}}, {{0, -1}}, {{3, 0}}, {{-3, 0}}}},		// 0>>2─┘│
	{{ {{0, 0}}, {{0, 1}}, {{0, 2}}, {{-1, 1}}, {{-1, 2}}, {{0, -1}}, {{0, -2}}, {{-1, -1}}, {{-1, -2}}, {{1, 0}}, {{0, 3}}, {{0, -3}}}},	// 1>>3──┘
} };

constexpr std::array<std::array<std::array<int, 2>, 12>, 4> iBlock180KickTable = { {
	{{ {{0, 0}}, {{1, 0}}, {{2, 0}}, {{-1, 0}}, {{-2, 0}}, {{0, -1}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}} }},		//2>>0─┐
	{{ {{0, 0}}, {{0, 1}}, {{0, 2}}, {{0, -1}}, {{0, -2}}, {{1, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}} }},			//3>>1─┼┐
	{{ {{0, 0}}, {{-1, 0}}, {{-2, 0}}, {{1, 0}}, {{2, 0}}, {{0, 1}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}} }},			//0>>2─┘│
	{{ {{0, 0}}, {{0, 1}}, {{0, 2}}, {{0, -1}}, {{0, -2}}, {{-1, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}}, {{0, 0}} }},		//1>>3──┘
} };