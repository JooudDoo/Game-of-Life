#include "Game.h"
#include "Parser.h"

#include <iostream>

int main() {
	FileParser a;
	GameData b = a.parseUniverseFF("TestUniverse.txt");
    std::cerr << b.name;
	//LifeGame game = LifeGame(100, 50);
	//game.runGame("Test universe", 20);
}