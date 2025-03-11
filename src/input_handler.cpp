// input_handler.cpp
// Implements input handling logic

#include "input_handler.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <iostream>

void InputHandler::pollEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			// TODO: handle quitting
			std::cout << "Quit event received" << std::endl;
			quit = true;
		}

		if (event.type == SDL_EVENT_KEY_DOWN) {
			/*if (!keyStates[event.key.key]) {
				justPressed[event.key.key] = true;
			}*/
			if (!event.key.repeat) {
				justPressed[event.key.key] = true;
			}
			keyStates[event.key.key] = true;

			// Quit on Escape key
			if (event.key.key == SDLK_ESCAPE) {
				std::cout << "Escape key pressed. Quitting..." << std::endl;
				quit = true;
			}
		}

		if (event.type == SDL_EVENT_KEY_UP) {
			keyStates[event.key.key] = false;
			justPressed[event.key.key] = false;
		}
	}
}

bool InputHandler::isKeyPressed(SDL_Keycode key) const {
	auto it = keyStates.find(key);
	return it != keyStates.end() && it->second;
}

bool InputHandler::isKeyJustPressed(SDL_Keycode key) {
	auto it = justPressed.find(key);
    if (it != justPressed.end() && it->second) {
        justPressed[key] = false;
        return true;
    }
    return false;
}
