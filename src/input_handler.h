// input_handler.h
// Defines input handling logic

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <unordered_map>

class InputHandler {
public:
    InputHandler() = default;
    ~InputHandler() = default;

	void pollEvents();
    bool isKeyPressed(SDL_Keycode key) const;
    bool isKeyJustPressed(SDL_Keycode key);
	bool shouldQuit() const { return quit; }

private:
	std::unordered_map<SDL_Keycode, bool> keyStates;
	std::unordered_map<SDL_Keycode, bool> justPressed;
	bool quit = false; // Track quit event
};

#endif
