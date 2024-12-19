#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class CTextureStorage
{
public:
	static sf::Texture& GetTexture(const std::string& filePath, const sf::Color& backgroundColor = sf::Color(195, 134, 255));

private:
	static std::unordered_map<std::string, sf::Texture> m_textureCache;
};
