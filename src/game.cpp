#define DEBUG_NO_FALL
#define DEBUG_NO_LOCK

#include "game.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

#include "constants.h"
#include "Mino.h"
#include "HudBuilder.h"

Game::Game()
	: m_running(true),
	m_lastFallTime(SDL_GetTicks()),
	m_renderer(GAME_TITLE, 1920, 1080),
	m_Mino(MinoType::NONE),
	m_nextTetrominosSize(1),
	m_canSwap(true),
	m_bufferTetromino(MinoType::NONE),
	m_SDF(100) {
	HudBuilder hudBuilder;
	m_hud = std::make_unique<Hud>(hudBuilder.setPosition(1300, 200).build());
	createNewTetromino();
}

void Game::placeTetrominoOnGrid() {
	m_grid.placeTetromino(m_Mino);
	m_grid.checkFullRows();
	createNewTetromino();
	m_canSwap = true;  // Allow swapping again
}

void Game::createNewTetromino() {
	if (m_nextTetrominos.empty()) {
		// Pre-fill the queue with a few upcoming tetrominoes
		for (int i = 0; i < m_nextTetrominosSize; ++i) {
			m_nextTetrominos.push(pickRandomTetromino());
		}
	}

	// Get the next tetromino from the queue
	m_Mino = m_nextTetrominos.front();
	m_nextTetrominos.pop();

	// Add a new random tetromino to the queue to maintain the flow
	m_nextTetrominos.push(pickRandomTetromino());

	m_Mino.setStartPosition();
}

void Game::setNextTetrominosSize(int size) {
	m_nextTetrominosSize = size;
}

Mino Game::pickRandomTetromino() { return Mino(m_bag7.pickNext()); }

void Game::run() {
	while (m_running) {
		m_inputHandler.pollEvents();

		if (m_inputHandler.shouldQuit()) {
			stop();
		}

		handleInput();

		update();

		render();

		SDL_Delay(1000 / FPS);
	}
}

void Game::update() {

	Uint64 now = SDL_GetTicks();

	// Handle the fall behavior (move tetromino down based on m_timeToFall)
	bool canFall = now - m_lastFallTime >= m_timeToFall;
	bool toFall = true;
#ifdef DEBUG_NO_FALL
	toFall = false;
#endif // DEBUG

	if (toFall && canFall && m_Mino.canMove(Direction::DOWN, m_grid.getGrid())) {
		m_Mino.move(Direction::DOWN);
		m_lastFallTime = now;
		return;
	}

	// Independently check the lock timer
	if (m_Mino.canMove(Direction::DOWN, m_grid.getGrid())) {
		m_touchState = TouchState::NotTouching;
		return;
	}
	switch (m_touchState) {
	case TouchState::NotTouching:
		m_touchState = TouchState::JustTouched;
		m_lastLockTime = now;
		break;
	case TouchState::JustTouched:
		m_touchState = TouchState::KeepsTouching;
		break;
	case TouchState::KeepsTouching:
#ifdef DEBUG_NO_LOCK
		return;  // Prevents locking in debug mode
#endif
		if (now - m_lastLockTime >= m_lockDelayTime) {
			placeTetrominoOnGrid();

			if (isGameOver()) {
				stop();
				std::cout << "Game over!" << std::endl;
			}
		}
		break;
	default:
		std::cerr << "Didn't expect default touch state" << std::endl;
		break;
	}
}

void Game::render() {
	m_renderer.clear();
	m_grid.draw(m_renderer);
	m_Mino.draw(m_renderer);
	m_hud->update(m_renderer, m_nextTetrominos, m_nextTetrominosSize, m_bufferTetromino);

	m_renderer.present();
}

bool Game::isGameOver() {
	std::array<std::array<int, 2>, 4> blocks = m_Mino.getRelativeBlocks();
	for (const auto& block : blocks) {
		int x = block[0];
		int y = block[1];
		if (m_grid.isCellOccupied(x, y)) {
			return true;  // Collision at spawn → game over
		}
	}
	return false;
}

void Game::handleInput() {
	Uint32 now = SDL_GetTicks();

	handleMovement(Direction::LEFT, SDL_SCANCODE_LEFT);
	handleMovement(Direction::RIGHT, SDL_SCANCODE_RIGHT);

	if (m_inputHandler.isKeyPressed(SDL_SCANCODE_DOWN)) {
		bool canFall = (now - m_lastFallTime) * m_SDF >= m_timeToFall;
		if (canFall && m_Mino.canMove(Direction::DOWN, m_grid.getGrid())) {
			m_Mino.move(Direction::DOWN);
			m_lastFallTime = now;
		}
	}

	if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_SPACE)) {
		m_Mino.hardDrop(m_grid.getGrid());
		placeTetrominoOnGrid();
	}

	if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_LSHIFT) || m_inputHandler.isKeyJustPressed(SDL_SCANCODE_RSHIFT)) {
		swapTetromino();
	}

	handleRotation(-1, { SDL_SCANCODE_Z, SDL_SCANCODE_UP });
	handleRotation(1, { SDL_SCANCODE_X });
	handleRotation(2, { SDL_SCANCODE_C });

	if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_1)) {
		m_renderer.setResolution(1920, 1080);
	}
	if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_0)) {
		m_renderer.setResolution(1280, 720);
	}
}

// Implement the swapTetromino method
void Game::swapTetromino() {
	if (!m_canSwap) return;
	// TODO: bring back to original rotate state
	if (m_bufferTetromino.getType() == MinoType::NONE) {
		// Buffer is empty, add current tetromino to buffer and get next one
		m_bufferTetromino = m_Mino;
		createNewTetromino();
	}
	else {
		// Swap current tetromino with the one in the buffer
		std::swap(m_Mino, m_bufferTetromino);
		m_Mino.setStartPosition();
	}
	m_bufferTetromino.setOriginalRotationState();
	m_canSwap = false;  // Prevent swapping again until the next tetromino is placed
}

void Game::handleMovement(Direction dir, SDL_Scancode key) {
	Uint32 now = SDL_GetTicks();
	Uint32 holdTime = m_inputHandler.getKeyHoldTime(key);

	if (m_inputHandler.isKeyJustPressed(key) ||
		(m_inputHandler.isKeyPressed(key) && holdTime > m_DAS && now - m_lastMoveTime > m_ARR)) {
		if (m_Mino.canMove(dir, m_grid.getGrid())) {
			m_Mino.move(dir);
			m_lastMoveTime = now;
		}
	}
}

// Rotations is the number of rotations of piece
void Game::handleRotation(int rotations, std::initializer_list<SDL_Scancode> keys) {
	for (SDL_Scancode key : keys) {
		if (m_inputHandler.isKeyJustPressed(key)) {
			m_Mino.rotate(rotations, m_grid.getGrid());
			return;
		}
	}
}