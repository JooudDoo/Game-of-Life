#include "Parser.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
GameRule DEFAULTRULE = { {3}, {2, 3} };


FileParser::FileParser() {
	if (!fs::exists(fs::current_path().string() + "\\logs")) {
		if (!std::filesystem::create_directories(fs::current_path().string() + "\\logs")) {
			logFile.open("\\log.txt");
			return;
		}
	}
	logFile.open("logs\\log.txt");
};

FileParser::~FileParser() {
	logFile.close();
}

GameData FileParser::parseUniverseFF(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open())
		throw std::exception("File not found");
	GameData worldData;
	
	logFile << "Parsing file: " << filePath << std::endl << std::endl;

	std::string line, data;
	bool skipNextRead = false;
	if (!skipNextRead) std::getline(file, line);
	if(!checkBlank(lifeVerBlank, line, data))
		throw std::exception("The version of the game of life wasn't found");
	logFile << "[VERSION] Life game version using: " << data << std::endl;

	if(!skipNextRead) std::getline(file, line);
	if (!checkBlank(lifeNameBlank, line, data)) {
		logFile << "[NAME] [WARNING] Universe name not found. Using default name." << std::endl;
		data = "GachiVerse";
		skipNextRead = true;
	}
	else {
		skipNextRead = false;
	}
	logFile << "[NAME] Universe name: \"" << data << "\"" << std::endl;
	worldData.name = data;

	if (!skipNextRead) std::getline(file, line);
	if (!checkBlank(lifeSizeBlank, line, data)) {
		logFile << "[SIZE] [WARNING] The field size was not found, the default size will be used: " << worldData.gameField.height << "x" << worldData.gameField.width << std::endl;
		skipNextRead = true;
	}
	else {
		logFile << "[SIZE] The field size will be: " << data << std::endl;
		std::pair<SHORT, SHORT> newSize = sizeFString(data); //TODO smt wrong with line
		worldData.gameField = Field{ newSize.first, newSize.second };
		skipNextRead = false;
	}

	if (!skipNextRead) std::getline(file, line);
	if (!checkBlank(lifeRuleBlank, line, data)) {
		logFile << "[RULE] [WARNING] Game rule not found. Using default rule." << "B2/S23" << std::endl;
		worldData.rule = DEFAULTRULE;
		skipNextRead = true;
	}
	else {
		logFile << "[RULE] Using game rule: " << data << std::endl;
		worldData.rule = ruleFString(data); //TODO smt wrong with line
		skipNextRead = false;
	}
	
	while (!file.eof()) {
		if (!skipNextRead) std::getline(file, line);
		skipNextRead = false;
		std::pair<SHORT, SHORT> coord = checkCell(line);
		if (worldData.gameField.placeCell(coord.second, coord.first)) {
			logFile << "[CELL] \tSuccesufully placed cell at " << line << std::endl;
		}
		else {
			logFile << "[CELL] \tCell already occupied at " << line << std::endl;
		}
	}

	return worldData;
}

bool FileParser::checkBlank(const std::string& blank, const std::string& line, std::string& ver) {
	size_t pos = line.find(blank);
	if (pos == line.npos)
		return false;
	char buff[1024] = {0};
	line.copy(buff, line.length() - blank.length(), blank.length());
	ver = std::string(buff);
	return true;
}

std::pair<SHORT, SHORT> FileParser::checkCell(const std::string& line) {
	int m1, m2;
	sscanf_s(line.c_str(), "%d %d", &m1, &m2);
	return {m1, m2};
}