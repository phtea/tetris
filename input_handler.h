#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    // Update the input state (handle events)
    void update();

    // Check if a key is pressed and released
    bool isKeyPressed(SDL_Scancode key);
    bool isKeyReleased(SDL_Scancode key);

private:
    const Uint8* keyboardState;
	Uint8 previousState[SDL_NUM_SCANCODES];
};

#endif
