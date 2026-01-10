#include "Game.h"
#include "GameBuilder.h"

int main() {
    GameBuilder builder({.width = 800, .height = 600});

    Game game = builder.setTimeToFall(900)
                    .setLockDelayTime(600)
                    .setDAS(150)
                    .setARR(30)
                    .setSDF(120)
                    .setNextTetrominosSize(2)
                    .build();

    game.run();

    return 0;
}
