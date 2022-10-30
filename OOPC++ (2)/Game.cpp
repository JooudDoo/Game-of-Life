#include "Game.h"

LifeGame::LifeGame() : canvasHeight(HEIGHTDEFAULT), canvasWidth(WIDTHDEFAULT) {
	logic = GameLogic(canvasWidth, canvasHeight);
	renderer = GameRenderer(canvasWidth, canvasHeight);
}

void LifeGame::runGame(const SHORT& FPS) {
	DWORD delay = 1000 / FPS;
	bool enabled = true;
	while (enabled) {
		renderFrame();
		enabled = renderer.checkPlayer();
		Sleep(delay);
	}
}

void LifeGame::renderFrame() {
	renderer.renderFrame(logic.getField());
	logic.simulate();
}

bool LifeGame::placeCell(const SHORT& x, const SHORT& y) {
	return logic.placeCell(x, y);
}

bool LifeGame::clearCell(const SHORT& x, const SHORT& y) {
	return logic.clearCell(x, y);
}