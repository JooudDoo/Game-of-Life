#include "GameRenderer.h"
#include "GameLogic.h"
#include "Game.h"

#include <iostream>

int main() {
	GameRenderer mainScreen = GameRenderer();
	GameLogic logic = GameLogic(15, 15);
	logic.placeCell(0, 1);
	logic.placeCell(1, 2);
	logic.placeCell(2, 0);
	logic.placeCell(2, 1);
	logic.placeCell(2, 2);
	mainScreen.renderFrame(logic.getField());
	std::cin.get();
	while (true) {
		logic.simulate();
		mainScreen.renderFrame(logic.getField());
		Sleep(30);
	}
}