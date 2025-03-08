#include "renderer.h"
#include "constants.h"

void Renderer::init(const char* title) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

void Renderer::cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Renderer::drawBlock(int x, int y) {
    SDL_Rect block = { x, y, BLOCK_SIZE, BLOCK_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &block);
}
