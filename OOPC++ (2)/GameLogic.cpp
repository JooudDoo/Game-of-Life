#include "GameLogic.h"

static GameRule DEFAULTRULE = { {3}, {2, 3} };

GameLogic::GameLogic() : width(WIDTHDEFAULT), height(HEIGHTDEFAULT) {
	gameField = Field{ width, height };
	blankField = Field{ width, height };
	rule = DEFAULTRULE;
}

GameLogic::GameLogic(const SHORT& iWidth, const SHORT& iHeight) : width(iWidth), height(iHeight) {
	gameField = Field{ width, height };
	blankField = Field{ width, height };
	rule = DEFAULTRULE;
}

GameLogic::GameLogic(const SHORT& iWidth, const SHORT& iHeight, const GameRule& iRule) : rule(iRule), width(iWidth), height(iHeight) {
	gameField = Field{ width, height };
	blankField = Field{ width, height };
}

void GameLogic::setRule(const GameRule& r) {
	rule = r;
}

Field& GameLogic::getField() {
	return gameField;
}

void GameLogic::simulate() {
	Field newField = Field(gameField);
	for (SHORT line = 0; line < height; line++) {
		for (SHORT x = 0; x < width; x++) {
			SHORT cntCellsAround = checkAliveSquare(line - 1, x - 1);
			if (gameField.at(line, x) == empty) {
				if (std::find(rule.birth.begin(), rule.birth.end(), cntCellsAround) != rule.birth.end()) {
					newField.placeCell(line, x);
				}
			}
			else {
				if (std::find(rule.stay.begin(), rule.stay.end(), cntCellsAround) == rule.stay.end()) {
					newField.clearCell(line, x);
				}
			}
			
		}

	}
	gameField = newField;
}

void GameLogic::simulateTicks(const int& tickToPass) {
	for (int tick = 0; tick < tickToPass; tick++) {
		simulate();
	}
}

SHORT GameLogic::checkAliveSquare(const SHORT& iY, const SHORT& iX) {
	SHORT cntCellsAround = 0 - gameField.at(iY + 1, iX + 1);
	for (SHORT line = iY; line < iY + 3; line++) {
		for (SHORT x = iX; x < iX + 3; x++) {
			cntCellsAround += gameField.at(line, x);
		}
	}
	return cntCellsAround;
}

bool GameLogic::isEmptyCell(const SHORT& y, const SHORT& x) {
	if (gameField.at(y, x) == empty) {
		return true;
	}
	return false;
}

bool GameLogic::clearCell(const SHORT& y, const SHORT& x) {
	if (gameField.at(y, x) == empty) {
		return false;
	}
	gameField.at(y,x) = empty;
	return true;
}

bool GameLogic::placeCell(const SHORT& y, const SHORT& x) {
	if (gameField.at(y, x) != empty) {
		return false;
	}
	gameField.at(y, x) = cell;
	return true;
}

bool GameLogic::switchCell(const SHORT& y, const SHORT& x) {
	if (!placeCell(y, x)) {
		return clearCell(y, x);
	}
	return true;
}

bool GameLogic::setBlankField(const Field& newBlank) {
	blankField = newBlank;
	gameField = newBlank;
	return true;
}

void GameLogic::loadBlankField() {
	gameField = blankField;
}

void GameLogic::clearField() {
	gameField = Field(width, height);
}