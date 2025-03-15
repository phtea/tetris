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
  if (!SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Could not set render draw color: %s\n", SDL_GetError());
  }
  if (!SDL_RenderClear(m_renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not clear render: %s\n",
                 SDL_GetError());
  }
}

void Renderer::present() {
  if (!SDL_RenderPresent(m_renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not present render: %s\n",
                 SDL_GetError());
  }
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

void Renderer::drawBlock(int x, int y, const SDL_Color& color) {
  // Avoid rendering if the block is above the screen
  if (y < 0) return;

  SDL_FRect block = {x, y, BLOCK_SIZE, BLOCK_SIZE};

  if (!SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Could not set render draw color: %s\n", SDL_GetError());
  }

  if (!SDL_RenderFillRect(m_renderer, &block)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not render fill rect: %s\n",
                 SDL_GetError());
  }
}

void Renderer::setDrawColor(const SDL_Color& color) {
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
  SDL_RenderLine(m_renderer, x1, y1, x2, y2);
}
