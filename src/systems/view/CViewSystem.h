#pragma once

#include "CEntityManager.h"
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
};

class CViewSystem
{
public:
	explicit CViewSystem(sf::RenderWindow& window)
		: m_window(window)
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
	HeroCard m_heroCard;
	bool m_isAvatarLoaded;
};
