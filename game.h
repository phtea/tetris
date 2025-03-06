// game.h - Handles game logic
#ifndef GAME_H
#define GAME_H

#include "renderer.h"
#include "tetromino.h"

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
};

#endif
