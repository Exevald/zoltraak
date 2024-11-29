#include "CGameController.h"
#include "animation/CAnimationSystem.h"
#include "camera/CCameraSystem.h"
#include "collision/CCollisionSystem.h"
#include "view/CViewSystem.h"

float CGameController::m_levelWidth = 0;
float CGameController::m_levelHeight = 0;

void CGameController::InitGameSettings(const Level& level)
{
	m_levelWidth = float(level[0].size()) * 20 * 3 - 60;
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