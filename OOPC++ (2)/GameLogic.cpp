#include "GameLogic.h"

GameLogic::GameLogic() : width(WIDTHDEFAULT), height(HEIGHTDEFAULT) {
	gameField = Field{ width, height };
	rule = DEFAULT_RULE;
}

GameLogic::GameLogic(const SHORT& iWidth, const SHORT& iHeight) : width(iWidth), height(iHeight) {
	gameField = Field{ width, height };
	rule = DEFAULT_RULE;
}

GameLogic::GameLogic(const SHORT& iWidth, const SHORT& iHeight, const GameRule& iRule) : rule(iRule), width(iWidth), height(iHeight) {
	gameField = Field{ width, height };
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

SHORT GameLogic::checkAliveSquare(const SHORT& iY, const SHORT& iX) {
	SHORT cntCellsAround = 0;
	for (SHORT line = iY; line < iY + 3; line++) {
		for (SHORT x = iX; x < iX + 3; x++) {
			if (gameField.at(line, x) != empty && (line != iY+1 || x != iX+1)) {
				cntCellsAround += 1;
			}
		}
	}
	return cntCellsAround;
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