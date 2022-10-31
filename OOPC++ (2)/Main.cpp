#include "Game.h"

#include <iostream>

int main() {
	LifeGame game = LifeGame(150, 50);
	game.runGame("Test universe", 10);
}