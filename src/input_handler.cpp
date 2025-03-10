// input_handler.cpp
// Implements input handling logic

#include "input_handler.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <iostream>

void InputHandler::pollEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			// TODO: handle quitting
			std::cout << "Quit event received" << std::endl;
			quit = true;
		}

		if (event.type == SDL_KEYDOWN) {
			if (!keyStates[event.key.keysym.sym]) {
				justPressed[event.key.keysym.sym] = true;
			}
			keyStates[event.key.keysym.sym] = true;

			// Quit on Escape key
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				std::cout << "Escape key pressed. Quitting..." << std::endl;
				quit = true;
			}
		}

		if (event.type == SDL_KEYUP) {
			keyStates[event.key.keysym.sym] = false;
			justPressed[event.key.keysym.sym] = false;
		}
	}
}

bool InputHandler::isKeyPressed(SDL_Keycode key) const {
	auto it = keyStates.find(key);
	return it != keyStates.end() && it->second;
}

bool InputHandler::isKeyJustPressed(SDL_Keycode key) {
	auto it = justPressed.find(key);
    if (it != keyStates.end() && it->second) {
        justPressed[key] = false;
        return true;
    }
    return false;
}
