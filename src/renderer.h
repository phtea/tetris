#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Renderer {
public:
	Renderer(const char* title, int screenWidth, int screenHeight);
	~Renderer();
	void clear();
	void present();
	void drawBlock(int x, int y, const SDL_Color& color);
	void setDrawColor(const SDL_Color& color);
	void setGridSize(int gridWidth, int gridHeight);

	void drawLine(int x1, int y1, int x2, int y2);
	void drawGrid(int gridWidth, int gridHeight);

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_blockTexture;
	int m_screenWidth;
	int m_screenHeight;
	int m_blockSize;
};