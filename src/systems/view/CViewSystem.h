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

struct MainMenu
{
	sf::Texture menuSoulTexture;
	sf::Sprite menuSoul;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	std::vector<sf::Text> menuOptions;
	int selectedMenuOption = -1;
};

struct PauseMenu
{
	sf::Texture pauseMenuAreaTexture;
	sf::Sprite pauseMenuArea;
	sf::Text pauseMenuTitle;
	std::vector<sf::Text> menuOptions;
};

struct LevelChoosingMenu
{
	sf::Text levelChoosingMenuTitle;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	std::vector<sf::Sprite> levelCards;
	std::unordered_map<int, std::vector<sf::Text>> levelCardsInfo;
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
	void DrawMainMenu();
	void DrawLevelChoosingMenu();
	void DrawMenuSoul();
	void DrawPauseMenu();

	sf::RenderWindow& m_window;
	Level& m_level;
	HeroCard m_heroCard;
	MainMenu m_menu;
	PauseMenu m_pauseMenu;
	LevelChoosingMenu m_levelChoosingMenu;
};
