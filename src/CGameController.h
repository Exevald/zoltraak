#pragma once

#include "CEntityManager.h"
#include "camera/CCameraSystem.h"
#include "collision/CCollisionSystem.h"
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
};

struct SaveInfo
{
	std::string PlayerName;
	std::string currentLocation;
	float gameTime;
};

class CGameController
{
public:
	static void InitGameSettings(const Level& level);

	static void InitSystems();

	static void Update(float deltaTime);

	static void Draw(sf::RenderWindow& window, Level& level);

	void SetSelectedEntityId(EntityId id);

	static void SaveGameInfo(int saveNumber, const SaveInfo& info);

	static void SetGameState(const CurrentState& state);

	static void SetCurrentMainMenuOption(int option);

	static void SetCurrentPauseMenuOption(int option);

	static void SetCurrentGameSaveNumber(int saveNumber);

	static void IncreaseElapsedTime(float deltaTime);

	static float GetElapsedTIme();

	static SaveInfo GetSaveInfo(int saveNumber);

	static int GetCurrentMainMenuOption();

	static int GetCurrentPauseMenuOption();

	static int GetCurrentGameSaveNumber();

	[[nodiscard]] EntityId GetSelectedEntityId() const;

	static sf::Vector2f GetWindowSizeSettings();

	static CurrentState GetCurrentGameState();

private:
	EntityId m_selectedEntityId{};
	static float m_elapsedTime;
	static float m_levelWidth;
	static float m_levelHeight;
	static CurrentState m_gameState;
	static int m_currentMainMenuOption;
	static int m_currentPauseMenuOption;
	static int m_currentGameSaveNumber;

	static std::string GetSaveFileName(int saveNumber);
};
