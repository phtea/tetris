#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>

class Renderer {
public:
	Renderer(const char* title, int screenWidth, int screenHeight);
	~Renderer();
	void clear();
	void present();
	void setDrawColor(const SDL_Color& color);
	void setGridSize(int gridWidth, int gridHeight);

	void setResolution(int newWidth, int newHeight);

	void drawLine(int x1, int y1, int x2, int y2);
	void drawGrid(int gridWidth, int gridHeight);
	void drawBlock(int x, int y, const SDL_Color& color);
	void drawText(const std::string& text, int x, int y);

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_blockTexture;
	TTF_Font* m_font;

	int m_screenWidth;
	int m_screenHeight;
	int m_xOffset;
	int m_yOffset;
	int m_blockSize;
};