#include "Game.h"

LifeGame createGameFFile(const std::string& filePath) {
	FileParser parser;
	GameData preferences = parser.parseUniverseFF(filePath);
	LifeGame game(preferences.gameField.width, preferences.gameField.height);
	game.setUniverseName(preferences.name);
	game.setFieldBlank(preferences.gameField);
	game.setGameRule(preferences.rule);
	game.isPaused = true;
	return game;
}

void LifeGame::setUniverseName(const std::string& name) {
	UniverseName = name;
}

void LifeGame::setGameRule(const GameRule& r) {
	logic.setRule(r);
}

bool LifeGame::setFieldBlank(const Field& fl) {
	return logic.setBlankField(fl);
}

inline long long time_to_msec(const sys_time_t& t) {
	return t.time * 1000LL + t.millitm;
}

LifeGame::LifeGame() : pauseTargetTPS(60), canvasHeight(HEIGHTDEFAULT), canvasWidth(WIDTHDEFAULT), logic(WIDTHDEFAULT, HEIGHTDEFAULT), renderer(WIDTHDEFAULT, HEIGHTDEFAULT), targetTPS(0) {}

LifeGame::LifeGame(const SHORT& cWidth, const SHORT& cHeight) : pauseTargetTPS(60), canvasHeight(cHeight), canvasWidth(cWidth), logic(cWidth, cHeight), renderer(cWidth, cHeight), targetTPS(0) {}

void LifeGame::runGame(const SHORT& targetTPS) {
	runGame(UniverseName, targetTPS);
}

void LifeGame::runGame(const std::string& universeName, const SHORT& targetTPS) {
	renderer.setTargetTPS(targetTPS);
	renderer.setUniverseName(universeName);
	LifeGame::UniverseName = universeName;
	LifeGame::targetTPS = targetTPS;
	SHORT currentTPS = -1;
	DWORD delay = 1000 / targetTPS;
	PlAction action = {none, NULL};
	SHORT frameCounter = 0;
	if (isPaused) pausedGame();
	while (action.code != quit) {
		renderFrame(true, currentTPS, action.code);
		action = renderer.checkPlayer(false);
		if (action.code == pause) pausedGame();
		else if (action.code == reset) resetField();
		Sleep(delay);
		frameCounter += 1;
		if (frameCounter == 15) {
			_ftime_s(&T_end);
			currentTPS = 1000 / ((time_to_msec(T_end) - time_to_msec(T_st))/ frameCounter);
			_ftime_s(&T_st);
			frameCounter = 0;
		}
	}
}

void LifeGame::pausedGame() {
	isPaused = true;
	PlAction action = { pause, NULL };
	DWORD delay = 1000 / pauseTargetTPS;
	while (true) {
		renderFrame(false, 0, action.code);
		action = renderer.checkPlayer(true);
		if (action.code == pause)
			break;
		else if (action.code == mouseClick) {
			logic.switchCell(action.mouseClick.Y, action.mouseClick.X);
		}
		else if (action.code == reset) resetField();
		else if (action.code == saveFieldToReset) setFieldBlank(logic.getField());
		else if (action.code == none)
			action.code = pause;
		Sleep(delay);
	}
	isPaused = false;
}

void LifeGame::resetField() {
	logic.clearField();
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