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
	void DrawField();
	void DrawItems();
	void DrawHeroCard();

	sf::RenderWindow& m_window;
};
