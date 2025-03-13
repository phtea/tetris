// input_handler.cpp
// Implements input handling logic

#include "input_handler.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <iostream>

void InputHandler::pollEvents() {
	SDL_Event event;
	Uint64 now = SDL_GetTicks();

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			// TODO: handle quitting
			std::cout << "Quit event received" << std::endl;
			quit = true;
		}

		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.repeat) {
				keyStates[event.key.key] = KeyStateType::isHeld;
			} else {
				keyStates[event.key.key] = KeyStateType::isJustPressed;

				// if just pressed the button - store its hold start time
				keyHoldStartTimes[event.key.key] = now;
			}

			// Quit on Escape key
			if (event.key.key == SDLK_ESCAPE) {
				std::cout << "Escape key pressed. Quitting..." << std::endl;
				quit = true;
			}
		}
		if (event.type == SDL_EVENT_KEY_UP) {
			keyStates[event.key.key] = KeyStateType::isNotHeld;
			keyHoldStartTimes.erase(event.key.key);
		}
	}
}

bool InputHandler::isKeyPressed(SDL_Keycode key) const {
	auto it = keyStates.find(key);
	return (it != keyStates.end() && (it->second == KeyStateType::isJustPressed || it->second == KeyStateType::isHeld));
}

bool InputHandler::isKeyJustPressed(SDL_Keycode key) {
	auto it = keyStates.find(key);
	if (it != keyStates.end() && it->second == KeyStateType::isJustPressed) {
		keyStates[key] = KeyStateType::isHeld;
		return true;
	}
	return false;
}

Uint32 InputHandler::getKeyHoldTime(SDL_Keycode key) const {
	auto it = keyHoldStartTimes.find(key);
	if (it != keyHoldStartTimes.end()) {
		return SDL_GetTicks() - it->second; // return hold duration
	}
	return 0;
}
