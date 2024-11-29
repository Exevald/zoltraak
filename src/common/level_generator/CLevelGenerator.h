#pragma once

#include <array>
#include <string>
#include <unordered_map>

typedef char CellType;
const int LevelMaxWidthSize = 33;
const int LevelMaxHeightSize = 33;
typedef std::array<CellType, LevelMaxWidthSize> LevelLine;
typedef std::array<LevelLine, LevelMaxHeightSize> Level;

class CLevelGenerator
{
public:
	static Level GenerateLevel(const std::string& levelFileName);
	static void PrintLevel(const Level& level);

	static Level GetLevel(const std::string& levelName);

private:
	static std::unordered_map<std::string, Level> m_levelStorage;
};
