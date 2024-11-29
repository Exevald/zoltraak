#include "CTextureStorage.h"

std::unordered_map<std::string, sf::Texture> CTextureStorage::m_textureCache;

sf::Texture& CTextureStorage::GetTexture(const std::string& filePath)
{
	auto it = m_textureCache.find(filePath);
	if (it == m_textureCache.end())
	{
		sf::Image image;
		if (!image.loadFromFile(filePath))
		{
			throw std::runtime_error("Failed to load texture: " + filePath);
		}

		sf::Color backgroundColor(195, 134, 255);
		image.createMaskFromColor(backgroundColor);

		sf::Texture texture;
		if (!texture.loadFromImage(image))
		{
			throw std::runtime_error("Failed to create texture from image: " + filePath);
		}

		m_textureCache[filePath] = texture;
	}

	return m_textureCache[filePath];
}