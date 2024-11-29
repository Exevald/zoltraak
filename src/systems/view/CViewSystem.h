#pragma once

#include "CEntityManager.h"
#include "level_generator/CLevelGenerator.h"
#include <SFML/Graphics.hpp>

struct HeroCard
{
	bool visible = false;
	sf::RectangleShape shape;
	sf::Texture avatarTexture;
	sf::Sprite avatarSprite;
	sf::RectangleShape maxHealthBar;
	sf::RectangleShape healthBar;
	sf::RectangleShape maxManaBar;
	sf::RectangleShape manaBar;
	sf::Font heroCardFont;
	sf::Text heroHealthText;
	sf::Text heroManaText;
	sf::Text heroExperienceText;
};

class CViewSystem
{
public:
	explicit CViewSystem(sf::RenderWindow& window, Level& level)
		: m_window(window)
		, m_level(level)
	{
		m_heroCard.visible = false;
	}

	void Draw();
	void Init();

private:
	void DrawField();
	void DrawItems();
	void UpdateHeroCard(EntityId selectedEntity);
	void UpdateHeroCardPosition();

	sf::RenderWindow& m_window;
	Level& m_level;
	HeroCard m_heroCard;
};
