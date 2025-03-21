#include <iostream>

#include "game.h"
#include "GameBuilder.h"

int main() {
	GameBuilder builder(800, 600);

	builder.setTimeToFall(900)
		.setLockDelayTime(600)
		.setDAS(150)
		.setARR(30)
		.setSDF(120)
		.setNextTetrominosSize(1);

	Game game = builder.build();

	game.run();

	return 0;
}