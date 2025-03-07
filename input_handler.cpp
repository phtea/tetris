#include "input_handler.h"
#include <SDL2/SDL.h>

InputHandler::InputHandler() {
    keyboardState = SDL_GetKeyboardState(NULL);
	memset(previousState, 0, sizeof(previousState)); // initialize previous state to zero
}

InputHandler::~InputHandler() {
    // Nothing to clean up for now
}

void InputHandler::update() {
	// store current state in previous state and update keyboardState
	memcpy(previousState, keyboardState, sizeof(previousState));
	keyboardState = SDL_GetKeyboardState(NULL);
}

// Returns true if key was not pressed but now is pressed
// (state: 0->1)
bool InputHandler::isKeyPressed(SDL_Scancode key) {
    return previousState[key] == 0 && keyboardState[key] == 1;
}

// Returns true if key is pressed and was NOT pressed previously
// (state: 1->0)
bool InputHandler::isKeyReleased(SDL_Scancode key) {
    return previousState[key] == 1 && keyboardState[key] == 0;
}
