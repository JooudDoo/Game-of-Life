#include "Game.h"

LifeGame::LifeGame() : canvasHeight(HEIGHTDEFAULT), canvasWidth(WIDTHDEFAULT) {
	logic = GameLogic(canvasWidth, canvasHeight);
	renderer = GameRenderer(canvasWidth, canvasHeight);
}

void LifeGame::runGame(const SHORT& FPS) {
	DWORD delay = 1000 / FPS;
	ConsoleCodes state = None;
	bool simulate = true;
	while (state != quit) {
		renderFrame(simulate);
		state = renderer.checkPlayer();
		if (state == pause) { simulate = !simulate; }
		Sleep(delay);
	}
	while (true) {
		Sleep(delay * FPS * 100);
	};
}

void LifeGame::renderFrame(const bool& isSimulate) {
	if(isSimulate) logic.simulate();
	renderer.renderFrame(logic.getField());
}

bool LifeGame::placeCell(const SHORT& x, const SHORT& y) {
	return logic.placeCell(x, y);
}

bool LifeGame::clearCell(const SHORT& x, const SHORT& y) {
	return logic.clearCell(x, y);
}