#include "CLevelGenerator.h"
#include <fstream>
#include <iostream>
#include <string>

std::unordered_map<std::string, Level> CLevelGenerator::m_levelStorage;

Level CLevelGenerator::GenerateLevel(const std::string& levelFileName)
{
	std::ifstream levelFile;
	levelFile.open(levelFileName);
	if (!levelFile.is_open())
	{
		throw std::runtime_error("Failed to open " + levelFileName + " for reading");
	}

	Level level;
	std::string line;
	int linePos = 0;

	while (std::getline(levelFile, line))
	{
		if (linePos >= LevelMaxHeightSize)
		{
			throw std::runtime_error("Labyrinth height bigger than " + std::to_string(LevelMaxHeightSize));
		}
		if (line.length() > LevelMaxWidthSize)
		{
			throw std::runtime_error("Labyrinth width bigger than " + std::to_string(LevelMaxWidthSize));
		}

		LevelLine levelLine;
		for (int i = 0; i < line.length(); i++)
		{
			levelLine[i] = line[i];
		}
		level[linePos] = levelLine;
		linePos++;
	}
	m_levelStorage[levelFileName] = level;

	return level;
}

void CLevelGenerator::PrintLevel(const Level& level)
{
	for (auto line : level)
	{
		for (auto ch : line)
		{
			std::cout << ch;
		}
		std::cout << std::endl;
	}
}

Level CLevelGenerator::GetLevel(const std::string& levelName)
{
	return m_levelStorage[levelName];
}
