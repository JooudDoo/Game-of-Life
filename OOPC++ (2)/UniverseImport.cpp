#include "FileWorkers.h"
#include <iostream>
#include <filesystem>
#include <ctime>

static std::string currentDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm now;
	localtime_s(&now, &t);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%H%M%S-%m-%d-%Y", &now);
	return buffer;
}

namespace fs = std::filesystem;
GameRule DEFAULTRULE = { {3}, {2, 3} };

UniverseImporter::UniverseImporter() {
	std::string logFileName = std::format("{}[{}].log", standartImportLogName, currentDateTime());
	if (!fs::exists(fs::current_path().string() + "\\logs")) {
		if (!std::filesystem::create_directories(fs::current_path().string() + "\\logs")) {
			logFile.open(logFileName);
			return;
		}
	}
	logFile.open(std::format("logs/{}", logFileName));
};

UniverseImporter::~UniverseImporter() {
	logFile.close();
}

GameData UniverseImporter::importUniverseFF(const std::string& filePath) {
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
		data = "UniverseVerse";
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
		if (newSize.first < 0) {
			logFile << "[SIZE] [WARNING] The field size was found, but not correct. Default size will be used: " << worldData.gameField.height << "x" << worldData.gameField.width << std::endl;
		}
		else {
			worldData.gameField = Field{ newSize.first, newSize.second };
		}
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
		std::pair<bool, COORD> coord = checkCell(line);
		if (coord.first && worldData.gameField.placeCell(coord.second.Y, coord.second.X)) {
			logFile << "[CELL] \tSuccesufully placed cell at " << line << std::endl;
		}
		else {
			if(coord.first)
				logFile << "[CELL] \tCell already occupied at " << line << std::endl;
			else
				logFile << "[CELL] \tIncorrect data: " << line << std::endl;
		}
	}

	file.close();
	return worldData;
}

bool UniverseImporter::checkBlank(const std::string& blank, const std::string& line, std::string& returnValue) {
	size_t pos = line.find(blank);
	if (pos == line.npos)
		return false;
	char buff[1024] = {0};
	line.copy(buff, line.length() - blank.length(), blank.length());
	returnValue = std::string(buff);
	return true;
}

std::pair<bool, COORD> UniverseImporter::checkCell(const std::string& line) {
	SHORT m1, m2;
	if (sscanf_s(line.c_str(), "%hd %hd", &m1, &m2) == EOF) return { false, {0, 0} };
	return { true, {m1, m2} };
}