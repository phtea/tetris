// renderer.h
// Defines rendering logic

#pragma once

#include <SDL3/SDL.h>

class Renderer {
public:
    Renderer() : window(nullptr), renderer(nullptr) {} // init pointers to nullptr
    ~Renderer();

    void init(const char* title);
    void clear();
    void present();
	void drawBlock(int x, int y);

    SDL_Renderer* getRenderer() { return renderer; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};