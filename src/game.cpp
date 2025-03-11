// game.cpp
// Implements the main game loop, state transitions, and game logic.

#include "game.h"
#include "constants.h"
#include "tetromino.h"
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

Game::Game()
	:	running(true),
		tetromino(static_cast<TetrominoType>(rand() % 7)),
		event(),
		lastFallTime(SDL_GetTicks()),
		lastMoveTime(0),
		renderer() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		// Initialization failed, output the error
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		stop();
		return;
	}

	renderer.init(GAME_TITLE);

	tetromino.setStartPosition();

	// TODO: make array of BOARD_HEIGHT and BOARD_WIDTH
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
		if (gridX >= 0 && gridX < BOARD_WIDTH && gridY >= 0 && gridY < BOARD_HEIGHT) { // TODO: make sure this bounds check is needed
			grid[gridY][gridX] = 1;
		}
	}
}

void Game::createNewTetromino() {
	tetromino = Tetromino(static_cast<TetrominoType>(rand() % 7));
	tetromino.setStartPosition();
}

void Game::run() {
	while (running) {
		inputHandler.pollEvents();

		if (inputHandler.shouldQuit()) {
			stop();
		}

		handleInput();

		update();

		render();

		SDL_Delay(1000/FPS);
	}
}

void Game::update() {
	Uint64 currentTime = SDL_GetTicks();
	static const int timeToFall = 1000;
	if (currentTime - lastFallTime >= timeToFall) {
		if (!tetromino.moveDown(BLOCK_SIZE, grid)) { // if collides
			placeTetrominoOnGrid();

			createNewTetromino();

			if (isGameOver()) {
				stop();
				std::cout << "Game over!" << std::endl;
			}
		}

		lastFallTime = currentTime;
#ifdef DEBUG
		auto pos = tetromino.getPosition();
		std::cout << "[" << pos[0] << " " << pos[1] << "]" << std::endl;
#endif // DEBUG
	}
}

void Game::render() {
	renderer.clear();
	drawGrid();

	tetromino.draw(renderer.getRenderer());
	renderer.present();
}

void Game::drawGrid() {
    SDL_SetRenderDrawColor(renderer.getRenderer(), 50, 50, 50, 255);  // Light gray color for grid lines

    // Draw vertical grid lines
    for (int x = 0; x <= BOARD_WIDTH; ++x) {
        int screenX = x * BLOCK_SIZE;
        SDL_RenderLine(renderer.getRenderer(), screenX, 0, screenX, BOARD_HEIGHT * BLOCK_SIZE);
    }

    // Draw horizontal grid lines
    for (int y = 0; y <= BOARD_HEIGHT; ++y) {
        int screenY = y * BLOCK_SIZE;
        SDL_RenderLine(renderer.getRenderer(), 0, screenY, BOARD_WIDTH * BLOCK_SIZE, screenY);
    }

    // Then, draw the blocks (the colored cells that are part of the tetromino or fixed)
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (grid[y][x] == 1) { // If the cell is occupied (1), draw it
                renderer.drawBlock(x * BLOCK_SIZE, y * BLOCK_SIZE);
            }
        }
    }
}


bool Game::isGameOver() {
	std::array<std::array<int, 2>, 4> blocks = tetromino.getBlocks();
	for (const auto& block : blocks) {
		int x = block[0] / BLOCK_SIZE;
		int y = block[1] / BLOCK_SIZE;
		if (grid[y][x] != 0) {
			return true; // Collision at spawn → game over
		}
	}
	return false;
}

void Game::stop() {
	running = false;
}

void Game::handleInput() {
    if (inputHandler.isKeyPressed(SDLK_LEFT)) {
        tetromino.moveLeft(BLOCK_SIZE, grid);
    }

    if (inputHandler.isKeyPressed(SDLK_RIGHT)) {
        tetromino.moveRight(BLOCK_SIZE, grid);
    }

    if (inputHandler.isKeyPressed(SDLK_DOWN)) {
        tetromino.moveDown(BLOCK_SIZE, grid);
    }

    if (inputHandler.isKeyJustPressed(SDLK_Z) || inputHandler.isKeyJustPressed(SDLK_UP)) {
        tetromino.rotate(-90, grid);
    }

    if (inputHandler.isKeyJustPressed(SDLK_X)) {
        tetromino.rotate(90, grid);
    }

    if (inputHandler.isKeyJustPressed(SDLK_C)) {
        tetromino.rotate(180, grid);
    }
}

