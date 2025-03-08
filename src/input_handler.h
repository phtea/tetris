#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL_keycode.h>
#include <unordered_map>

class InputHandler {
public:
    InputHandler() = default;
    ~InputHandler() = default;

	void pollEvents();
    bool isKeyPressed(SDL_Keycode key) const;

private:
	std::unordered_map<SDL_Keycode, bool> keyStates;
};

#endif
