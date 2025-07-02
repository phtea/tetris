constexpr bool DEBUG_NO_FALL = false;
constexpr bool DEBUG_NO_LOCK = false;

#include "Game.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include <cstdlib>
#include <iostream>
#include <ostream>

#include "Constants.h"
#include "Logger.h"
#include "Mino.h"

Game::Game(int screenWidth, int screenHeight)
    : m_Hud(1300, 200, 1.0f), m_running(true), m_lastFallTime(SDL_GetTicks()), m_SDF(100),
      m_renderer(GAME_TITLE, screenWidth, screenHeight), m_Mino(MinoType::NONE), m_nextMinosSize(1),
      m_bufferMino(MinoType::NONE), m_canSwap(true) {
    createNewMino();
}

Game::Game(int screenWidth, int screenHeight, Uint32 timeToFall, Uint32 lockDelayTime, Uint32 das,
           Uint32 arr, Uint32 sdf, int nextMinosSize)
    : m_Hud(1300, 200, 1.0f), m_running(true), m_timeToFall(timeToFall),
      m_lastFallTime(SDL_GetTicks()), m_lockDelayTime(lockDelayTime), m_DAS(das), m_ARR(arr),
      m_SDF(sdf), m_renderer(GAME_TITLE, screenWidth, screenHeight), m_Mino(MinoType::NONE),
      m_nextMinosSize(nextMinosSize), m_bufferMino(MinoType::NONE), m_canSwap(true) {
    createNewMino();
}

void Game::placeMinoOnGrid() {
    m_grid.placeTetromino(m_Mino);
    m_grid.checkFullRows();
    createNewMino();
    m_canSwap = true; // Allow swapping again
}

void Game::createNewMino() {
    if (m_nextMinos.empty()) {
        // Pre-fill the queue with a few upcoming tetrominoes
        for (int i = 0; i < m_nextMinosSize; ++i) {
            m_nextMinos.push(pickRandomMino());
        }
    }

    // Get the next tetromino from the queue
    m_Mino = m_nextMinos.front();
    m_nextMinos.pop();

    // Add a new random tetromino to the queue to maintain the flow
    m_nextMinos.push(pickRandomMino());

    m_Mino.setStartPosition();
}

Mino Game::pickRandomMino() { return Mino(m_bag7.pickNext()); }

void Game::run() {
    SDL_Event event;
    m_gameState = GameState::RUNNING;
    while (m_running) {
        m_inputHandler.pollEvents(event);
        if (m_inputHandler.shouldQuit()) {
            stopGame();
        }
        if (m_gameState == GameState::RUNNING) {
            update();
        }

        handleInput();
        render();
        SDL_Delay(1000 / FPS);
    }
}

void Game::setTimeToFall(Uint32 timeToFall) { m_timeToFall = timeToFall; }

void Game::setLockDelayTime(Uint32 lockDelayTime) { m_lockDelayTime = lockDelayTime; }

void Game::setDAS(Uint32 das) { m_DAS = das; }

void Game::setARR(Uint32 arr) { m_ARR = arr; }

void Game::setSDF(Uint32 sdf) { m_SDF = sdf; }

void Game::setNextMinosSize(int size) { m_nextMinosSize = size; }

void Game::update() {

    Uint64 now = SDL_GetTicks();
    m_Hud.update(m_renderer, m_nextMinosSize);

    handleFallDelay(now);
    handleLockDelay(now);
}

void Game::handleFallDelay(Uint64 now) {
    // Handle the fall behavior (move tetromino down based on m_timeToFall)
    bool canFall = now - m_lastFallTime >= m_timeToFall;
    if (!DEBUG_NO_FALL && canFall && m_Mino.canMove(Direction::DOWN, m_grid.getGrid())) {
        m_Mino.move(Direction::DOWN);
        m_lastFallTime = now;
    }
}

