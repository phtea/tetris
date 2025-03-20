#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>

#include <unordered_map>

enum class KeyStateType : uint8_t { isJustPressed, isHeld, isNotHeld };

class InputHandler {
public:
	InputHandler() = default;
	~InputHandler() = default;

	void pollEvents();

	bool isKeyPressed(SDL_Scancode key) const;
	bool isKeyJustPressed(SDL_Scancode key);
	Uint32 getKeyHoldTime(SDL_Scancode key) const;

	bool shouldQuit() const { return quit; }

private:
	std::unordered_map<SDL_Scancode, KeyStateType> m_keyStates;
	std::unordered_map<SDL_Scancode, Uint64> m_keyHoldStartTimes;
	bool quit = false;
};