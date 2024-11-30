#include "CCameraSystem.h"
#include "../CGameController.h"
#include "Utils.h"
#include "events/CEventDispatcher.h"

void CCameraSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntityMoved, [this](const SEvent& event) {
		if (event.type == EventType::EntityMoved)
		{
			const auto& movedEventData = std::get<EntityMovedEventData>(event.data);
			UpdateCameraPosition(movedEventData.id);
		}
	});
}

void CCameraSystem::UpdateCameraPosition(EntityId movedEntityId)
{
	WindowSettings windowSettings;

	auto& entityManager = CEntityManager::GetInstance();
	EntityId camera = entityManager.GetEntitiesWithComponents<CameraComponent>().front();
	auto* cameraComp = entityManager.GetComponent<CameraComponent>(camera);
	auto* positionComp = entityManager.GetComponent<PositionComponent>(movedEntityId);
	auto currentGameState = CGameController::GetCurrentGameState();

	if (!cameraComp || !positionComp || currentGameState == CurrentState::MainMenu)
	{
		return;
	}

	sf::View view = m_window.getView();

	const float windowHalfWidth = float(windowSettings.windowWidth) / 2.0f;
	const float windowHalfHeight = float(windowSettings.windowHeight) / 2.0f;

	float playerX = positionComp->x;
	float playerY = positionComp->y;

	float cameraX = playerX;
	float cameraY = playerY;

	if (playerX < cameraX - windowHalfWidth)
	{
		cameraX = playerX + windowHalfWidth;
	}
	if (playerX > cameraX + windowHalfWidth)
	{
		cameraX = playerX - windowHalfWidth;
	}
	if (playerY < cameraY - windowHalfHeight)
	{
		cameraY = playerY + windowHalfHeight;
	}
	if (playerY > cameraY + windowHalfHeight)
	{
		cameraY = playerY - windowHalfHeight;
	}

	cameraX = std::max(windowHalfWidth, std::min(cameraX, float(CGameController::GetWindowSizeSettings().x) - windowHalfWidth));
	cameraY = std::max(windowHalfHeight, std::min(cameraY, float(CGameController::GetWindowSizeSettings().y) - windowHalfHeight));

	view.setCenter(cameraX, cameraY);
	m_window.setView(view);
}
