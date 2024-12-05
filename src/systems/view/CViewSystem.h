#pragma once

#include "../../CGameController.h"
#include "CEntityManager.h"
#include <SFML/Graphics.hpp>

struct SaveInfo;

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

struct HeroFightCard {
	sf::RectangleShape area;
	sf::Sprite heroIcon;
	sf::Text heroHpText;
	sf::Text heroHpValue;
};

struct FightScene
{
	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::Sprite infoCard;
	sf::Text infoText;
	std::vector<HeroFightCard> heroesFightInfo;
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
	void DrawMapItem(int x, int y, const sf::Texture& texture, const sf::IntRect& textureRect);
	void DrawField();
	void DrawItems();
	void UpdateHeroCard(EntityId selectedEntity);
	void UpdateHeroCardPosition();
	void DrawMainMenu();
	void DrawLevelChoosingMenu();
	void DrawLevelChoosingTitle(sf::Font& font);
	void DrawLevelChoosingBackground();
	void DrawLevelSaveCards(sf::Font& font);
	void CreateLevelSaveCards();
	void DrawLevelSaveCardsInfo(sf::Font& font);
	void CreateLevelSaveCardInfo(int saveIndex, const SaveInfo& saveInfo, sf::Font& font);
	void UpdateHeroCardBackground(const ColorThemeComponent* colorThemeComp);
	void UpdateHeroAvatar(const AvatarComponent* avatarComp);
	void UpdateHeroHealthBar(const HealthComponent* healthComp, const ColorThemeComponent* colorThemeComp);
	void UpdateHeroManaBar(const ManaComponent* manaComp);
	void UpdateHeroInfoText(const ExperienceComponent* experienceComp);
	void SetupPauseMenuArea();
	void SetupPauseMenuTitle();
	void SetupPauseMenuOptions();
	void DrawMenuSoul();
	void DrawPauseMenu();
	void DrawFightScene();

	sf::RenderWindow& m_window;
	Level& m_level;
	HeroCard m_heroCard;
	MainMenu m_menu;
	PauseMenu m_pauseMenu;
	LevelChoosingMenu m_levelChoosingMenu;
	FightScene m_fightingScene;
};
