#pragma once

#include "CustomTypes.h"
#include "Game.h"

class GameBuilder {
  public:
    GameBuilder(const ScreenResolution &res) : m_res(res) {}

    // Chainable setters for optional configuration
    GameBuilder &setScreenWidth(int width) {
        m_res.width = width;
        return *this;
    }

    GameBuilder &setScreenHeight(int height) {
        m_res.height = height;
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
    [[nodiscard]] Game build() const {
        const GameConfig cfg{.timings = {.timeToFall = m_timeToFall,
                                   .lockDelayTime = m_lockDelayTime,
                                   .das = m_DAS,
                                   .arr = m_ARR,
                                   .sdf = m_SDF},
                       .nextMinosSize = m_nextMinosSize};
        return {m_res, cfg};
    }

  private:
    ScreenResolution m_res{0, 0};
    Uint32 m_timeToFall{1000};
    Uint32 m_lockDelayTime{500};
    Uint32 m_DAS{167};
    Uint32 m_ARR{33};
    Uint32 m_SDF{100};
    int m_nextMinosSize{1};
};
