#include "CGameController.h"
#include "animation/CAnimationSystem.h"
#include "camera/CCameraSystem.h"
#include "collision/CCollisionSystem.h"
#include "event/CEventSystem.h"
#include "fight/CFightSystem.h"
#include "inventory/CInventorySystem.h"
#include "view/CViewSystem.h"
#include <fstream>
#include <iostream>

namespace
{
constexpr int TILE_SIZE = 20;
constexpr int SCALE_FACTOR = 3;
constexpr int MARGIN = 60;
const std::array<std::string, 3> SAVE_FILES = { "save1.txt", "save2.txt", "save3.txt" };
} // namespace

float CGameController::m_elapsedTime = 0;
float CGameController::m_levelWidth = 0;
float CGameController::m_levelHeight = 0;
CurrentState CGameController::m_gameState = CurrentState::MainMenu;
int CGameController::m_currentMainMenuOption = -1;
int CGameController::m_currentPauseMenuOption = -1;
int CGameController::m_currentGameSaveNumber = 0;
int CGameController::m_currentFightActionNumber = 0;
FightPhase CGameController::m_currentFightPhase = FightPhase::CharactersTurn;
sf::Clock CGameController::m_clock;
float CGameController::m_deltaTime = 0.f;
int CGameController::m_currentInventoryMenuPosition = 0;
int CGameController::m_currentInventorySectionNumber = 0;
int CGameController::m_selectedInventoryItemNumber = 0;
std::unordered_map<EntityId, std::vector<InventoryItem>> CGameController::m_allHeroesInventory;
int CGameController::m_activeInventoryCharacterNumber = 1;
EquipmentType CGameController::m_currentEquipmentType = EquipmentType::Weapon;
int CGameController::m_currentEquipmentItemNumber = 0;

void CGameController::InitGameSettings(const Level& level)
{
	m_levelWidth = float(level.front().size()) * TILE_SIZE * SCALE_FACTOR - MARGIN;
	m_levelHeight = float(level.size()) * TILE_SIZE * SCALE_FACTOR - MARGIN;
}

void CGameController::InitSystems()
{
	CEventSystem::Init();
	CMovementSystem::Init();
	CCollisionSystem::Init();
	CFightSystem::Init();
	CInventorySystem::Init();
}

void CGameController::Draw(sf::RenderWindow& window, Level& level)
{
	static CViewSystem viewSystem(window, level);
	static CCameraSystem cameraSystem(window);

	viewSystem.Init();
	cameraSystem.Init();
	viewSystem.Draw();
}

void CGameController::Update()
{
	CAnimationSystem::Update();
	CFightSystem::Update();
	CInventorySystem::Update();
}

void CGameController::SetSelectedEntityId(EntityId id)
{
	m_selectedEntityId = id;
}

EntityId CGameController::GetSelectedEntityId() const
{
	return m_selectedEntityId;
}

sf::Vector2f CGameController::GetGameSizeSettings()
{
	return { m_levelWidth, m_levelHeight };
}

void CGameController::SetGameState(const CurrentState& state)
{
	m_gameState = state;
}

CurrentState CGameController::GetCurrentGameState()
{
	return m_gameState;
}

void CGameController::SetCurrentMainMenuOption(int option)
{
	m_currentMainMenuOption = option;
}

int CGameController::GetCurrentMainMenuOption()
{
	return m_currentMainMenuOption;
}

void CGameController::SetCurrentPauseMenuOption(int option)
{
	m_currentPauseMenuOption = option;
}

int CGameController::GetCurrentPauseMenuOption()
{
	return m_currentPauseMenuOption;
}

void CGameController::SetCurrentGameSaveNumber(int saveNumber)
{
	if (saveNumber < 0 || saveNumber >= SAVE_FILES.size())
	{
		throw std::invalid_argument("Invalid save number");
	}
	m_currentGameSaveNumber = saveNumber;
}

int CGameController::GetCurrentGameSaveNumber()
{
	return m_currentGameSaveNumber;
}

