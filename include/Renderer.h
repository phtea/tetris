#pragma once

#include "CustomTypes.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

struct ScreenPosition {
    float x, y;
};

class Renderer {
  public:
    Renderer(const std::string& title, const ScreenResolution &res);
    ~Renderer();
    void clear();
    void present();
    void setDrawColor(const SDL_Color &color);
    void setGridSize(const ScreenResolution& res);

    void setResolution(const ScreenResolution &res);

    void drawLine(int x1, int y1, int x2, int y2);
    void drawGrid(const ScreenResolution& res);
    void drawBlock(int x, int y, const SDL_Color &color);
    void drawText(const std::string &text, int x, int y);

    void update();

    ScreenPosition getResolution();

    // New methods for drawing at pixel positions
    void drawBlockAtPixel(int pixelX, int pixelY, const SDL_Color &color);
    void drawBlockAtPixel(int pixelX, int pixelY, const SDL_Color &color, int blockSize);
    void drawTextAtPixel(const std::string &text, ScreenPosition pos, bool centered);

    // New methods for position and size calculation
    [[nodiscard]] float calculateHudX(int baseX) const;
    [[nodiscard]] float calculateHudY(int baseY) const;
    [[nodiscard]] int calculateHudBlockSize() const;

    [[nodiscard]] float calculateFontSize(float baseFontSize) const;

    [[nodiscard]] int getBlockSize() const { return m_blockSize; }

  private:
    void loadFont(float fontSize);

    SDL_Window *m_window{nullptr};
    SDL_Renderer *m_renderer{nullptr};
    SDL_Texture *m_blockTexture{nullptr};
    TTF_Font *m_font{nullptr};

		ScreenResolution m_res{0, 0};
    int m_xOffset{0};
    int m_yOffset{0};
    int m_blockSize{0};

    static constexpr int BASE_WIDTH = 1920;
    static constexpr int BASE_HEIGHT = 1080;
    static constexpr int BASE_FONT_SIZE = 24;
};
