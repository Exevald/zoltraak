#include "movement/CMovementSystem.h"
#include "../../events/CEventDispatcher.h"
#include <iostream>

void CMovementSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntityMoved, [](const SEvent& event) {
		if (event.type == EventType::EntityMoved)
		{
			const auto& movedEventData = std::get<EntityMovedEventData>(event.data);

			Entity* entity = movedEventData.entity;
			const std::string direction = movedEventData.direction;

			if (direction == "up")
			{
				entity->components.positions[entity->id].y -= VELOCITY;
			}
			else if (direction == "down")
			{
				entity->components.positions[entity->id].y += VELOCITY;
			}
			else if (direction == "left")
			{
				entity->components.positions[entity->id].x -= VELOCITY;
			}
			else if (direction == "right")
			{
				entity->components.positions[entity->id].x += VELOCITY;
			}
		}
	});
}