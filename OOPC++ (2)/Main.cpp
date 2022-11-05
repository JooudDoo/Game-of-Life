#include "Game.h"

#include <iostream>

int main() {
	LifeGame a = createGameFFile("TestUniverse.txt");
	a.runGame(244);
}