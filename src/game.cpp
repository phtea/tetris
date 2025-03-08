#include "game.h"
#include "constants.h"
#include "tetromino.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL_scancode.h>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

Game::Game() : running(true), tetromino(static_cast<TetrominoType>(rand()%7)) {  
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		// Initialization failed, output the error
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		running = false;
		return;
	}

	renderer.init(GAME_TITLE);

	// set tetromino start pos at top center
	tetromino.setPosition(SCREEN_WIDTH / 2 - BLOCK_SIZE, 0); // TODO: create setStartPosition function

	lastFallTime = SDL_GetTicks();

	// initialize the grid (10*20)
	grid = std::vector<std::vector<int>>(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
}

Game::~Game() {
	renderer.cleanup();
	SDL_Quit();
}

void Game::placeTetrominoOnGrid() {
	for (const auto& block : tetromino.getBlocks()) {
		int gridX = block[0] / BLOCK_SIZE;
		int gridY = block[1] / BLOCK_SIZE;

		grid[gridY][gridX] = 1; // Mark grid cell as occupied
	}
}

void Game::createNewTetromino() {
	tetromino = Tetromino(static_cast<TetrominoType>(rand() % 7));
	tetromino.setPosition(SCREEN_WIDTH/2 - BLOCK_SIZE, 0);
}

void Game::run() {
	while (running) {
		handleInput();

		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastFallTime >= 1000) { // 1 sec delay
			if (!tetromino.collidesWith(Direction::DOWN, grid)) {
				tetromino.moveDown(BLOCK_SIZE);
			} else {
				// store landed tetrimino
				placeTetrominoOnGrid();

				// spawn new one
				createNewTetromino();

				if (isGameOver()) {
					running = false;
					std::cout << "Game over!" << std::endl;
				}
			}
			lastFallTime = currentTime;

			auto pos = tetromino.getPosition();
			std::cout << "[" << pos[0] << " " << pos[1] << "]" << std::endl;
		}

		// render the game
		renderer.clear();

		// Draw existing blocks on the grid
		for (int y = 0; y < BOARD_HEIGHT; ++y) {
			for (int x = 0; x < BOARD_WIDTH; ++x) {
				if (grid[y][x] == 1) {
					renderer.drawBlock(x * BLOCK_SIZE, y * BLOCK_SIZE);
				}
			}
		}

		tetromino.draw(renderer.getRenderer());
		renderer.present();
	}
}

bool Game::isGameOver() {
	auto blocks = tetromino.getBlocks();
	for (const auto& block : blocks) {
		int x = block[0] / BLOCK_SIZE;
		int y = block[1] / BLOCK_SIZE;
		if (grid[y][x] != 0) {
			return true; // Collision at spawn → game over
		}
	}
	return false;
}

void Game::handleInput() {
	// Handle quit event
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
	}

	inputHandler.update();

	Uint32 currentTime = SDL_GetTicks();

	// If 500ms has passed since the last move
	if (currentTime - lastMoveTime >= 500) {
		if (inputHandler.isKeyPressed(SDL_SCANCODE_LEFT) && !tetromino.collidesWith(Direction::LEFT, grid)) {
			tetromino.moveLeft(BLOCK_SIZE);
		}

		if (inputHandler.isKeyPressed(SDL_SCANCODE_RIGHT) && !tetromino.collidesWith(Direction::RIGHT, grid)) {
			tetromino.moveRight(BLOCK_SIZE);
		}

		if (inputHandler.isKeyPressed(SDL_SCANCODE_DOWN) && !tetromino.collidesWith(Direction::DOWN, grid)) {
			tetromino.moveDown(BLOCK_SIZE);
		}

		// Update the last move time
		lastMoveTime = currentTime;
	}
}
