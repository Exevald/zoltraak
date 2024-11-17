#pragma once

#include <SFML/Graphics.hpp>

class CViewSystem
{
public:
	explicit CViewSystem(sf::RenderWindow& window)
		: m_window(window)
	{
	}

	void Draw();

private:
	sf::RenderWindow& m_window;
};
