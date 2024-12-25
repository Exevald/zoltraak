#pragma once

#include <SFML/Graphics.hpp>

class IRenderer
{
public:
	virtual void Init() = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;

	virtual ~IRenderer() = default;
};