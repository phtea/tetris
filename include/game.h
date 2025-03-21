// game.h
// Handles game logic

#pragma once

#include <vector>

#include "Bag7.h"
#include "Grid.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Mino.h"
#include "Hud.h"
#include <queue>

enum class TouchState : uint8_t { NotTouching, JustTouched, KeepsTouching };

typedef std::array<MinoType, 7> bag7_t;

class Game {
public:
	Game(int screenWidth, int screenHeight);
	Game(int screenWidth, int screenHeight, Uint32 timeToFall, Uint32 lockDelayTime, Uint32 das, Uint32 arr, Uint32 sdf, int nextMinosSize);
	~Game() = default;
	void run();

	void setTimeToFall(Uint32 timeToFall);
	void setLockDelayTime(Uint32 lockDelayTime);
	void setDAS(Uint32 das);
	void setARR(Uint32 arr);
	void setSDF(Uint32 sdf);
	void setNextMinosSize(int size);

private:
	void update();
	void handleFallDelay(Uint64 now);
	void handleLockDelay(Uint64 now);
	void createNewMino();
	void placeMinoOnGrid();
	void render();
	bool isGameOver();
	void stopGame() { m_running = false; }
	void restartGame();

	void swapTetromino();

	void handleMovement(Direction dir, SDL_Scancode key);
	void handleRotation(int angle, std::initializer_list<SDL_Scancode> keys);
	void handleInput();

	Mino pickRandomMino();

	//Hud m_Hud;
	Hud m_Hud;
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
	Mino m_Mino;
	InputHandler m_inputHandler;

	// queue of future tetriminos
	std::queue<Mino> m_nextMinos; // Queue to store upcoming pieces
	int m_nextMinosSize;

	// swap buffer
	Mino m_bufferMino;
	bool m_canSwap;
};
