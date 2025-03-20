// game.h
// Handles game logic

#pragma once

#include <vector>

#include "bag7.h"
#include "grid.h"
#include "input_handler.h"
#include "renderer.h"
#include "tetromino.h"
#include "hud.h"
#include <queue>

enum class TouchState : uint8_t { NotTouching, JustTouched, KeepsTouching };

typedef std::array<TetrominoType, 7> bag7_t;

class Game {
public:
	Game();
	~Game() = default;
	void run();

private:
	void update();
	void createNewTetromino();
	void placeTetrominoOnGrid();
	void render();
	bool isGameOver();
	void stop() { m_running = false; }

	void swapTetromino();

	void handleMovement(Direction dir, SDL_Scancode key);
	void handleRotation(int angle, std::initializer_list<SDL_Scancode> keys);
	void handleInput();

	Tetromino pickRandomTetromino();
	void setNextTetrominosSize(int size);

	//Hud m_hud;
	std::unique_ptr<Hud> m_hud;
	bool m_running;
	TouchState m_touchState = TouchState::NotTouching;
	Bag7 m_bag7;

	Uint32 m_timeToFall = 1000;

	Uint32 m_lastFallTime = 0;
	Uint32 m_lastMoveTime = 0;
	Uint32 m_lockDelayTime = 500;  // Lock delay time (milliseconds)
	Uint32 m_lastLockTime = 0;     // Time when tetromino last touched the grid

	Uint32 m_DAS = 167;     // Delay before auto-repeat (milliseconds)
	Uint32 m_ARR = 33;      // Auto-repeat rate (milliseconds)
	Uint32 m_SDF;			// Soft-drop factor (the factor with which soft drop changes the gravity)

	Grid m_grid;
	Renderer m_renderer;
	Tetromino m_tetromino;
	InputHandler m_inputHandler;

	// queue of future tetriminos
	std::queue<Tetromino> m_nextTetrominos; // Queue to store upcoming pieces
	int m_nextTetrominosSize;

	// swap buffer
	Tetromino m_bufferTetromino;
	bool m_canSwap;
};
