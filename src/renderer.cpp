#include "renderer.h"

#include <iostream>

#include "constants.h"

Renderer::Renderer(const char* title, int screenWidth, int screenHeight)
	: m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_xOffset(0), m_yOffset(0) {
	if (!SDL_CreateWindowAndRenderer(title, m_screenWidth, m_screenHeight, 0, &m_window, &m_renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window and renderer: %s\n",
			SDL_GetError());
	}
	setGridSize(GRID_WIDTH, GRID_HEIGHT);
	m_blockTexture = nullptr;

	// Ensure SDL_ttf is initialized (you can do this once at the start of your program)
	if (!TTF_Init()) {
		std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << SDL_GetError() << std::endl;
		return;
	}

	// Load font
	m_font = TTF_OpenFont("C:/dev/Tetris/fonts/Kgsecondchancessketch.ttf", 24);
	if (!m_font) {
		std::cerr << "Failed to load font! SDL_ttf Error: " << SDL_GetError() << std::endl;
		return;
	}


	//m_blockTexture = IMG_LoadTexture(m_renderer, "C:/dev/Tetris/assets/block.png");
	//if (!m_blockTexture) {
	//	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create block texture: %s\n",
	//		SDL_GetError());
	//}
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_DestroyTexture(m_blockTexture);
	TTF_CloseFont(m_font);
}

void Renderer::clear() {
	if (!SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Could not set render draw color: %s\n", SDL_GetError());
	}
	if (!SDL_RenderClear(m_renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not clear render: %s\n",
			SDL_GetError());
	}
}

void Renderer::present() {
	if (!SDL_RenderPresent(m_renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not present render: %s\n",
			SDL_GetError());
	}
}


//void Renderer::drawBlock(int x, int y, const SDL_Color& color) {
//    if (y < 0) return;
//
//    SDL_FRect block = { x, y, BLOCK_SIZE, BLOCK_SIZE };
//    if (!SDL_RenderTexture(m_renderer, m_blockTexture, NULL, &block)) {
//        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not render block texture: %s\n", SDL_GetError());
//    }
//}


// Draws block based on X and Y (compared to grid). blockSize is handled here 
void Renderer::drawBlock(int x, int y, const SDL_Color& color) {
	if (y < 0) return;

	SDL_FRect block = { m_xOffset + x * m_blockSize,
						m_yOffset + y * m_blockSize,
						m_blockSize,
						m_blockSize };

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_renderer, &block);
}

void Renderer::drawText(const std::string& text, int x, int y) {


	SDL_Color color = { 255, 255, 255, 255 }; // White text color

	// Create a surface from the text
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, text.c_str(), 0, color);
	if (!textSurface) {
		std::cerr << "Unable to create text surface! SDL_ttf Error: " << SDL_GetError() << std::endl;
		TTF_CloseFont(m_font);
		return;
	}

	// Create texture from surface
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
	SDL_DestroySurface(textSurface);  // No longer needed

	// Render the text
	SDL_FRect renderQuad = { x+m_xOffset*2, y+m_blockSize, textSurface->w, textSurface->h };
	SDL_RenderTexture(m_renderer, textTexture, NULL, &renderQuad);

	// Clean up
	SDL_DestroyTexture(textTexture);
}

void Renderer::setDrawColor(const SDL_Color& color) {
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderLine(m_renderer, x1, y1, x2, y2);
}

void Renderer::drawGrid(int gridWidth, int gridHeight) {
	for (int x = 0; x <= gridWidth; ++x) {
		int screenX = m_xOffset + x * m_blockSize;
		drawLine(screenX, m_yOffset, screenX, m_yOffset + gridHeight * m_blockSize);
	}

	for (int y = 0; y <= gridHeight; ++y) {
		int screenY = m_yOffset + y * m_blockSize;
		drawLine(m_xOffset, screenY, m_xOffset + gridWidth * m_blockSize, screenY);
	}
}

void Renderer::setGridSize(int gridWidth, int gridHeight) {
	m_blockSize = std::min(m_screenWidth / gridWidth, m_screenHeight / gridHeight);

	// Calculate offsets to center the grid
	int gridWidthPx = gridWidth * m_blockSize;
	int gridHeightPx = gridHeight * m_blockSize;

	m_xOffset = (m_screenWidth - gridWidthPx) / 2;
	m_yOffset = (m_screenHeight - gridHeightPx) / 2;
}

void Renderer::setResolution(int newWidth, int newHeight) {
	m_screenWidth = newWidth;
	m_screenHeight = newHeight;

	// Update SDL window size
	SDL_SetWindowSize(m_window, m_screenWidth, m_screenHeight);

	// Recalculate grid and block size
	setGridSize(GRID_WIDTH, GRID_HEIGHT);
}