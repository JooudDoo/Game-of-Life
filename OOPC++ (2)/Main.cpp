#include "Game.h"

#include <iostream>

int main() {
	LifeGame game = LifeGame(100, 50);
	game.runGame("Test universe", 20);
}