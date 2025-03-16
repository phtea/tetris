#pragma once

#include <SDL3/SDL.h>
#include "tetromino.h"
#include "renderer.h"
#include <queue>

class Hud {
public:
	Hud();
	~Hud();

	void update(Renderer& renderer, const std::queue<Tetromino>& tetrominos, int count, const Tetromino& bufferTetromino);
	void render(Renderer& renderer);

private:
	int m_nextTetrominoX;
	int m_nextTetrominoY;
	int m_bufferTetrominoX;
	int m_bufferTetrominoY;

	// New method to render the next tetromino
	void renderNextTetromino(Renderer& renderer, const std::queue<Tetromino> tetrominos, int count);
	void renderBufferTetromino(Renderer& renderer, const Tetromino& bufferTetromino);
};