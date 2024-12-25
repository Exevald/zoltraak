#pragma once

#include "CEntityManager.h"
#include "camera/CCameraSystem.h"
#include "collision/CCollisionSystem.h"
#include "inventory/item_factory/CInventoryItemFactory.h"
#include "level_generator/CLevelGenerator.h"
#include "movement/CMovementSystem.h"
#include "view/CViewSystem.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum class CurrentState
{
	MainMenu,
	LevelChoosing,
	Player,
	PauseMenu,
	Settings,
	Fight,
	Inventory,
	Vendor,
};

struct SaveInfo
{
	std::string PlayerName;
	std::string currentLocation;
	float gameTime;
};

enum class FightPhase
{
	CharactersTurn,
	EnemiesTurn,
	BattleEnded,
};

class CGameController
{
public:
	static void InitGameSettings(const Level& level);
	static void InitSystems(const CInventoryItemFactory& factory);
	static void Update();
	static void Draw(sf::RenderWindow& window, Level& level);
	static void SetSelectedEntityId(EntityId id);
	static void SaveGameInfo(int saveNumber, const SaveInfo& info);
	static void SetGameState(const CurrentState& state);
	static void SetCurrentMainMenuOption(int option);
	static void SetCurrentPauseMenuOption(int option);
	static void SetCurrentGameSaveNumber(int saveNumber);
	static void SetCurrentFightActionNumber(int actionNumber);
	static void SetFightPhase(const FightPhase& phase);
	static void IncreaseElapsedTime();
	static void UpdateDeltaTime();
	static void SetCurrentInventoryMenuPosition(int position);
	static void SetCurrentInventorySectionNumber(int sectionNumber);
	static void SetSelectedInventoryItemNumber(int itemNumber);
	static void UpdateHeroInventory(EntityId heroId, const std::vector<InventoryItem>& items);
	static void RemoveItemFromHeroInventory(EntityId heroId, int itemIndex);
	static void SetActiveInventoryCharacterNumber(int activeCharacterNumber);
	static void SetCurrentInventoryEquipmentType(const EquipmentType& type);
	static void SetCurrentInventoryEquipmentItemNumber(int number);
	static void SetCurrentVendorActionNumber(int number);
	static void UpdateAllHeroesInventory();
	static void SetCurrentVendorState(const VendorState& state);
	static void SetCurrentVendorItemToBuyNumber(int number);
	static void SetCurrentHeroItemToSellNumber(int number);

	static float GetElapsedTIme();
	static SaveInfo GetSaveInfo(int saveNumber);
	static FightPhase GetCurrentFightPhase();
	static int GetCurrentFightActionNumber();
	static int GetCurrentMainMenuOption();
	static int GetCurrentPauseMenuOption();
	static int GetCurrentGameSaveNumber();
	static EntityId GetSelectedEntityId();
	static sf::Vector2f GetGameSizeSettings();
	static CurrentState GetCurrentGameState();
	static float GetDeltaTime();
	static int GetCurrentInventoryMenuPosition();
	static int GetCurrentInventorySectionNumber();
	static int GetSelectedInventoryItemNumber();
	static std::unordered_map<EntityId, std::vector<InventoryItem>> GetAllHeroesInventory();
	static std::vector<InventoryItem> GetHeroesItemsToSell();
	static int GetActiveInventoryCharacterNumber();
	static EquipmentType GetCurrentEquipmentType();
	static int GetCurrentEquipmentItemNumber();
	static int GetCurrentVendorActionNumber();
	static VendorState GetCurrentVendorState();
	static int GetCurrentVendorItemToBuyNumber();
	static int GetCurrentHeroItemToSellNumber();

private:
	static EntityId m_selectedEntityId;
	static float m_elapsedTime;
	static float m_levelWidth;
	static float m_levelHeight;
	static CurrentState m_gameState;
	static int m_currentMainMenuOption;
	static int m_currentPauseMenuOption;
	static int m_currentGameSaveNumber;
	static int m_currentFightActionNumber;
	static FightPhase m_currentFightPhase;
	static sf::Clock m_clock;
	static float m_deltaTime;
	static int m_currentInventoryMenuPosition;
	static int m_currentInventorySectionNumber;
	static int m_selectedInventoryItemNumber;
	static std::unordered_map<EntityId, std::vector<InventoryItem>> m_allHeroesInventory;
	static int m_activeInventoryCharacterNumber;
	static EquipmentType m_currentEquipmentType;
	static int m_currentEquipmentItemNumber;
	static int m_currentVendorActionNumber;
	static VendorState m_currentVendorState;
	static int m_currentVendorItemToBuyNumber;
	static int m_currentHeroItemToSellNumber;

	static std::string GetSaveFileName(int saveNumber);
};
