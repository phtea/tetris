#include "game.h"
#include "constants.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

Game::Game() : running(true), tetromino(static_cast<TetrominoType>(rand()%7)) {  
    SDL_Init(SDL_INIT_VIDEO);
    renderer.init("Teatris");

	// set tetromino start pos at top center
	tetromino.setPosition(SCREEN_WIDTH / 2 - BLOCK_SIZE, 0);

	lastFallTime = SDL_GetTicks();
}

Game::~Game() {
    renderer.cleanup();
    SDL_Quit();
}

void Game::run() {
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastFallTime >= 1000) { // 1 sec delay
			tetromino.moveDown(BLOCK_SIZE);
			lastFallTime = currentTime;
		}

        renderer.clear();
        tetromino.draw(renderer.getRenderer());
        renderer.present();
    }
}
