#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    void update();
    bool isKeyPressed(SDL_Scancode key);

private:
    const Uint8* keyboardState;
};

#endif
