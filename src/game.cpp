#include "game.h"
#include "constants.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

Game::Game() : running(true), tetromino(static_cast<TetrominoType>(rand()%7)) {  
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		// Initialization failed, output the error
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		running = false;
		return;
	}

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
		// handle user input
		handleInput();

		// handle tetrimino falling
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastFallTime >= 1000) { // 1 sec delay
			tetromino.moveDown(BLOCK_SIZE);
			lastFallTime = currentTime;
			auto pos = tetromino.getPosition();
			std::cout << "[" << pos[0] << " " << pos[1] << "]" << std::endl;
		}

		// render the game
        renderer.clear();
        tetromino.draw(renderer.getRenderer());
        renderer.present();
    }
}

void Game::handleInput() {
    // Handle quit event
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }

    // Update input handler (check for pressed keys)
    inputHandler.update();

    // Move the tetromino based on user input
    if (inputHandler.isKeyPressed(SDL_SCANCODE_LEFT)) {
		std::cout << "left clicked" << std::endl;
        tetromino.moveLeft(BLOCK_SIZE);  // Move left if LEFT arrow key is pressed
    }
    if (inputHandler.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        tetromino.moveRight(BLOCK_SIZE);  // Move right if RIGHT arrow key is pressed
    }
    if (inputHandler.isKeyPressed(SDL_SCANCODE_DOWN)) {
        tetromino.moveDown(BLOCK_SIZE);  // Move down if DOWN arrow key is pressed
    }
    if (inputHandler.isKeyPressed(SDL_SCANCODE_UP)) {
        tetromino.rotate();  // Rotate tetromino if UP arrow key is pressed
    }
}
