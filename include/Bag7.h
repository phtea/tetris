#pragma once

#include <array>
#include <iostream>
#include <random>

#include "CustomTypes.h"

// The Bag7 class wraps around a std::array of 7 MinoType elements
class Bag7 {
  public:
    using bag7_t = std::array<MinoType, 7>;

    Bag7();

    void shuffle();
    void print() const;
    MinoType pickNext();

  private:
    bag7_t m_bag;
    size_t m_currentIndex;
    static std::random_device rd;
    static std::default_random_engine engine;
};
