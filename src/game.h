// game.h
// Handles game logic

#pragma once

#include "renderer.h"
#include "tetromino.h"
#include "input_handler.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
	void update();
	void createNewTetromino();
	void handleInput();
	void placeTetrominoOnGrid();
	void render();
	void drawGrid();
	bool isGameOver();
	void stop();

    bool running;
	Uint64 lastFallTime = 0;
	Uint64 lastMoveTime = 0;
	const Uint32 DAS = 167; // Delay before auto-repeat (milliseconds)
	const Uint32 ARR = 33;  // Auto-repeat rate (milliseconds)

    Renderer renderer;
    Tetromino tetromino;
	InputHandler inputHandler;
    SDL_Event event;

	// Grid of blocks [BOARD_WIDTH, BOARD_HEIGHT]
	std::vector<std::vector<int>> grid;
};