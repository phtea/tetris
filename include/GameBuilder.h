#pragma once
#include "Game.h"

class GameBuilder {
  public:
    // Constructor with required parameters (screen size, for example)
    GameBuilder(int screenWidth, int screenHeight)
        : m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_timeToFall(1000),
          m_lockDelayTime(500), m_DAS(167), m_ARR(33), m_SDF(100), m_nextMinosSize(1) {}

    // Chainable setters for optional configuration
    GameBuilder &setScreenWidth(int width) {
        m_screenWidth = width;
        return *this;
    }

    GameBuilder &setScreenHeight(int height) {
        m_screenHeight = height;
        return *this;
    }

    GameBuilder &setTimeToFall(Uint32 timeToFall) {
        m_timeToFall = timeToFall;
        return *this;
    }

    GameBuilder &setLockDelayTime(Uint32 lockDelayTime) {
        m_lockDelayTime = lockDelayTime;
        return *this;
    }

    GameBuilder &setDAS(Uint32 das) {
        m_DAS = das;
        return *this;
    }

    GameBuilder &setARR(Uint32 arr) {
        m_ARR = arr;
        return *this;
    }

    GameBuilder &setSDF(Uint32 sdf) {
        m_SDF = sdf;
        return *this;
    }

    GameBuilder &setNextTetrominosSize(int size) {
        m_nextMinosSize = size;
        return *this;
    }

    // Build and return a Game instance.
    // Note: If Game's constructor doesn't take these parameters,
    // you might have to add additional setters in Game or make GameBuilder a friend.
    Game build() {
        return Game(m_screenWidth, m_screenHeight, m_timeToFall, m_lockDelayTime, m_DAS, m_ARR,
                    m_SDF, m_nextMinosSize);
    }

  private:
    int m_screenWidth;
    int m_screenHeight;
    Uint32 m_timeToFall;
    Uint32 m_lockDelayTime;
    Uint32 m_DAS;
    Uint32 m_ARR;
    Uint32 m_SDF;
    int m_nextMinosSize;
};
