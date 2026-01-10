#pragma once

#include <SDL3/SDL.h>

#include "CustomTypes.h"
#include "Renderer.h"
#include <array>
#include <unordered_map>
#include <vector>

using grid_t = std::vector<std::vector<int>>;
using blocks_t = std::array<std::array<int, 2>, 4>;

class Mino {
  public:
    Mino(MinoType type);

    [[nodiscard]] bool canMove(Direction dir, const grid_t &grid) const;
    void move(Direction dir);
    void hardDrop(const grid_t &grid);
    bool rotate(int rotations, const grid_t &grid);
    void setOriginalRotationState();

    void setPosition(Position pos);
    void setStartPosition();

    // Gets fixed blocks for the tetromino
    [[nodiscard]] blocks_t getBlocks() const { return m_blocks; }
    // Gets the blocks relative to grid
    [[nodiscard]] blocks_t getRelativeBlocks() const;

    [[nodiscard]] std::array<int, 2> getPosition() const { return {m_X, m_Y}; }
    [[nodiscard]] SDL_Color getColor() const { return m_color; }
    [[nodiscard]] MinoType getType() const { return m_type; }

    void draw(Renderer &renderer) const;

  private:
    void setColor();
    [[nodiscard]] bool collidesWithGrid(const blocks_t &testBlocks, const grid_t &grid) const;

    // rotation logic
    void setShape(int newRotation);
    constexpr static std::array<std::array<int, 2>, 12>
    getWallKicks(int from, int to, MinoType type);
    constexpr static std::array<std::array<int, 2>, 12> get180WallKicks(int newRotation,
                                                                        MinoType type);
    blocks_t applyRotation(int newRotation);

    MinoType m_type;
    SDL_Color m_color;
    blocks_t m_blocks;
    int m_rotationState{0};
    static const std::unordered_map<MinoType, std::array<std::array<std::array<int, 2>, 4>, 4>>
        shapes;

    int m_X, m_Y; // grid-based positions! (not pixel positions)
};

constexpr std::array<std::array<std::array<int, 2>, 5>, 8> iKicks = {{
    {{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}}, // 0->1
    {{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}}, // 1->0
    {{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}}, // 1->2
    {{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}}, // 2->1
    {{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}}, // 2->3
    {{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}}, // 3->2
    {{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}}, // 3->0
    {{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}}, // 0->3
}};

constexpr std::array<std::array<std::array<int, 2>, 5>, 8> otherKicks = {{
    {{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}}, // 0->1
    {{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},     // 1->0
    {{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},     // 1->2
    {{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}}, // 2->1
    {{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},    // 2->3
    {{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},  // 3->2
    {{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},  // 3->0
    {{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},    // 0->3
}};

// 180 kick tables work as expected!
constexpr std::array<std::array<std::array<int, 2>, 12>, 4> otherBlock180KickTable = {{
    {{{{0, 0}},
      {{-1, 0}},
      {{-2, 0}},
      {{-1, -1}},
      {{-2, -1}},
      {{1, 0}},
      {{2, 0}},
      {{1, -1}},
      {{2, -1}},
      {{0, 1}},
      {{-3, 0}},
      {{3, 0}}}}, // 2>>0─┐
    {{{{0, 0}},
      {{0, 1}},
      {{0, 2}},
      {{1, 1}},
      {{1, 2}},
      {{0, -1}},
      {{0, -2}},
      {{1, -1}},
      {{1, -2}},
      {{-1, 0}},
      {{0, 3}},
      {{0, -3}}}}, // 3>>1─┼┐
    {{{{0, 0}},
      {{1, 0}},
      {{2, 0}},
      {{1, 1}},
      {{2, 1}},
      {{-1, 0}},
      {{-2, 0}},
      {{-1, 1}},
      {{-2, 1}},
      {{0, -1}},
      {{3, 0}},
      {{-3, 0}}}}, // 0>>2─┘│
    {{{{0, 0}},
      {{0, 1}},
      {{0, 2}},
      {{-1, 1}},
      {{-1, 2}},
      {{0, -1}},
      {{0, -2}},
      {{-1, -1}},
      {{-1, -2}},
      {{1, 0}},
      {{0, 3}},
      {{0, -3}}}}, // 1>>3──┘
}};

constexpr std::array<std::array<std::array<int, 2>, 12>, 4> iBlock180KickTable = {{
    {{{{0, 0}},
      {{1, 0}},
      {{2, 0}},
      {{-1, 0}},
      {{-2, 0}},
      {{0, -1}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}}}}, // 2>>0─┐
    {{{{0, 0}},
      {{0, 1}},
      {{0, 2}},
      {{0, -1}},
      {{0, -2}},
      {{1, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}}}}, // 3>>1─┼┐
    {{{{0, 0}},
      {{-1, 0}},
      {{-2, 0}},
      {{1, 0}},
      {{2, 0}},
      {{0, 1}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}}}}, // 0>>2─┘│
    {{{{0, 0}},
      {{0, 1}},
      {{0, 2}},
      {{0, -1}},
      {{0, -2}},
      {{-1, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}},
      {{0, 0}}}}, // 1>>3──┘
}};
