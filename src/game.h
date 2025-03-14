// game.h
// Handles game logic

#pragma once

#include <vector>

#include "bag7.h"
#include "grid.h"
#include "input_handler.h"
#include "renderer.h"
#include "tetromino.h"

enum class TouchState : uint8_t { NotTouching, JustTouched, KeepsTouching };

typedef std::array<TetrominoType, 7> bag7_t;

class Game {
 public:
  Game();
  ~Game();
  void run();

 private:
  void update();
  void createNewTetromino();
  void handleInput();
  void placeTetrominoOnGrid();
  void render();
  void drawGrid();
  bool isGameOver();
  void stop();
  Tetromino pickRandomTetromino();

  bool m_running;
  TouchState m_touchState = TouchState::NotTouching;
  Bag7 m_bag7;
  Uint32 m_timeToFall = 1000;
  Uint32 m_lastFallTime = 0;
  Uint32 m_lastMoveTime = 0;
  Uint32 m_lockDelayTime = 500;  // Lock delay time (milliseconds)
  Uint32 m_lastLockTime = 0;     // Time when tetromino last touched the grid
  const Uint32 m_DAS = 167;      // Delay before auto-repeat (milliseconds)
  const Uint32 m_ARR = 33;       // Auto-repeat rate (milliseconds)

  Grid m_grid;  // Grid of m_blocks [BOARD_WIDTH, BOARD_HEIGHT]
  Renderer m_renderer;
  Tetromino m_tetromino;
  InputHandler m_inputHandler;
  SDL_Event m_event;
};
