// tetromino.cpp
// Implements logic around tetromino

#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <vector>

enum class TetrominoType { I, J, L, O, S, T, Z };
enum class Direction { LEFT, RIGHT, DOWN };


class Tetromino {
public:
    Tetromino(TetrominoType type);

	bool moveLeft(int amount, const std::vector<std::vector<int>>& grid);
	bool moveRight(int amount, const std::vector<std::vector<int>>& grid);
	bool moveDown(int amount, const std::vector<std::vector<int>>& grid);
    void rotate(int angle, const std::vector<std::vector<int>>& grid);

    void setPosition(int newX, int newY);
    void setStartPosition();
	std::vector<int> getPosition();
	std::array<std::array<int, 2>, 4> getBlocks() const;

	bool collidesWith(const std::array<std::array<int, 2>, 4>& testBlocks, const std::vector<std::vector<int>>& grid) const;

    void draw(SDL_Renderer* renderer);
    
private:
    TetrominoType type;
    SDL_Color color;
	
	std::array<std::array<int, 2>, 4> blocks;
    int x, y; // Absolute position of Tetromino
	int rotationState = 0; // 0-3 (spawn, right, 180, left)

	std::pair<int, int> getPivot() const;

    void setColor();
    void setShape();
};
