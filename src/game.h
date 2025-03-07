// game.h - Handles game logic
#ifndef GAME_H
#define GAME_H

#include "renderer.h"
#include "tetromino.h"
#include "input_handler.h"

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    bool running;
    SDL_Event event;
    Renderer renderer;
    Tetromino tetromino;
	Uint32 lastFallTime;

	InputHandler inputHandler; // Instance of the InputHandler
	
	void handleInput();
};

#endif
