#include "CGameController.h"
#include "view/CViewSystem.h"

void CGameController::Init()
{
	CMovementSystem::Init();
}

void CGameController::Draw(sf::RenderWindow& window)
{
	CViewSystem viewSystem(window);

	viewSystem.Draw();
}

void CGameController::SetSelectedEntityId(EntityId id)
{
	m_selectedEntityId = id;
}

EntityId CGameController::GetSelectedEntityId() const
{
	return m_selectedEntityId;
}
