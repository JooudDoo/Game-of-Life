#include "Game.h"

inline long long time_to_msec(const sys_time_t& t) {
	return t.time * 1000LL + t.millitm;
}

LifeGame::LifeGame() : canvasHeight(HEIGHTDEFAULT), canvasWidth(WIDTHDEFAULT), logic(WIDTHDEFAULT, HEIGHTDEFAULT), renderer(WIDTHDEFAULT, HEIGHTDEFAULT), targetTPS(0) {}

LifeGame::LifeGame(const SHORT& cWidth, const SHORT& cHeight) : canvasHeight(cHeight), canvasWidth(cWidth), logic(cWidth, cHeight), renderer(cWidth, cHeight), targetTPS(0) {}

void LifeGame::runGame(const std::string& universeName, const SHORT& targetTPS) {
	renderer.setTargetTPS(targetTPS);
	renderer.setUniverseName(universeName);
	LifeGame::targetTPS = targetTPS;
	SHORT currentTPS = -1;
	DWORD delay = 1000 / targetTPS;
	PlAction action = {none, NULL};
	while (action.code != quit) {
		_ftime_s(&T_st);
		renderFrame(true, currentTPS, action.code);
		action = renderer.checkPlayer(false);
		if (action.code == pause) pausedGame();
		Sleep(delay);
		_ftime_s(&T_end);
		currentTPS = 1000 / (time_to_msec(T_end) - time_to_msec(T_st)) + 1;
	}
}

void LifeGame::pausedGame() {
	PlAction action = { pause, NULL };
	DWORD delay = 1000 / targetTPS;
	while (true) {
		renderFrame(false, 0, action.code);
		action = renderer.checkPlayer(true);
		if (action.code == pause)
			break;
		else if (action.code == mouseClick) {
			logic.switchCell(action.mouseClick.Y, action.mouseClick.X);
		}
		else if (action.code == none)
			action.code = pause;
		Sleep(delay);
	}
}

void LifeGame::renderFrame(const bool& isSimulate, const SHORT& currentTPS, const ConsoleCodes& state) {
	if(isSimulate) logic.simulate();
	renderer.renderGUI(currentTPS, state);
	renderer.renderFrame(logic.getField());
}

bool LifeGame::placeCell(const SHORT& x, const SHORT& y) {
	return logic.placeCell(x, y);
}

bool LifeGame::clearCell(const SHORT& x, const SHORT& y) {
	return logic.clearCell(x, y);
}