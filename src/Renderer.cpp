#include "Renderer.h"
#include "Constants.h"
#include "CustomTypes.h"

Renderer::Renderer(const std::string& title, const ScreenResolution &res)
    : m_res(res) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        return;
    }

    if (!SDL_CreateWindowAndRenderer(title.c_str(), m_res.width, m_res.height, SDL_WINDOW_RESIZABLE,
                                     &m_window, &m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindowAndRenderer Error: %s\n",
                     SDL_GetError());
        return;
    }

    setGridSize({GRID_WIDTH, GRID_HEIGHT});
    m_blockTexture = nullptr;

    // Ensure SDL_ttf is initialized (you can do this once at the start of your program)
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_Init Error: %s\n", SDL_GetError());
        return;
    }

    loadFont(BASE_FONT_SIZE);

    // m_blockTexture = IMG_LoadTexture(m_renderer, RESOURCES_PATH "textures/block.png");
    // if (!m_blockTexture) {
    //	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create block texture: %s\n",
    //		SDL_GetError());
    // }
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_DestroyTexture(m_blockTexture);
    TTF_CloseFont(m_font);
    SDL_Quit();
}

void Renderer::clear() {
    if (!SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetRenderDrawColor Error: %s\n", SDL_GetError());
    }
    if (!SDL_RenderClear(m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderClear Error: %s\n", SDL_GetError());
    }
}

void Renderer::present() {
    if (!SDL_RenderPresent(m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_RenderPresent Error: %s\n", SDL_GetError());
    }
}

// void Renderer::drawBlock(int gridX, int gridY, const SDL_Color& color) {
//     if (gridY < 0) return;
//
//     SDL_FRect block = { gridX, gridY, BLOCK_SIZE, BLOCK_SIZE };
//     if (!SDL_RenderTexture(m_renderer, m_blockTexture, NULL, &block)) {
//         SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not draw block texture: %s\n",
//         SDL_GetError());
//     }
// }

// Draws block based on X and Y (compared to grid). blockSize is handled here

void Renderer::drawText(const std::string &text, int gridX, int gridY) {
    const SDL_Color color = {255, 255, 255, 255}; // White text color

    // Create a surface from the text
    SDL_Surface *textSurface = TTF_RenderText_Solid(m_font, text.c_str(), 0, color);
    if (textSurface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_RenderText_Solid Error: %s\n", SDL_GetError());
        TTF_CloseFont(m_font);
        return;
    }

    // Create texture from surface
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    SDL_DestroySurface(textSurface); // No longer needed

    // Convert grid position to pixel position
    const int pixelX = m_xOffset + gridX * m_blockSize;
    const int pixelY = m_yOffset + gridY * m_blockSize;

    // Render the text
    const SDL_FRect renderQuad = {static_cast<float>(pixelX), static_cast<float>(pixelY),
                                  static_cast<float>(textSurface->w),
                                  static_cast<float>(textSurface->h)};
    SDL_RenderTexture(m_renderer, textTexture, nullptr, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
}

void Renderer::update() {
    int width{0};
    int height{0};
    SDL_GetWindowSize(m_window, &width, &height);
    if (m_res.width != width || m_res.height != height) {
        setResolution({width, height});
    }
}

ScreenPosition Renderer::getResolution() {
    int width{0};
    int height{0};
    SDL_GetWindowSize(m_window, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

void Renderer::drawTextAtPixel(const std::string &text, ScreenPosition pos, bool centered) {
    const SDL_Color color = {255, 255, 255, 255}; // White text color

    // Create a surface from the text
    SDL_Surface *textSurface = TTF_RenderText_Solid(m_font, text.c_str(), 0, color);
    if (textSurface == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_RenderText_Solid Error: %s\n", SDL_GetError());
        return;
    }

    // Create texture from surface
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    if (textTexture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateTextureFromSurface Error: %s\n",
                     SDL_GetError());
        SDL_DestroySurface(textSurface); // Surface no longer needed
        return;
    }
    // Get the text width and height
    const int textWidth = textSurface->w;
    const int textHeight = textSurface->h;
    SDL_DestroySurface(textSurface); // Surface no longer needed

    // Render the text
    // TODO: YOU DONT HAVE TO LOAD FONT EACH TIME! YOU CAN JUST CHANGE THE SIZE
    // OF renderQuad W and H!
    const float textPosX = centered ? pos.x - static_cast<float>(textWidth) / 2.0F : pos.x;
    const float textPosY = centered ? pos.y - static_cast<float>(textHeight) / 2.0F : pos.y;
    const SDL_FRect renderQuad = {textPosX, textPosY, static_cast<float>(textWidth),
                                  static_cast<float>(textHeight)};

    SDL_RenderTexture(m_renderer, textTexture, nullptr, &renderQuad);

    // Clean up
    SDL_DestroyTexture(textTexture);
}

void Renderer::drawBlockAtPixel(int pixelX, int pixelY, const SDL_Color &color) {
    drawBlockAtPixel(pixelX, pixelY, color, m_blockSize);
}

void Renderer::drawBlockAtPixel(int pixelX, int pixelY, const SDL_Color &color, int blockSize) {
    const SDL_FRect block = {static_cast<float>(pixelX), static_cast<float>(pixelY),
                             static_cast<float>(blockSize), static_cast<float>(blockSize)};

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &block);
}

void Renderer::drawBlock(int x, int y, const SDL_Color &color) {
    if (y < 0) {
        return;
    }

    const SDL_FRect block = {static_cast<float>(m_xOffset + x * m_blockSize),
                       static_cast<float>(m_yOffset + y * m_blockSize),
                       static_cast<float>(m_blockSize), static_cast<float>(m_blockSize)};

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &block);
}

void Renderer::setDrawColor(const SDL_Color &color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderLine(m_renderer, static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2));
}

void Renderer::drawGrid(const ScreenResolution& res) {
    for (int x = 0; x <= res.width; ++x) {
        const int screenX = m_xOffset + x * m_blockSize;
        drawLine(screenX, m_yOffset, screenX, m_yOffset + res.height * m_blockSize);
    }

    for (int y = 0; y <= res.height; ++y) {
        const int screenY = m_yOffset + y * m_blockSize;
        drawLine(m_xOffset, screenY, m_xOffset + res.width * m_blockSize, screenY);
    }
}

void Renderer::setGridSize(const ScreenResolution& res) {
    m_blockSize = std::min(m_res.width / res.width, m_res.height / res.height);

    // Calculate offsets to center the grid
    const int gridWidthPx = res.width * m_blockSize;
    const int gridHeightPx = res.height * m_blockSize;

    m_xOffset = (m_res.width - gridWidthPx) / 2;
    m_yOffset = (m_res.height - gridHeightPx) / 2;
}

void Renderer::setResolution(const ScreenResolution &res) {
		m_res = res;

    // Update SDL window size
    SDL_SetWindowSize(m_window, m_res.width, m_res.height);

    // Recalculate grid and block size
    setGridSize({GRID_WIDTH, GRID_HEIGHT});
    float scaledFontSize = calculateFontSize(BASE_FONT_SIZE);
    loadFont(scaledFontSize);
}

float Renderer::calculateHudX(int baseX) const {
    float scaleX = static_cast<float>(m_res.width) / BASE_WIDTH;
    return scaleX * static_cast<float>(baseX);
}

float Renderer::calculateHudY(int baseY) const {
    float scaleY = static_cast<float>(m_res.height) / BASE_HEIGHT;
    return scaleY * static_cast<float>(baseY);
}

int Renderer::calculateHudBlockSize() const {
    float scaleX = static_cast<float>(m_res.width) / BASE_WIDTH;
    float scaleY = static_cast<float>(m_res.height) / BASE_HEIGHT;
    return static_cast<int>(std::max(scaleX, scaleY)) * m_blockSize;
}

float Renderer::calculateFontSize(float baseFontSize) const {
    float scaleX = static_cast<float>(m_res.width) / BASE_WIDTH;
    float scaleY = static_cast<float>(m_res.height) / BASE_HEIGHT;
    return std::min(scaleX, scaleY) * baseFontSize;
}

void Renderer::loadFont(float fontSize) {
    if (m_font != nullptr) {
        TTF_CloseFont(m_font);
    }

    m_font = TTF_OpenFont(RESOURCES_PATH "/fonts/Kgsecondchancessketch.ttf", fontSize);
    if (m_font == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont Error: %s\n", SDL_GetError());
    }
}
