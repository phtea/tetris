#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <vector>

enum class TetrominoType { I, J, L, O, S, T, Z };
enum class Direction { LEFT, RIGHT, DOWN };


class Tetromino {
public:
    Tetromino(TetrominoType type);

    void moveLeft(int blockSize);
    void moveRight(int blockSize);
    void moveDown(int blockSize);

    void rotate();

    void setPosition(int newX, int newY);
	std::vector<int> getPosition();
	std::vector<std::array<int, 2>> getBlocks() const;

	/*bool collidesWith(Direction direction);*/
	bool collidesWith(Direction direction, const std::vector<std::vector<int>>& grid);

    void draw(SDL_Renderer* renderer);
    
private:
    TetrominoType type;
    SDL_Color color;
	
    int blocks[4][2]; // Relative positions of the blocks
    int x, y; // Absolute position of Tetromino

    void setColor();
    void setShape();
};
