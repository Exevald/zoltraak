#pragma once

#include "../../CGameController.h"
#include "CEntityManager.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>

struct SaveInfo;

struct HeroCard
{
	bool isVisible = false;
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

struct HeroInfoCard
{
	sf::Text heroName;
	sf::RectangleShape area;
	sf::Sprite heroIcon;
	sf::Text heroHealthText;
	sf::Text heroHealthValue;
	sf::RectangleShape heroHealthBar;
	sf::RectangleShape heroMaxHealthBar;
	sf::RectangleShape actionsArea;
	std::vector<sf::Sprite> actions;
};

struct FightScene
{
	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::Sprite fightInfoCard;
	std::vector<sf::Text> fightInfoText;
	std::unordered_map<int, HeroInfoCard> heroesFightInfo;
	sf::Sprite heroSoul;
	sf::Sprite battleArea;
};

struct Inventory
{
	sf::Sprite sectionTitle;
	std::vector<sf::Sprite> inventorySections;
	sf::Sprite inventoryMenu;
	sf::Text money;
	std::vector<sf::Sprite> heroIcons;
	sf::Sprite inventoryMenuSoul;
	sf::Text heroDescription;
	std::vector<sf::Text> heroStats;
	std::vector<sf::Text> inventoryItemMenuActions;
	std::unordered_map<int, sf::Text> inventoryItems;
	std::unordered_map<int, HeroInfoCard> heroesInfo;
	sf::Text heroName;
};

class CViewSystem
{
public:
	explicit CViewSystem(sf::RenderWindow& window, Level& level)
		: m_window(window)
		, m_level(level)
	{
		m_heroCard.isVisible = false;
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
	void DrawLevelChoosingTitle();
	void DrawLevelChoosingBackground();
	void DrawLevelSaveCards();
	void CreateLevelSaveCards();
	void DrawLevelSaveCardsInfo();
	void CreateLevelSaveCardInfo(int saveIndex, const SaveInfo& saveInfo);
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
	void UpdateActiveFightUserInfo(EntityId entity);
	void SetFightInfo();
	void SetAttackBar(const sf::Vector2f& fightInfoCardPosition);
	void SetSpareText();
	void SetActActions();
	void SetFightInventory();
	void SetMagicText();
	void DrawInventory();
	void UpdateInventoryItems();

	sf::RenderWindow& m_window;
	Level& m_level;
	HeroCard m_heroCard;
	MainMenu m_menu;
	PauseMenu m_pauseMenu;
	LevelChoosingMenu m_levelChoosingMenu;
	FightScene m_fightingScene;
	Inventory m_inventory;
	FightAction m_currentFightAction = FightAction::Info;
	int m_activeFightHero = 2;
	bool m_isAttackBarActive = false;
	InventoryState m_currentInventoryState = InventoryState::MenuSectionSelection;
	InventoryAction m_currentInventoryItemAction = InventoryAction::Use;
};
