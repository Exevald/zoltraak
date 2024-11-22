#include "CGameController.h"
#include "camera/CCameraSystem.h"
#include "collision/CCollisionSystem.h"
#include "view/CViewSystem.h"

void CGameController::InitSystems()
{
	CMovementSystem::Init();
	CCollisionSystem::Init();
}

void CGameController::Draw(sf::RenderWindow& window)
{
	static CViewSystem viewSystem(window);
	static CCameraSystem cameraSystem(window);

	viewSystem.Draw();
	cameraSystem.Init();
}

void CGameController::SetSelectedEntityId(EntityId id)
{
	m_selectedEntityId = id;
}

EntityId CGameController::GetSelectedEntityId() const
{
	return m_selectedEntityId;
}
