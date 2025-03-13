// m_renderer.cpp
// Implements rendering logic

#include "renderer.h"

#include <iostream>

#include "constants.h"

void Renderer::init(const char* title) {
  m_window = SDL_CreateWindow(title, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!m_window) {
    // In the case that the m_window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                 SDL_GetError());
  }
  m_renderer = SDL_CreateRenderer(m_window, NULL);
  if (!m_renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n",
                 SDL_GetError());
  }
}

void Renderer::clear() {
  bool success = SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
  if (!success) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Could not set render draw color: %s\n", SDL_GetError());
  }
  success = SDL_RenderClear(m_renderer);
  if (!success) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not clear render: %s\n",
                 SDL_GetError());
  }
}

void Renderer::present() {
  bool success = SDL_RenderPresent(m_renderer);
  if (!success) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not present render: %s\n",
                 SDL_GetError());
  }
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

void Renderer::drawBlock(int x, int y) {
  // todo: fix segfault if block is above screen
  SDL_FRect block = {x, y, BLOCK_SIZE, BLOCK_SIZE};
  bool success = SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  if (!success) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Could not set render draw color: %s\n", SDL_GetError());
  }
  success = SDL_RenderFillRect(m_renderer, &block);
  if (!success) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not render fill rect: %s\n",
                 SDL_GetError());
  }
}