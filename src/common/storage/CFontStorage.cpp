#include "CFontStorage.h"
#include <iostream>

std::unordered_map<std::string, sf::Font> CFontStorage::m_fontCache;

sf::Font& CFontStorage::GetFont(const std::string& filePath)
{
	auto it = m_fontCache.find(filePath);
	if (it == m_fontCache.end())
	{
		sf::Font font;
		if (!font.loadFromFile(filePath))
		{
			throw std::runtime_error("Failed to load font: " + filePath);
		}
		m_fontCache[filePath] = font;
	}
	return m_fontCache[filePath];
}
