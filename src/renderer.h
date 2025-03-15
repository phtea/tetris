#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Renderer {
public:
	Renderer(const char* title);
	~Renderer();
	void clear();
	void present();
	void drawBlock(int x, int y, const SDL_Color& color);
	void setDrawColor(const SDL_Color& color);
	void drawLine(int x1, int y1, int x2, int y2);

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_blockTexture;
};