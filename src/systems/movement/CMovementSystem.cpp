#include "movement/CMovementSystem.h"
#include "../../events/CEventDispatcher.h"

void CMovementSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntityMoved, [](const SEvent& event) {
		if (event.type == EventType::EntityMoved)
		{
			const auto& movedEventData = std::get<EntityMovedEventData>(event.data);
			EntityId entityId = movedEventData.id;
			const std::string& direction = movedEventData.direction;

			auto positionComponent = CEntityManager::GetInstance().GetComponent<PositionComponent>(entityId);
			if (!positionComponent)
			{
				return;
			}

			if (direction == "up")
			{
				positionComponent->y -= defaultVelocity;
			}
			else if (direction == "down")
			{
				positionComponent->y += defaultVelocity;
			}
			else if (direction == "left")
			{
				positionComponent->x -= defaultVelocity;
			}
			else if (direction == "right")
			{
				positionComponent->x += defaultVelocity;
			}
		}
	});
}