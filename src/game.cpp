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

Game::Game(int screenWidth, int screenHeight)
	: m_running(true),
	m_lastFallTime(SDL_GetTicks()),
	m_renderer(GAME_TITLE, screenWidth, screenHeight),
	m_Mino(MinoType::NONE),
	m_nextMinosSize(1),
	m_canSwap(true),
	m_bufferMino(MinoType::NONE),
	m_SDF(100) {
	HudBuilder hudBuilder;
	m_hud = std::make_unique<Hud>(hudBuilder.setPosition(1300, 200).build());
	createNewMino();
}

Game::Game(int screenWidth, int screenHeight, Uint32 timeToFall, Uint32 lockDelayTime, Uint32 das, Uint32 arr, Uint32 sdf, int nextMinosSize)
	: m_running(true),
	m_lastFallTime(SDL_GetTicks()),
	m_timeToFall(timeToFall),
	m_lockDelayTime(lockDelayTime),
	m_DAS(das),
	m_ARR(arr),
	m_SDF(sdf),
	m_renderer(GAME_TITLE, screenWidth, screenHeight),
	m_Mino(MinoType::NONE),
	m_nextMinosSize(nextMinosSize),
	m_canSwap(true),
	m_bufferMino(MinoType::NONE) {
	HudBuilder hudBuilder;
	m_hud = std::make_unique<Hud>(hudBuilder.setPosition(1300, 200).build());
	createNewMino();
}

void Game::placeMinoOnGrid() {
	m_grid.placeTetromino(m_Mino);
	m_grid.checkFullRows();
	createNewMino();
	m_canSwap = true;  // Allow swapping again
}

void Game::createNewMino() {
	if (m_nextTetrominos.empty()) {
		// Pre-fill the queue with a few upcoming tetrominoes
		for (int i = 0; i < m_nextMinosSize; ++i) {
			m_nextTetrominos.push(pickRandomMino());
		}
	}

	// Get the next tetromino from the queue
	m_Mino = m_nextTetrominos.front();
	m_nextTetrominos.pop();

	// Add a new random tetromino to the queue to maintain the flow
	m_nextTetrominos.push(pickRandomMino());

	m_Mino.setStartPosition();
}


Mino Game::pickRandomMino() { return Mino(m_bag7.pickNext()); }

void Game::run() {
	SDL_Event event;
	while (m_running) {
		m_inputHandler.pollEvents(event);

		if (m_inputHandler.shouldQuit()) {
			stop();
		}

		handleInput();

		update();

		render();

		SDL_Delay(1000 / FPS);
	}
}

void Game::setTimeToFall(Uint32 timeToFall) { m_timeToFall = timeToFall; }

void Game::setLockDelayTime(Uint32 lockDelayTime) { m_lockDelayTime = lockDelayTime; }

void Game::setDAS(Uint32 das) { m_DAS = das;  }

void Game::setARR(Uint32 arr) {	m_ARR = arr; }

void Game::setSDF(Uint32 sdf) { m_SDF = sdf; }

void Game::setNextMinosSize(int size) {	m_nextMinosSize = size; }

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
			placeMinoOnGrid();

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
	m_renderer.update();
	m_renderer.clear();
	m_grid.draw(m_renderer);
	m_Mino.draw(m_renderer);
	m_hud->update(m_renderer, m_nextTetrominos, m_nextMinosSize, m_bufferMino);

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
		placeMinoOnGrid();
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
	if (m_bufferMino.getType() == MinoType::NONE) {
		// Buffer is empty, add current tetromino to buffer and get next one
		m_bufferMino = m_Mino;
		createNewMino();
	}
	else {
		// Swap current tetromino with the one in the buffer
		std::swap(m_Mino, m_bufferMino);
		m_Mino.setStartPosition();
	}
	m_bufferMino.setOriginalRotationState();
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