#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

class Renderer {
public:
    void init(const char* title);
    void clear();
    void present();
    void cleanup();
    SDL_Renderer* getRenderer() { return renderer; }
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif
