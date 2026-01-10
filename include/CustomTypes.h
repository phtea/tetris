#pragma once
#include <cstdint>

enum class MinoType : uint8_t { I, J, L, O, S, T, Z, NONE };
enum class Direction : uint8_t { LEFT, RIGHT, DOWN };

struct Position {
	int x;
	int y;
};

struct ScreenResolution {
	int width;
	int height;
};
