// input_handler.h
// Defines input handling logic

#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include <unordered_map>

enum class KeyStateType : uint8_t { isJustPressed, isHeld, isNotHeld };

class InputHandler {
public:
	InputHandler() = default;
	~InputHandler() = default;

	void pollEvents();
	bool isKeyPressed(SDL_Keycode key) const;
	bool isKeyJustPressed(SDL_Keycode key);
	Uint32 getKeyHoldTime(SDL_Keycode key) const;

	bool shouldQuit() const { return quit; }

private:
	std::unordered_map<SDL_Keycode, KeyStateType> m_keyStates;
	std::unordered_map<SDL_Keycode, Uint64> m_keyHoldStartTimes;
	bool quit = false;
};