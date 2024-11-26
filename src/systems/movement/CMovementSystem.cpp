#include "movement/CMovementSystem.h"
#include "events/CEventDispatcher.h"
#include "Utils.h"

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
	GameFieldSettings gameFieldSettings;
	auto& entityManager = CEntityManager::GetInstance();

	auto* positionComponent = entityManager.GetComponent<PositionComponent>(movedEntityId);
	auto* velocityComponent = entityManager.GetComponent<VelocityComponent>(movedEntityId);

	if (!positionComponent)
	{
		return;
	}

	float newX = positionComponent->x;
	float newY = positionComponent->y;
	if (direction == "up")
	{
		newY -= velocityComponent->vy;
	}
	else if (direction == "down")
	{
		newY += velocityComponent->vy;
	}
	else if (direction == "left")
	{
		newX -= velocityComponent->vx;
	}
	else if (direction == "right")
	{
		newX += velocityComponent->vx;
	}
	newX = std::max(0.f, std::min(newX, float(gameFieldSettings.gameWidth) - 50.f));
	newY = std::max(0.f, std::min(newY, float(gameFieldSettings.gameHeight) - 50.f));

	positionComponent->x = newX;
	positionComponent->y = newY;
}
