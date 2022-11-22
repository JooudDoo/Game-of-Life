#include "FileWorkers.h"
#include <iostream>
#include <filesystem>
#include <ctime>

static std::string currentDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm now;
	localtime_s(&now, &t);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%m-%d-%Y %X", &now);
	return buffer;
}

namespace fs = std::filesystem;

UniverseExporter::UniverseExporter() {
	std::string logFileName = std::format("{}[{}].log", standartExportLogName, currentDateTime());
	if (!fs::exists(fs::current_path().string() + "\\logs")) {
		if (!std::filesystem::create_directories(fs::current_path().string() + "\\logs")) {
			logFile.open(logFileName);
			return;
		}
	}
	logFile.open(std::format("logs/{}", logFileName));
}

UniverseExporter::~UniverseExporter() {
	logFile.close();
}

bool UniverseExporter::exportUniverseTF(const std::string& filePath, const std::string& universeName, const COORD& consoleSize, const GameRule& rules, const Field& gameField) {
	std::ofstream exportFile(filePath);
	exportFile << lifeVerBlank << "1.06" << std::endl;
	exportFile << lifeNameBlank << universeName << std::endl;
	exportFile << lifeSizeBlank << consoleSize.X << "x" << consoleSize.Y << std::endl;
	exportFile << lifeRuleBlank << "B";
	for (auto iRule : rules.birth) {
		exportFile << iRule;
	}
	exportFile << "/S";
	for (auto iRule : rules.stay) {
		exportFile << iRule;
	}
	exportFile << std::endl;
	for (size_t x = 0; x < gameField.width; x++) {
		for (size_t y = 0; y < gameField.height; y++) {
			if (gameField.data[y][x] != Cell_T::empty) {
				exportFile << x << ' ' << y << std::endl;
			}
		}
	}


	exportFile.close();
	return true;
}