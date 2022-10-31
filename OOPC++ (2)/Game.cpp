#include "Game.h"

LifeGame::LifeGame() : canvasHeight(HEIGHTDEFAULT), canvasWidth(WIDTHDEFAULT) {
	logic = GameLogic(canvasWidth, canvasHeight);
	renderer = GameRenderer(canvasWidth, canvasHeight);
}

LifeGame::LifeGame(const SHORT& cWidth, const SHORT& cHeight) : canvasHeight(cHeight), canvasWidth(cWidth) {
	logic = GameLogic(canvasWidth, canvasHeight);
	renderer = GameRenderer(canvasWidth, canvasHeight);
}


void LifeGame::runGame(const SHORT& FPS) {
	DWORD delay = 1000 / FPS;
	PlAction action = {none, NULL};
	bool simulate = true;
	while (action.code != quit) { 
		renderFrame(simulate);
		action = renderer.checkPlayer(!simulate);
		if (action.code == pause) { simulate = !simulate; }
		if (action.code == mouseClick) {
			logic.placeCell(action.mouseClick.Y, action.mouseClick.X);
		}
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