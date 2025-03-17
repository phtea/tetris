#include "hud.h"
#include <iostream>

Hud::Hud(int hudX, int hudY)
	: m_hudX(hudX), m_hudY(hudY), m_elementSpacing(50), m_hudScale(0.5f) { // Example spacing
}

Hud::~Hud() {}

void Hud::update(Renderer& renderer, const std::queue<Tetromino>& tetrominos, int count, const Tetromino& bufferTetromino) {
	m_elementSpacing = renderer.getBlockSize();
	m_currentElementPos = 0; // Reset the current element position
	if (m_showNext) {
		renderNextTetromino(renderer, tetrominos, count);
	}
	if (m_showHold) {
		renderBufferTetromino(renderer, bufferTetromino);
	}
}

void Hud::render(Renderer& renderer) {
	// You could render additional information like score, level, etc. here
	// For now, we just need to display the next tetromino and buffer tetromino
}

void Hud::move(int deltaX, int deltaY) {
	m_hudX += deltaX;
	m_hudY += deltaY;
}

void Hud::setShowNext(bool show) {
	m_showNext = show;
}

void Hud::setShowHold(bool show) {
	m_showHold = show;
}

void Hud::renderNextTetromino(Renderer& renderer, std::queue<Tetromino> tetrominos, int count) {
	renderTetromino(renderer, "Next:", tetrominos, count);
}

void Hud::renderBufferTetromino(Renderer& renderer, const Tetromino& bufferTetromino) {
	std::queue<Tetromino> bufferQueue;
	if (bufferTetromino.getType() != TetrominoType::NONE) {
		bufferQueue.push(bufferTetromino);
	}
	renderTetromino(renderer, "Hold:", bufferQueue, 1);
}

void Hud::renderTetromino(Renderer& renderer, const std::string& label, std::queue<Tetromino> tetrominos, int count) {
	int labelX = renderer.calculateHudX(m_hudX);
	int labelY = renderer.calculateHudY(m_hudY + m_currentElementPos);
	renderer.drawTextAtPixel(label, labelX, labelY);

	int yOffset = m_elementSpacing; // Offset to space out the tetrominos visually

	int blockSize = renderer.getBlockSize() * m_hudScale;

	for (size_t i = 0; i < count && !tetrominos.empty(); ++i) {
		Tetromino t = tetrominos.front();
		tetrominos.pop();

		SDL_Color blockColor = t.getColor();
		auto blocks = t.getBlocks();

		for (const auto& block : blocks) {
			int x = renderer.calculateHudX(m_hudX + static_cast<int>(block[0] * blockSize));
			int y = renderer.calculateHudY(m_hudY + static_cast<int>(block[1] * blockSize) + yOffset + m_currentElementPos);
			renderer.drawBlockAtPixel(x, y, blockColor, blockSize);
		}

		yOffset += m_elementSpacing; // Move each next tetromino down visually
	}

	m_currentElementPos += yOffset + m_elementSpacing; // Update the current element position
}