void Game::handleLockDelay(Uint64 now) {
    // Independently check the lock timer
    if (m_Mino.canMove(Direction::DOWN, m_grid.getGrid())) {
        m_touchState = TouchState::NotTouching;
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
        if (now - m_lastLockTime >= m_lockDelayTime && !DEBUG_NO_LOCK) {
            placeMinoOnGrid();

            if (isGameOver()) {
                restartGame();
                m_gameState = GameState::GAMEOVER;
                LOG("Game over! Press R to Restart");
                // std::cout << "Game over! Press R to Restart" << std::endl;
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
    m_Hud.draw(m_renderer, m_nextMinos, m_bufferMino);

    if (m_gameState == GameState::PAUSED) {
        ScreenPosition pos = m_renderer.getResolution();
        pos.x /= 2;
        pos.y /= 2;
        m_renderer.drawTextAtPixel("GAME PAUSED.", pos);
    }

    m_renderer.present();
}

bool Game::isGameOver() {
    std::array<std::array<int, 2>, 4> blocks = m_Mino.getRelativeBlocks();
    for (const auto &block : blocks) {
        int x = block[0];
        int y = block[1];
        if (m_grid.isCellOccupied(x, y)) {
            return true; // Collision at spawn → game over
        }
    }
    return false;
}

void Game::restartGame() {
    // Clear the grid
    m_grid.clear();

    // Reset timers
    m_lastFallTime = SDL_GetTicks();
    m_lastLockTime = SDL_GetTicks();
    m_lastMoveTime = SDL_GetTicks();

    // Reset touch state
    m_touchState = TouchState::NotTouching;

    // Reset ability to swap
    m_canSwap = true;

    // Clear mino queue and refill it
    std::queue<Mino> emptyQueue;
    std::swap(m_nextMinos, emptyQueue);  // Clear queue
    m_bufferMino = Mino(MinoType::NONE); // Reset buffer mino

    // Recreate minos
    createNewMino();

    // Mark game as running again
    m_running = true;
}

void Game::handleInput() {
    Uint32 now = SDL_GetTicks();

    if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_P)) {
        // Toggle pause
        if (m_gameState == GameState::RUNNING) {
            m_gameState = GameState::PAUSED;
        } else if (m_gameState == GameState::PAUSED) {
            m_gameState = GameState::RUNNING;
        }
    }
    if (m_gameState != GameState::RUNNING)
        return;

    handleMovement(Direction::LEFT, SDL_SCANCODE_LEFT, now);
    handleMovement(Direction::RIGHT, SDL_SCANCODE_RIGHT, now);

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

    if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_R)) {
        restartGame();
    }

    if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_LSHIFT) ||
        m_inputHandler.isKeyJustPressed(SDL_SCANCODE_RSHIFT)) {
        swapTetromino();
    }

    handleRotation(-1, {SDL_SCANCODE_Z, SDL_SCANCODE_UP});
    handleRotation(1, {SDL_SCANCODE_X});
    handleRotation(2, {SDL_SCANCODE_C});

    if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_1)) {
        m_renderer.setResolution(1920, 1080);
    }
    if (m_inputHandler.isKeyJustPressed(SDL_SCANCODE_0)) {
        m_renderer.setResolution(1280, 720);
    }
}

// Implement the swapTetromino method
void Game::swapTetromino() {
    if (!m_canSwap) {
        return;
    }
    // TODO: bring back to original rotate state
    if (m_bufferMino.getType() == MinoType::NONE) {
        // Buffer is empty, add current tetromino to buffer and get next one
        m_bufferMino = m_Mino;
        createNewMino();
    } else {
        // Swap current tetromino with the one in the buffer
        std::swap(m_Mino, m_bufferMino);
        m_Mino.setStartPosition();
    }
    m_bufferMino.setOriginalRotationState();
    m_canSwap = false; // Prevent swapping again until the next tetromino is placed
}

void Game::handleMovement(Direction dir, SDL_Scancode key, Uint32 now) {
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
