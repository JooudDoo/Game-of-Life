#include "Game.h"
#include <format>
#include <sstream>

#define TimeIt(st,code,  ed) _ftime_s(&st); \
							code; \
							_ftime_s(&ed);

#define NONEG(x) (x>=0 && x<= 1000?x:0)
#define NORMALIZE(x,y) (x%y)

inline long long time_to_msec(const sys_time_t& t) {
	return t.time * 1000LL + t.millitm;
}

LifeGame createGameFFile(const std::string& filePath, LifeGame* prevGame) {
	if (prevGame) {
		prevGame->~LifeGame();
	}
	UniverseImporter parser;
	GameData preferences = parser.parseUniverseFF(filePath);
	LifeGame game(preferences.gameField.width, preferences.gameField.height);
	game.setUniverseName(preferences.name);
	game.setTargetFPS(244);
	game.setFieldBlank(preferences.gameField);
	game.setGameRule(preferences.rule);
	game.gameStatus = gamePause;
	return game;
}

void LifeGame::setUniverseName(const std::string& name) {
	universeName = name;
	con.gRen.setUniverseName(universeName);
}

void LifeGame::setTargetFPS(const SHORT& fps) {
	targetFPS = fps;
	TargetDelay = 1000 / (fps+1*!fps);
	con.gRen.setTargetFPS(fps);
}

void LifeGame::setGameRule(const GameRule& r) {
	logic.setRule(r);
}

bool LifeGame::setFieldBlank(const Field& fl) {
	return logic.setBlankField(fl);
}

LifeGame::LifeGame() : gameStatus(gameNone), canvasHeight(HEIGHTDEFAULT), canvasWidth(WIDTHDEFAULT), logic(WIDTHDEFAULT, HEIGHTDEFAULT), targetFPS(0) {}

LifeGame::LifeGame(const SHORT& cWidth, const SHORT& cHeight) : con(cWidth, cHeight), gameStatus(gameNone), canvasHeight(cHeight), canvasWidth(cWidth), logic(cWidth, cHeight), targetFPS(0) {}

void LifeGame::runGame() { //REFACTOR THIS 
	setTargetFPS(targetFPS);
	setUniverseName(universeName);

	std::vector<PlAction> actions;
	sys_time_t T_st, T_end;
	bool quit = false;

	if (gameStatus == gamePause) pausedGame();
	while (!quit) {
		gameStatus = gameWork;
		TimeIt(T_st,
			renderFrame(true, none);
			actions = con.getPlayerActions(false);
			for (auto action : actions) {
				if (action.code == pause) { pausedGame(); break; }
				else if (action.code == consoleMode) { consoledGame(); break; }
				else if (action.code == reset) setBlankField();
			}, 
			T_end);
		DWORD milsBetFrame = (time_to_msec(T_end) - time_to_msec(T_st));
		DWORD delay = TargetDelay - milsBetFrame + !milsBetFrame;
		Sleep(delay - delay * (delay > 1000));
	}
}

void LifeGame::pausedGame() {
	gameStatus = gamePause;
	std::vector<PlAction> actions;
	bool CTRLSTATE = false;
	bool CTRLPLACE = true;
	bool quit = false;

	renderFrame(false, pause);
	while (!quit) {
		actions = con.getPlayerActions(true);
		renderFrame(false, pause);
		for (auto action : actions) {
			if (CTRLSTATE == true && action.ctrlState == false)
				CTRLSTATE = false;
			if (action.code == pause) {
				quit = true;
				break;
			}
			else if (action.code == mouseCtrlMove) {
				if (CTRLSTATE == false) {
					CTRLSTATE = true;
					if (logic.isEmptyCell(action.mouseClick.Y, action.mouseClick.X))
						CTRLPLACE = true;
					else
						CTRLPLACE = false;
				}
				if (CTRLPLACE)
					logic.placeCell(action.mouseClick.Y, action.mouseClick.X);
				else
					logic.clearCell(action.mouseClick.Y, action.mouseClick.X);

			}
			else if (action.code == mouseClick) logic.switchCell(action.mouseClick.Y, action.mouseClick.X);
			else if (action.code == reset) setBlankField();
			else if (action.code == saveFieldToReset) setFieldBlank(logic.getField());
			else if (action.code == consoleMode) { consoledGame(); break; }
			else if (action.code == none) action.code = pause;
		}
	}
}

