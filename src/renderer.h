// m_renderer.h
// Defines rendering logic

#pragma once

#include <SDL3/SDL.h>

class Renderer {
 public:
  Renderer() : m_window(nullptr), m_renderer(nullptr) {}
  ~Renderer();

  void init(const char* title);
  void clear();
  void present();
  void drawBlock(int x, int y);

  SDL_Renderer* getRenderer() { return m_renderer; }

 private:
  SDL_Window* m_window;
  SDL_Renderer* m_renderer;
};