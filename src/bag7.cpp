#include "bag7.h"

#include <algorithm>

// Initialize the static random engine once
std::random_device Bag7::rd;
std::default_random_engine Bag7::engine(rd());

// Constructor initializes the m_bag with all 7 Tetromino types and shuffle them
Bag7::Bag7() : m_currentIndex(0) {
  m_bag = {TetrominoType::I, TetrominoType::O, TetrominoType::T,
           TetrominoType::S, TetrominoType::Z, TetrominoType::J,
           TetrominoType::L};
  shuffle();
}

// Shuffle the contents of the m_bag
void Bag7::shuffle() { std::shuffle(m_bag.begin(), m_bag.end(), engine); }

// Print the contents of the m_bag for visualization/debugging
void Bag7::print() const {
  for (const auto& tetromino : m_bag) {
    std::cout << static_cast<int>(tetromino) << " ";
  }
  std::cout << std::endl;
}

TetrominoType Bag7::pickNext() {
  if (m_currentIndex >= m_bag.size()) {
    shuffle();
    m_currentIndex = 0;
  }
  return m_bag[m_currentIndex++];
}
