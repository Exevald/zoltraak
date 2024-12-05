#include "CGameController.h"
#include "animation/CAnimationSystem.h"
#include "camera/CCameraSystem.h"
#include "collision/CCollisionSystem.h"
#include "view/CViewSystem.h"
#include <fstream>
#include <iostream>

float CGameController::m_elapsedTime = 0;
float CGameController::m_levelWidth = 0;
float CGameController::m_levelHeight = 0;
CurrentState CGameController::m_gameState = CurrentState::MainMenu;
int CGameController::m_currentMainMenuOption = -1;
int CGameController::m_currentPauseMenuOption = -1;
int CGameController::m_currentGameSaveNumber = 0;

void CGameController::InitGameSettings(const Level& level)
{
	m_levelWidth = float(level.front().size()) * 20 * 3 - 60;
	m_levelHeight = float(level.size()) * 20 * 3 - 60;
}

void CGameController::InitSystems()
{
	CMovementSystem::Init();
	CCollisionSystem::Init();
}

void CGameController::Draw(sf::RenderWindow& window, Level& level)
{
	static CViewSystem viewSystem(window, level);
	static CCameraSystem cameraSystem(window);

	viewSystem.Init();
	cameraSystem.Init();

	viewSystem.Draw();
}

void CGameController::Update(float deltaTime)
{
	CAnimationSystem::Update(deltaTime);
}

void CGameController::SetSelectedEntityId(EntityId id)
{
	m_selectedEntityId = id;
}

EntityId CGameController::GetSelectedEntityId() const
{
	return m_selectedEntityId;
}

sf::Vector2f CGameController::GetWindowSizeSettings()
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
	m_currentGameSaveNumber = saveNumber;
}

int CGameController::GetCurrentGameSaveNumber()
{
	return m_currentGameSaveNumber;
}

void CGameController::SaveGameInfo(int saveNumber, const SaveInfo& info)
{
	std::string fileName;
	switch (saveNumber)
	{
	case 0: {
		fileName = "save1.txt";
		break;
	}
	case 1: {
		fileName = "save2.txt";
		break;
	}
	case 2: {
		fileName = "save3.txt";
		break;
	}
	default:
		throw std::invalid_argument("invalid save number");
	}

	std::ofstream outputFile;
	outputFile.open(fileName, std::ios::out | std::ios::trunc);
	if (!outputFile.is_open())
	{
		throw std::runtime_error("Failed to open " + fileName + " for writing");
	}

	outputFile << info.PlayerName << std::endl;
	outputFile << info.currentLocation << std::endl;
	outputFile << info.gameTime << std::endl;

	if (!outputFile.flush())
	{
		throw std::runtime_error("Failed to save data on disk");
	}
}

SaveInfo CGameController::GetSaveInfo(int saveNumber)
{
	std::string fileName = CGameController::GetSaveFileName(saveNumber);

	std::ifstream inputFile;
	inputFile.open(fileName);
	if (!inputFile.is_open())
	{
		throw std::runtime_error("Failed to open " + fileName + " for reading");
	}

	SaveInfo info;

	std::getline(inputFile, info.PlayerName);
	std::getline(inputFile, info.currentLocation);
	inputFile >> info.gameTime;

	return info;
}

std::string CGameController::GetSaveFileName(int saveNumber)
{
	std::string fileName;
	switch (saveNumber)
	{
	case 0: {
		fileName = "save1.txt";
		break;
	}
	case 1: {
		fileName = "save2.txt";
		break;
	}
	case 2: {
		fileName = "save3.txt";
		break;
	}
	default:
		throw std::invalid_argument("invalid save number");
	}

	return fileName;
}

void CGameController::IncreaseElapsedTime(float deltaTime)
{
	m_elapsedTime += deltaTime;
}

float CGameController::GetElapsedTIme()
{
	return m_elapsedTime;
}
