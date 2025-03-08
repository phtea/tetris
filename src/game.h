// game.h - Handles game logic
#ifndef GAME_H
#define GAME_H

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
	void updateGame();
	void createNewTetromino();
	void handleInput();
	void placeTetrominoOnGrid();
	void renderGame();
	void drawGrid();
	bool isGameOver();

    bool running;
	Uint32 lastFallTime;
	Uint32 lastMoveTime;

    Renderer renderer;
    Tetromino tetromino;
	InputHandler inputHandler;
    SDL_Event event;

	std::vector<std::vector<int>> grid;
};

#endif
