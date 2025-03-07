#include "input_handler.h"

InputHandler::InputHandler() {
    keyboardState = SDL_GetKeyboardState(NULL);
}

InputHandler::~InputHandler() {
    // Nothing to clean up for now
}

void InputHandler::update() {
    SDL_PumpEvents();  // Update keyboard state
    keyboardState = SDL_GetKeyboardState(NULL);
}

bool InputHandler::isKeyPressed(SDL_Scancode key) {
    return keyboardState[key];  // Returns true if key is currently pressed
}
