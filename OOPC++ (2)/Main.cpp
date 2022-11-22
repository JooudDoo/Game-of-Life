#include "Game.h"

#include <iostream>

int main() {
	LifeGame a = createGameFFile("Universes/DefaultUniverse.txt");
	a.runGame();
}