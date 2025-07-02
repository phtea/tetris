#include "InputHandler.h"
#include "Logger.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>

#include <iostream>

void InputHandler::pollEvents(SDL_Event &event) {
    Uint64 now = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            LOG("Quit event received");
            quit = true;
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
            if (event.key.repeat) {
                m_keyStates[event.key.scancode] = KeyStateType::isHeld;
            } else {
                m_keyStates[event.key.scancode] = KeyStateType::isJustPressed;

                // if just pressed the button - store its hold start time
                m_keyHoldStartTimes[event.key.scancode] = now;
            }
        }
        if (event.type == SDL_EVENT_KEY_UP) {
            m_keyStates[event.key.scancode] = KeyStateType::isNotHeld;
            m_keyHoldStartTimes.erase(event.key.scancode);
        }
    }
}

bool InputHandler::isKeyPressed(SDL_Scancode key) const {
    auto it = m_keyStates.find(key);
    return (it != m_keyStates.end() &&
            (it->second == KeyStateType::isJustPressed || it->second == KeyStateType::isHeld));
}

bool InputHandler::isKeyJustPressed(SDL_Scancode key) {
    auto it = m_keyStates.find(key);
    if (it != m_keyStates.end() && it->second == KeyStateType::isJustPressed) {
        m_keyStates[key] = KeyStateType::isHeld;
        return true;
    }
    return false;
}

Uint32 InputHandler::getKeyHoldTime(SDL_Scancode key) const {
    auto it = m_keyHoldStartTimes.find(key);
    if (it != m_keyHoldStartTimes.end()) {
        return SDL_GetTicks() - it->second; // return hold duration
    }
    return 0;
}
