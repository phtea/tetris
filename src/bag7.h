#pragma once

#include <array>
#include <iostream>
#include <random>

#include "custom_types.h"

// The Bag7 class wraps around a std::array of 7 TetrominoType elements
class Bag7 {
public:
	using bag7_t = std::array<TetrominoType, 7>;

	Bag7();

	void shuffle();
	void print() const;
	TetrominoType pickNext();

private:
	bag7_t m_bag;
	size_t m_currentIndex;
	static std::random_device rd;
	static std::default_random_engine engine;
};