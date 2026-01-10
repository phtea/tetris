#include "Hud.h"
#include "CustomTypes.h"
#include "Renderer.h"
#include <string>

Hud::Hud(const Position& pos, float scale)
    : m_hudX(pos.x), m_hudY(pos.y), m_elementSpacing(50), m_hudScale(scale), m_currentElementPos(0),
      m_showNext(true), m_showHold(true), m_hudBordersEnabled(true) {}

void Hud::update(Renderer &renderer, int nextCount) {
    m_elementSpacing = renderer.getBlockSize();
    m_NextCount = nextCount;
}

void Hud::draw(Renderer &renderer, const std::queue<Mino> &minos, const Mino &bufferMino, int level, int score) {
    m_currentElementPos = 0; // Reset the current element position
		showStat(renderer, "Level", level);
		showStat(renderer, "Score", score);
    if (m_showNext) {
        renderNextTetromino(renderer, minos, m_NextCount);
    }
    if (m_showHold) {
        renderBufferTetromino(renderer, bufferMino);
    }
    if (m_hudBordersEnabled) {
        drawBorders(renderer);
    }
}

void Hud::showStat(Renderer &renderer, const std::string& statLabel, int stat) {
    const float labelX = renderer.calculateHudX(m_hudX);
    const float labelY = renderer.calculateHudY(m_hudY + m_currentElementPos);
    const ScreenPosition pos{labelX, labelY};

		const std::string levelLabel = statLabel + ": " + std::to_string(stat);
    renderer.drawTextAtPixel(levelLabel, pos, false);
    m_currentElementPos += static_cast<int>(m_elementSpacing); // Update the current element position
}

void Hud::move(int deltaX, int deltaY) {
    m_hudX += deltaX;
    m_hudY += deltaY;
}

void Hud::setShowNext(bool show) { m_showNext = show; }

void Hud::setShowHold(bool show) { m_showHold = show; }

void Hud::drawBorders(Renderer &renderer) const {
    // Set the border color (e.g., white)
    const SDL_Color borderColor = {255, 255, 255, 255};
    renderer.setDrawColor(borderColor);

    // Calculate HUD dimensions
    const float hudX = renderer.calculateHudX(m_hudX) * 0.99F;
    const float hudY = renderer.calculateHudY(m_hudY);

    // Calculate the width and height based on the elements
    const float hudWidth = renderer.getBlockSize() * 4; // Assuming the width of the HUD is 4 blocks
    const auto hudHeight = static_cast<float>(m_currentElementPos);        // Total height of the elements plus some spacing

    // Draw the borders
    renderer.drawLine(hudX, hudY, hudX + hudWidth, hudY);                         // Top border
    renderer.drawLine(hudX, hudY, hudX, hudY + hudHeight);                        // Left border
    renderer.drawLine(hudX + hudWidth, hudY, hudX + hudWidth, hudY + hudHeight);  // Right border
    renderer.drawLine(hudX, hudY + hudHeight, hudX + hudWidth, hudY + hudHeight); // Bottom border
}

void Hud::renderNextTetromino(Renderer &renderer, std::queue<Mino> minos, int count) {
    renderTetromino(renderer, "Next:", std::move(minos), count);
}

void Hud::renderBufferTetromino(Renderer &renderer, const Mino &bufferMino) {
    std::queue<Mino> bufferQueue;
    if (bufferMino.getType() != MinoType::NONE) {
        bufferQueue.push(bufferMino);
    }
    renderTetromino(renderer, "Hold:", bufferQueue, 1);
}

void Hud::renderTetromino(Renderer &renderer, const std::string &label, std::queue<Mino> tetrominos,
                          int count) {
    const float labelX = renderer.calculateHudX(m_hudX);
    const float labelY = renderer.calculateHudY(m_hudY + m_currentElementPos);

    const ScreenPosition pos{static_cast<float>(labelX), static_cast<float>(labelY)};

    renderer.drawTextAtPixel(label, pos, false);

    const float blockSize = renderer.getBlockSize() * m_hudScale;
    const int minoSpacing = static_cast<int>(blockSize) * 3; // Space each mino by 3 block heights (adjust as needed)

    for (auto i = 0; i < count && !tetrominos.empty(); ++i) {
        const Mino t = tetrominos.front();
        tetrominos.pop();

        const SDL_Color blockColor = t.getColor();
        const auto blocks = t.getBlocks();

        // Calculate vertical offset for this mino
        const int yOffset = static_cast<int>(m_elementSpacing) + (i * minoSpacing);

        for (const auto &block : blocks) {
            const float x = renderer.calculateHudX(m_hudX + (static_cast<double>(block[0]) * blockSize));
            const float y = renderer.calculateHudY(m_hudY + (static_cast<double>(block[1]) * blockSize) + yOffset + m_currentElementPos);

            renderer.drawBlockAtPixel(x, y, blockColor, blockSize);
        }
    }

    m_currentElementPos += static_cast<int>(m_elementSpacing * 2) + (count * minoSpacing); // Update the current element position
}
