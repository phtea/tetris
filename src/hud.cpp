#include "Hud.h"

Hud::Hud(int hudX, int hudY, float scale)
    : m_hudX(hudX), m_hudY(hudY), m_elementSpacing(50), m_hudScale(scale), m_currentElementPos(0),
      m_showNext(true), m_showHold(true), m_hudBordersEnabled(true) {}

Hud::~Hud() {}

void Hud::update(Renderer &renderer, int nextCount) {
    m_elementSpacing = renderer.getBlockSize();
    m_NextCount = nextCount;
}

void Hud::draw(Renderer &renderer, const std::queue<Mino> &nextMinos, const Mino &bufferMino) {
    m_currentElementPos = 0; // Reset the current element position
    if (m_showNext) {
        renderNextTetromino(renderer, nextMinos, m_NextCount);
    }
    if (m_showHold) {
        renderBufferTetromino(renderer, bufferMino);
    }
    if (m_hudBordersEnabled) {
        drawBorders(renderer);
    }
}

void Hud::move(int deltaX, int deltaY) {
    m_hudX += deltaX;
    m_hudY += deltaY;
}

void Hud::setShowNext(bool show) { m_showNext = show; }

void Hud::setShowHold(bool show) { m_showHold = show; }

void Hud::drawBorders(Renderer &renderer) {
    // Set the border color (e.g., white)
    SDL_Color borderColor = {255, 255, 255, 255};
    renderer.setDrawColor(borderColor);

    // Calculate HUD dimensions
    int hudX = renderer.calculateHudX(m_hudX) * 0.99;
    int hudY = renderer.calculateHudY(m_hudY);

    // Calculate the width and height based on the elements
    int hudWidth = renderer.getBlockSize() * 4; // Assuming the width of the HUD is 4 blocks
    int hudHeight = m_currentElementPos;        // Total height of the elements plus some spacing

    // Draw the borders
    renderer.drawLine(hudX, hudY, hudX + hudWidth, hudY);                         // Top border
    renderer.drawLine(hudX, hudY, hudX, hudY + hudHeight);                        // Left border
    renderer.drawLine(hudX + hudWidth, hudY, hudX + hudWidth, hudY + hudHeight);  // Right border
    renderer.drawLine(hudX, hudY + hudHeight, hudX + hudWidth, hudY + hudHeight); // Bottom border
}

void Hud::renderNextTetromino(Renderer &renderer, std::queue<Mino> tetrominos, int count) {
    renderTetromino(renderer, "Next:", tetrominos, count);
}

void Hud::renderBufferTetromino(Renderer &renderer, const Mino &bufferTetromino) {
    std::queue<Mino> bufferQueue;
    if (bufferTetromino.getType() != MinoType::NONE) {
        bufferQueue.push(bufferTetromino);
    }
    renderTetromino(renderer, "Hold:", bufferQueue, 1);
}

void Hud::renderTetromino(Renderer &renderer, const std::string &label, std::queue<Mino> tetrominos,
                          int count) {
    int labelX = renderer.calculateHudX(m_hudX);
    int labelY = renderer.calculateHudY(m_hudY + m_currentElementPos);
    ScreenPosition pos(labelX, labelY);
    renderer.drawTextAtPixel(label, pos);

    int yOffset = m_elementSpacing; // Offset to space out the nextMinos visually

    int blockSize = renderer.getBlockSize() * m_hudScale;

    for (int i = 0; i < count && !tetrominos.empty(); ++i) {
        Mino t = tetrominos.front();
        tetrominos.pop();

        SDL_Color blockColor = t.getColor();
        auto blocks = t.getBlocks();

        for (const auto &block : blocks) {
            int x = renderer.calculateHudX(m_hudX + static_cast<int>(block[0] * blockSize));
            int y = renderer.calculateHudY(m_hudY + static_cast<int>(block[1] * blockSize) +
                                           yOffset + m_currentElementPos);
            renderer.drawBlockAtPixel(x, y, blockColor, blockSize);
        }

        yOffset += m_elementSpacing; // Move each next mino down visually
    }

    m_currentElementPos += yOffset + m_elementSpacing; // Update the current element position
}
