#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_ROWS = 20;
const int GRID_COLS = 10;
const int BLOCK_SIZE = 30;

class Tetrimino {
public:
	int shape[4][4]; // 4x4 matrix for tetrimino shape
	int x, y;        // positions on grid
	SDL_Color color; // color of tetrimino

	Tetrimino(int type) {
		x = 3; // start pos
		y = 0;
		color = {255, 0, 0, 255}; // red color

		// a simple square block (O-tetrimino)
		int square[4][4] = {
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		};

		// Copy shape to class variable
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				shape[i][j] = square[i][j];
			}
		}
	}

	void draw(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (shape[i][j]) {
					SDL_Rect block = {(x+j) * BLOCK_SIZE, (y+i) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
					SDL_RenderFillRect(renderer, &block);
				}
			}
		}
	}
};

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
			<< std::endl;
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Tetris SDL2", SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
									   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
			<< std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer =
		SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError()
			<< std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	bool running = true;
	SDL_Event event;

	Tetrimino tetrimino(0); // create an 0-tetrimino

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		tetrimino.draw(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
