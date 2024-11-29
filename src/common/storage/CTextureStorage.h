#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class CTextureStorage
{
public:
	static sf::Texture& GetTexture(const std::string& filePath);

private:
	static std::unordered_map<std::string, sf::Texture> m_textureCache;
};

