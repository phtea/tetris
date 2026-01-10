#pragma once

#include "CustomTypes.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

struct ScreenPosition {
    double x, y;
};

class Renderer {
  public:
    Renderer(const std::string &title, const ScreenResolution &res);
    ~Renderer();
    void clear();
    void present();
    void setDrawColor(const SDL_Color &color);
    void setGridSize(const ScreenResolution &res);

    void setResolution(const ScreenResolution &res);

    void drawLine(float x1, float y1, float x2, float y2);
    void drawGrid(const ScreenResolution &res);
    void drawBlock(double x, double y, const SDL_Color &color);
    void drawText(const std::string &text, const ScreenPosition &pos);

    void update();

    ScreenPosition getResolution();

    // New methods for drawing at pixel positions
    void drawBlockAtPixel(float pixelX, float pixelY, const SDL_Color &color);
    void drawBlockAtPixel(float pixelX, float pixelY, const SDL_Color &color, float blockSize);
    void drawTextAtPixel(const std::string &text, ScreenPosition pos, bool centered);

    // New methods for position and size calculation
    [[nodiscard]] float calculateHudX(double baseX) const;
    [[nodiscard]] float calculateHudY(double baseY) const;
    [[nodiscard]] float calculateHudBlockSize() const;

    [[nodiscard]] float calculateFontSize(float baseFontSize) const;

    [[nodiscard]] float getBlockSize() const { return m_blockSize; }

  private:
    void loadFont(float fontSize);

    SDL_Window *m_window{nullptr};
    SDL_Renderer *m_renderer{nullptr};
    SDL_Texture *m_blockTexture{nullptr};
    TTF_Font *m_font{nullptr};

    ScreenResolution m_res{0, 0};
    int m_xOffset{0};
    int m_yOffset{0};
    float m_blockSize{0};

    static constexpr int BASE_WIDTH = 1920;
    static constexpr int BASE_HEIGHT = 1080;
    static constexpr int BASE_FONT_SIZE = 24;
};
