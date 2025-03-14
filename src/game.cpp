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

  // TODO: make array of BOARD_HEIGHT and BOARD_WIDTH
  m_grid = grid_t(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
}

Game::~Game() { SDL_Quit(); }

void Game::placeTetrominoOnGrid() {
  for (const auto& block : m_tetromino.getBlocks()) {
    int gridX = block[0] / BLOCK_SIZE;
    int gridY = block[1] / BLOCK_SIZE;
    if (gridX >= 0 && gridX < BOARD_WIDTH && gridY >= 0 &&
        gridY < BOARD_HEIGHT) {  // TODO: make sure this bounds check is needed
      m_grid[gridY][gridX] = 1;
    }
  }

  // Check for full rows and remove them
  for (int y = 0; y < BOARD_HEIGHT; ++y) {
    if (std::all_of(m_grid[y].begin(), m_grid[y].end(),
                    [](int cell) { return cell == 1; })) {
      std::cout << "Row " << y << " is full and will be cleared" << std::endl;

      // Move all rows above down by one
      for (int row = y; row > 0; --row) {
        m_grid[row] = m_grid[row - 1];
      }

      // Clear the top row
      m_grid[0] = std::vector<int>(BOARD_WIDTH, 0);
    }
  }

  // Reset the touch state and timer after placing the tetromino
  m_touchState = TouchState::NotTouching;
  m_lastLockTime = 0;  // Reset lock timer
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
    if (m_tetromino.moveIfCan(Direction::DOWN, BLOCK_SIZE, m_grid)) {
      m_lastFallTime = now;

#ifdef DEBUG
      auto pos = m_tetromino.getPosition();
      std::cout << "[" << pos[0] << " " << pos[1] << "]" << std::endl;
#endif  // DEBUG

      return;
    }
  }

  // Independently check the lock timer
  if (m_tetromino.canMoveDown(m_grid)) {
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
        createNewTetromino();

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

  // Then, draw the m_blocks (the colored cells that are part of the m_tetromino
  // or fixed)
  for (int y = 0; y < BOARD_HEIGHT; ++y) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
      if (m_grid[y][x] == 1) {  // If the cell is occupied (1), draw it
        m_renderer.drawBlock(x * BLOCK_SIZE, y * BLOCK_SIZE);
      }
    }
  }
}

bool Game::isGameOver() {
  std::array<std::array<int, 2>, 4> blocks = m_tetromino.getBlocks();
  for (const auto& block : blocks) {
    int x = block[0] / BLOCK_SIZE;
    int y = block[1] / BLOCK_SIZE;
    if (m_grid[y][x] != 0) {
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
      m_tetromino.moveIfCan(Direction::LEFT, BLOCK_SIZE, m_grid);
      m_lastMoveTime = now;
    }
  }

  // hold delay logic => DSA + ARR
  if (m_inputHandler.isKeyPressed(SDLK_RIGHT)) {
    Uint32 holdTime = m_inputHandler.getKeyHoldTime(SDLK_RIGHT);
    if (m_inputHandler.isKeyJustPressed(SDLK_RIGHT) ||
        holdTime > m_DAS && now - m_lastMoveTime > m_ARR) {
      m_tetromino.moveIfCan(Direction::RIGHT, BLOCK_SIZE, m_grid);
      m_lastMoveTime = now;
    }
  }

  if (m_inputHandler.isKeyPressed(SDLK_DOWN)) {
    m_tetromino.moveIfCan(Direction::DOWN, BLOCK_SIZE, m_grid);
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_SPACE)) {
    m_tetromino.hardDrop(BLOCK_SIZE, m_grid);
    placeTetrominoOnGrid();
    createNewTetromino();
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_Z) ||
      m_inputHandler.isKeyJustPressed(SDLK_UP)) {
    m_tetromino.rotate(-90, m_grid);
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_X)) {
    m_tetromino.rotate(90, m_grid);
  }

  if (m_inputHandler.isKeyJustPressed(SDLK_C)) {
    m_tetromino.rotate(180, m_grid);
  }
}