void CGameController::SaveGameInfo(int saveNumber, const SaveInfo& info)
{
	std::string fileName = GetSaveFileName(saveNumber);

	std::ofstream outputFile(fileName, std::ios::out | std::ios::trunc);
	if (!outputFile.is_open())
	{
		throw std::runtime_error("Failed to open " + fileName + " for writing");
	}

	outputFile << info.PlayerName << "\n"
			   << info.currentLocation << "\n"
			   << info.gameTime << "\n";

	if (!outputFile)
	{
		throw std::runtime_error("Failed to write data to " + fileName);
	}
}

SaveInfo CGameController::GetSaveInfo(int saveNumber)
{
	std::string fileName = GetSaveFileName(saveNumber);

	std::ifstream inputFile(fileName);
	if (!inputFile.is_open())
	{
		throw std::runtime_error("Failed to open " + fileName + " for reading");
	}

	SaveInfo info;
	std::getline(inputFile, info.PlayerName);
	std::getline(inputFile, info.currentLocation);
	inputFile >> info.gameTime;

	if (inputFile.fail())
	{
		throw std::runtime_error("Failed to read data from " + fileName);
	}

	return info;
}

std::string CGameController::GetSaveFileName(int saveNumber)
{
	if (saveNumber < 0 || saveNumber >= SAVE_FILES.size())
	{
		throw std::invalid_argument("Invalid save number");
	}
	return SAVE_FILES[saveNumber];
}

void CGameController::IncreaseElapsedTime()
{
	m_elapsedTime += CGameController::GetDeltaTime();
}

float CGameController::GetElapsedTIme()
{
	return m_elapsedTime;
}

void CGameController::SetCurrentFightActionNumber(int actionNumber)
{
	m_currentFightActionNumber = actionNumber;
}

int CGameController::GetCurrentFightActionNumber()
{
	return m_currentFightActionNumber;
}

void CGameController::SetFightPhase(const FightPhase& phase)
{
	m_currentFightPhase = phase;
}

FightPhase CGameController::GetCurrentFightPhase()
{
	return m_currentFightPhase;
}

void CGameController::UpdateDeltaTime()
{
	m_deltaTime = m_clock.restart().asSeconds();
}

float CGameController::GetDeltaTime()
{
	return m_deltaTime;
}

void CGameController::SetCurrentInventoryMenuPosition(int position)
{
	m_currentInventoryMenuPosition = position;
}

int CGameController::GetCurrentInventoryMenuPosition()
{
	return m_currentInventoryMenuPosition;
}

void CGameController::SetCurrentInventorySectionNumber(int sectionNumber)
{
	m_currentInventorySectionNumber = sectionNumber;
}

int CGameController::GetCurrentInventorySectionNumber()
{
	return m_currentInventorySectionNumber;
}

void CGameController::SetSelectedInventoryItemNumber(int itemNumber)
{
	m_selectedInventoryItemNumber = itemNumber;
}

int CGameController::GetSelectedInventoryItemNumber()
{
	return m_selectedInventoryItemNumber;
}

void CGameController::UpdateHeroInventory(EntityId heroId, const std::vector<InventoryItem>& items)
{
	m_allHeroesInventory[heroId] = items;
}

std::unordered_map<EntityId, std::vector<InventoryItem>> CGameController::GetAllHeroesInventory()
{
	return m_allHeroesInventory;
}

void CGameController::RemoveItemFromHeroInventory(EntityId heroId, int itemIndex)
{
	auto it = m_allHeroesInventory.find(heroId);
	if (it == m_allHeroesInventory.end())
	{
		return;
	}
	if (itemIndex < 0 || itemIndex >= it->second.size())
	{
		return;
	}

	it->second.erase(it->second.begin() + itemIndex);
}

void CGameController::SetActiveInventoryCharacterNumber(int activeCharacterNumber)
{
	m_activeInventoryCharacterNumber = activeCharacterNumber;
}

int CGameController::GetActiveInventoryCharacterNumber()
{
	return m_activeInventoryCharacterNumber;
}

void CGameController::SetCurrentInventoryEquipmentType(const EquipmentType& type)
{
	m_currentEquipmentType = type;
}

EquipmentType CGameController::GetCurrentEquipmentType()
{
	return m_currentEquipmentType;
}

void CGameController::SetCurrentInventoryEquipmentItemNumber(int number)
{
	m_currentEquipmentItemNumber = number;
}

int CGameController::GetCurrentEquipmentItemNumber()
{
	return m_currentEquipmentItemNumber;
}
