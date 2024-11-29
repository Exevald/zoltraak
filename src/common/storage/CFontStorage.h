#pragma once

#include <SFML/Graphics/Font.hpp>
#include <unordered_map>

class CFontStorage
{
public:
	static sf::Font& GetFont(const std::string& filePath);

private:
	static std::unordered_map<std::string, sf::Font> m_fontCache;
};
