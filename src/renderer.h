#pragma once

#include <SDL3/SDL.h>

class Renderer {
 public:
  Renderer() : m_window(nullptr), m_renderer(nullptr) {}
  ~Renderer();

  void init(const char* title);
  void clear();
  void present();
  void drawBlock(int x, int y, const SDL_Color& color);
  void setDrawColor(const SDL_Color& color);
  void drawLine(int x1, int y1, int x2, int y2);

  SDL_Renderer* getRenderer() { return m_renderer; }

 private:
  SDL_Window* m_window;
  SDL_Renderer* m_renderer;
};