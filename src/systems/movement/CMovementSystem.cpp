#include "movement/CMovementSystem.h"
#include "../CGameController.h"
#include "Utils.h"
#include "events/CEventDispatcher.h"
#include <iostream>

void CMovementSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntityMoved, [](const SEvent& event) {
		if (event.type == EventType::EntityMoved)
		{
			const auto& movedEventData = std::get<EntityMovedEventData>(event.data);
			EntityId entityId = movedEventData.id;
			const std::string& direction = movedEventData.direction;

			HandleEntityMoved(entityId, direction);
		}
	});
}

void CMovementSystem::HandleEntityMoved(EntityId movedEntityId, const std::string& direction)
{
	auto& entityManager = CEntityManager::GetInstance();

	auto* positionComponent = entityManager.GetComponent<PositionComponent>(movedEntityId);
	auto* velocityComponent = entityManager.GetComponent<VelocityComponent>(movedEntityId);
	auto* animationComponent = entityManager.GetComponent<AnimationComponent>(movedEntityId);
	auto* imageComponent = entityManager.GetComponent<ImageComponent>(movedEntityId);

	if (!positionComponent || (!animationComponent && !imageComponent))
	{
		return;
	}

	float newX = positionComponent->x;
	float newY = positionComponent->y;
	if (direction == "up" || direction == "down")
	{
		newY += velocityComponent->vy;
	}
	else if (direction == "left" || "right")
	{
		newX += velocityComponent->vx;
	}
	if (animationComponent)
	{
		newX = std::max(0.f, std::min(newX, float(CGameController::GetWindowSizeSettings().x) - animationComponent->sprite.getScale().x * 18));
		newY = std::max(0.f, std::min(newY, float(CGameController::GetWindowSizeSettings().y) - animationComponent->sprite.getScale().y * 37));
	}
	if (imageComponent)
	{
		newX = std::max(0.f, std::min(newX, float(CGameController::GetWindowSizeSettings().x) - 22));
		newY = std::max(0.f, std::min(newY, float(CGameController::GetWindowSizeSettings().y) - 22));
	}

	positionComponent->x = newX;
	positionComponent->y = newY;
}
