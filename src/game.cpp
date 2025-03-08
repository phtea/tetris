#include "game.h"
#include "constants.h"
#include "tetromino.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL_keycode.h>
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

	tetromino.setStartPosition();

	lastFallTime = SDL_GetTicks();

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

		grid[gridY][gridX] = 1;
	}
}

void Game::createNewTetromino() {
	tetromino = Tetromino(static_cast<TetrominoType>(rand() % 7));
	tetromino.setStartPosition();
}

void Game::run() {
	while (running) {
		inputHandler.pollEvents();

		handleInput();

		updateGame();

		renderGame();

		SDL_Delay(1000/FPS);
	}
}

void Game::updateGame() {
	Uint32 currentTime = SDL_GetTicks();
	static const int timeToFall = 1000;
	if (currentTime - lastFallTime >= timeToFall) { // 1 second delay between tetromino falls
		if (!tetromino.collidesWith(Direction::DOWN, grid)) {
			tetromino.moveDown(BLOCK_SIZE);
		} else { // if collides
			placeTetrominoOnGrid();

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
}

void Game::renderGame() {
	renderer.clear();
	drawGrid();

	tetromino.draw(renderer.getRenderer());
	renderer.present();
}

void Game::drawGrid() {
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (grid[y][x] == 1) { // If the cell is occupied (1), draw it
                renderer.drawBlock(x * BLOCK_SIZE, y * BLOCK_SIZE);
            }
        }
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
	if (inputHandler.isKeyPressed(SDLK_LEFT) && !tetromino.collidesWith(Direction::LEFT, grid)) {
		tetromino.moveLeft(BLOCK_SIZE);
	}

	if (inputHandler.isKeyPressed(SDLK_RIGHT) && !tetromino.collidesWith(Direction::RIGHT, grid)) {
		tetromino.moveRight(BLOCK_SIZE);
	}

	if (inputHandler.isKeyPressed(SDLK_DOWN) && !tetromino.collidesWith(Direction::DOWN, grid)) {
		tetromino.moveDown(BLOCK_SIZE);
	}
}
