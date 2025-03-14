// game.cpp
// Implements the main game loop, state transitions, and game logic.

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
#include "tetromino.h"

Game::Game()
    : m_running(true),
      m_event(),
      m_lastFallTime(SDL_GetTicks()),
      m_lastMoveTime(0),
      m_renderer(),
      m_bag7(),
      m_tetromino(pickRandomTetromino()) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    // Initialization failed, output the error
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    stop();
    return;
  }

  m_renderer.init(GAME_TITLE);

  m_tetromino.setStartPosition();
}

Game::~Game() { SDL_Quit(); }

void Game::placeTetrominoOnGrid() {
  m_grid.placeTetromino(m_tetromino);
  m_grid.checkFullRows();
  createNewTetromino();
}

void Game::createNewTetromino() {
  m_tetromino = pickRandomTetromino();
  m_tetromino.setStartPosition();
}

Tetromino Game::pickRandomTetromino() { return Tetromino(m_bag7.pickNext()); }

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
  if (now - m_lastFallTime >= m_timeToFall) {
    if (m_tetromino.moveIfCan(Direction::DOWN, m_grid.getGrid())) {
      m_lastFallTime = now;

#ifdef DEBUG
      auto pos = m_tetromino.getPosition();
      std::cout << "[" << pos[0] << " " << pos[1] << "]" << std::endl;
#endif  // DEBUG

      return;
    }
  }

  // Independently check the lock timer
  if (m_tetromino.canMoveDown(m_grid.getGrid())) {
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
  drawGrid();

  m_tetromino.draw(m_renderer.getRenderer());
  m_renderer.present();
}

void Game::drawGrid() {
  SDL_SetRenderDrawColor(m_renderer.getRenderer(), 50, 50, 50,
                         255);  // Light gray m_color for m_grid lines

  // Draw vertical m_grid lines
  for (int x = 0; x <= BOARD_WIDTH; ++x) {
    int screenX = x * BLOCK_SIZE;
    SDL_RenderLine(m_renderer.getRenderer(), screenX, 0, screenX,
                   BOARD_HEIGHT * BLOCK_SIZE);
  }

  // Draw horizontal m_grid lines
  for (int y = 0; y <= BOARD_HEIGHT; ++y) {
    int screenY = y * BLOCK_SIZE;
    SDL_RenderLine(m_renderer.getRenderer(), 0, screenY,
                   BOARD_WIDTH * BLOCK_SIZE, screenY);
  }

  const auto& grid = m_grid.getGrid();
  for (int y = 0; y < BOARD_HEIGHT; ++y) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
      if (grid[y][x] == 1) {  // If the cell is occupied (1), draw it
        m_renderer.drawBlock(x * BLOCK_SIZE, y * BLOCK_SIZE);
      }
    }
  }
}

bool Game::isGameOver() {
  std::array<std::array<int, 2>, 4> blocks = m_tetromino.getBlocks();
  for (const auto& block : blocks) {
    int x = block[0];
    int y = block[1];
    if (m_grid.isCellOccupied(x, y)) {
      return true;  // Collision at spawn → game over
    }
  }
  return false;
}

void Game::stop() { m_running = false; }

void Game::handleInput() {
  Uint32 now = SDL_GetTicks();

  // hold delay logic => DSA + ARR
  if (m_inputHandler.isKeyPressed(SDLK_LEFT)) {
    Uint32 holdTime = m_inputHandler.getKeyHoldTime(SDLK_LEFT);
    if (m_inputHandler.isKeyJustPressed(SDLK_LEFT) ||
        holdTime > m_DAS && now - m_lastMoveTime > m_ARR) {
      m_tetromino.moveIfCan(Direction::LEFT, m_grid.getGrid());
      m_lastMoveTime = now;
    }
  }

  // hold delay logic => DSA + ARR
  if (m_inputHandler.isKeyPressed(SDLK_RIGHT)) {
    Uint32 holdTime = m_inputHandler.getKeyHoldTime(SDLK_RIGHT);
    if (m_inputHandler.isKeyJustPressed(SDLK_RIGHT) ||
        holdTime > m_DAS && now - m_lastMoveTime > m_ARR) {
      m_tetromino.moveIfCan(Direction::RIGHT, m_grid.getGrid());
      m_lastMoveTime = now;
    }
  }

  if (m_inputHandler.isKeyPressed(SDLK_DOWN)) {
    m_tetromino.moveIfCan(Direction::DOWN, m_grid.getGrid());
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_SPACE)) {
    m_tetromino.hardDrop(m_grid.getGrid());
    placeTetrominoOnGrid();
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_Z) ||
      m_inputHandler.isKeyJustPressed(SDLK_UP)) {
    m_tetromino.rotate(-90, m_grid.getGrid());
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_X)) {
    m_tetromino.rotate(90, m_grid.getGrid());
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_C)) {
    m_tetromino.rotate(180, m_grid.getGrid());
  }
}