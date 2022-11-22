#pragma once
#include "utility.h"
#include <fstream>

constexpr auto standartImportLogName = "ImportLog";
constexpr auto standartExportLogName = "ExportLog";
constexpr auto lifeVerBlank = "#Life ";
constexpr auto lifeNameBlank = "#N ";
constexpr auto lifeSizeBlank = "#S ";
constexpr auto lifeRuleBlank = "#R ";

class UniverseImporter {
public:
	UniverseImporter();
	~UniverseImporter();

	GameData importUniverseFF(const std::string&); //parse universe from file [poleznoe]

private:

	std::ofstream logFile;
	bool checkBlank(const std::string&, const std::string&, std::string&);
	std::pair<bool, COORD> checkCell(const std::string&);

};

class UniverseExporter {
public:
	UniverseExporter();
	~UniverseExporter();

	bool exportUniverseTF(const std::string& filePath, const std::string& universeName, const COORD& consoleSize, const GameRule& rules, const Field& gameField);

private:
	std::ofstream logFile;
};