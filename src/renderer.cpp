// renderer.cpp
// Implements rendering logic

#include "renderer.h"
#include "constants.h"
#include <iostream>

void Renderer::init(const char* title) {
    window = SDL_CreateWindow(title, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
    }
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    bool success = SDL_RenderClear(renderer);
    if (!success) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not clear render: %s\n", SDL_GetError());
    }
}

void Renderer::present() {
    bool success = SDL_RenderPresent(renderer);
    if (!success) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not present render: %s\n", SDL_GetError());
    }
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Renderer::drawBlock(int x, int y) {
	// todo: fix segfault if block is above screen
    SDL_FRect block = { x, y, BLOCK_SIZE, BLOCK_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &block);
}
