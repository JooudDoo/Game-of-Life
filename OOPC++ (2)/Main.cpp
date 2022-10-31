#include "Game.h"

#include <iostream>

int main() {
	LifeGame game = LifeGame(50, 25);

	game.placeCell(0, 1);
	game.placeCell(1, 2);
	game.placeCell(2, 0);
	game.placeCell(2, 1);
	game.placeCell(2, 2);
	game.runGame(10);
	
}