void LifeGame::consoledGame() {
	gameStatus = gameConsole;
	renderFrame(false, consoleMode);
	con.switchToConsoleMode();
	std::pair<ConsoleWriteCode, std::string> consoleCommand;
	while (true) {
		consoleCommand = con.gInp.proccesInput();
		con.consoleWriteProcessed();
		if (consoleCommand.first == offConMode) break;
		else if (consoleCommand.first == ConsoleWriteCode::NAC) {
			con.writeLineWithPrefix("This command isn't recognized", consoleWriteWarningPrefix);
		}
		else if (consoleCommand.first == ConsoleWriteCode::tickSkip) {
			int tickToPass = std::stoi(nullConsoleParametr);
			if (isNumber(consoleCommand.second)) tickToPass = std::stoi(consoleCommand.second);
			if (tickToPass == std::stoi(nullConsoleParametr)) {
				con.writeLineWithPrefix("Invalid argument", consoleWriteWarningPrefix);
			}
			else {
				logic.simulateTicks(tickToPass);
				con.writeLineWithPrefix(std::format("Successfully simulated {} ticks", tickToPass), consoleWriteAnnotationPrefix);
			}
			renderFrame(false, consoleMode);
			con.switchToConsoleMode();
		}
		else if (consoleCommand.first == ConsoleWriteCode::cleanField) {
			clearField();
			renderFrame(false, consoleMode);
			con.switchToConsoleMode();
			con.writeLineWithPrefix("Field has been cleared successfully", consoleWriteAnnotationPrefix);
		}
		else if (consoleCommand.first == ConsoleWriteCode::clearConsole) {
			con.clearWriteConsole();
		}
		else if (consoleCommand.first == ConsoleWriteCode::switchUniverseName) {
			setUniverseName(consoleCommand.second);
			con.writeLineWithPrefix(std::format("New name set successfully: \"{}\"", consoleCommand.second), consoleWriteAnnotationPrefix);
			renderFrame(false, consoleMode);
			con.switchToConsoleMode();
		}
		else if (consoleCommand.first == ConsoleWriteCode::switchTargetFPS) {
			int targetFPS = std::stoi(nullConsoleParametr);
			if (isNumber(consoleCommand.second)) targetFPS = std::stoi(consoleCommand.second);
			if (targetFPS == std::stoi(nullConsoleParametr) || targetFPS < 0) {
				con.writeLineWithPrefix("Invalid argument", consoleWriteWarningPrefix);
			}
			else {
				setTargetFPS(targetFPS);
				renderFrame(false, consoleMode);
				con.switchToConsoleMode();
				con.writeLineWithPrefix(std::format("Successfully set new target FPS: {}", targetFPS), consoleWriteAnnotationPrefix);	
			}
		}
		else if (consoleCommand.first == ConsoleWriteCode::loadFromFile) {
			createGameFFile(consoleCommand.second, this).runGame();
		}
		else if (consoleCommand.first == ConsoleWriteCode::dumpToFile) {
			UniverseExporter().exportUniverseToFile(
				consoleCommand.second,
				universeName,
				con.gRen.getCanvasSize(),
				logic.getRule(),
				logic.getField()
			);
			con.writeLineWithPrefix(std::format("Successfully dumped universe to {}", consoleCommand.second), consoleWriteAnnotationPrefix);
		}
		else if (consoleCommand.first == ConsoleWriteCode::help) {
			con.writeLineWithPrefix("", consoleWriteHelpPrefix);
			con.writeLineWithPrefix("\t\tHelp", consoleWriteHelpPrefix);
			con.writeLineWithPrefix("", consoleWriteHelpPrefix);
			for (auto al : aliasesForWriteCodes) {
				con.writeLineWithPrefix(al.helpPopUp, consoleWriteHelpPrefix);
				std::stringstream aliases;
				aliases << "\t\t";
				for (auto alias : al.alias) {
					aliases << " " << alias;
					for (SHORT params = 0; params < al.numberParams; params++) {
						aliases << " " << consoleWriteParameterHelpPopUpSym;
					}
					aliases << ",";
				}
				aliases.seekp(-1, aliases.cur); aliases << ' ';
				con.writeLineWithPrefix(aliases.str(), consoleWriteHelpPrefix);
			}
		}
	};
	con.switchFromConsoleMode();
}

void LifeGame::setBlankField() {
	logic.cleanField();
}

void LifeGame::clearField() {
	logic.clearField();
}

void LifeGame::renderFrame(const bool& isSimulate, const ConsoleInteractiveCode& state) {
	if(isSimulate) logic.simulate();
	con.renderConsoleFrame(logic.getField(), state);
}

bool LifeGame::placeCell(const SHORT& x, const SHORT& y) {
	return logic.placeCell(x, y);
}

bool LifeGame::clearCell(const SHORT& x, const SHORT& y) {
	return logic.clearCell(x, y);
}