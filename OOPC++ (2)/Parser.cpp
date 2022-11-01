#include "Parser.h"
#include <fstream>
#include <iostream>

FileParser::FileParser() {};

GameData FileParser::parseUniverseFF(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open())
		throw std::exception("File not found");
	GameData worldData;
	
	//Parse first line (With #Life x.xx)
	std::string line, version;
	std::getline(file, line);
	if(!checkLifeVer(line, version))
		throw std::exception("The version of the game of life wasn't found");
	std::cerr << version << std::endl; //???????????????????????????????????????????????????????????????????????
	std::getline(file, line);

	return worldData;
}

bool FileParser::checkLifeVer(const std::string& line, std::string& ver) {
	size_t pos = line.find(lifeVerBlank);
	if (pos == line.npos)
		return false;
	line.copy(const_cast<char*>(ver.c_str()), line.length() - strlen(lifeVerBlank), strlen(lifeVerBlank));
	return true;
}