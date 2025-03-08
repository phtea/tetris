#include "input_handler.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <iostream>

void InputHandler::pollEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			// handle quitting
			std::cout << "Quit event received" << std::endl;
		}

		if (event.type == SDL_KEYDOWN) {
			keyStates[event.key.keysym.sym] = true;
		}

		if (event.type == SDL_KEYUP) {
			keyStates[event.key.keysym.sym] = false;
		}
	}
}

bool InputHandler::isKeyPressed(SDL_Keycode key) const {
	auto it = keyStates.find(key);
	return it != keyStates.end() && it->second;
}
