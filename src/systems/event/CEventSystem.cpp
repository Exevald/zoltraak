#include "event/CEventSystem.h"
#include "../events/CEventDispatcher.h"

void CEventSystem::handleMouseClick(CGameController& gameController, const sf::Vector2f& mousePos)
{
	Entity* clickedEntity = nullptr;

	for (Entity* entity : gameController.GetEntitiesWithShapes())
	{
		auto& shapeComp = entity->components.shapes[entity->id];
		sf::FloatRect bounds;

		if (shapeComp.type == ShapeType::Rectangle)
		{
			bounds = sf::FloatRect(entity->components.positions[entity->id].x,
				entity->components.positions[entity->id].y,
				shapeComp.size.x, shapeComp.size.y);
		}
		else if (shapeComp.type == ShapeType::Circle)
		{
			bounds = sf::FloatRect(entity->components.positions[entity->id].x - shapeComp.radius,
				entity->components.positions[entity->id].y - shapeComp.radius,
				shapeComp.radius * 2, shapeComp.radius * 2);
		}

		if (bounds.contains(mousePos))
		{
			clickedEntity = entity;
			break;
		}
	}
	EntitySelectedEventData selectedData{};
	selectedData.entity = clickedEntity;

	SEvent entitySelectedEvent;
	entitySelectedEvent.type = EventType::EntitySelected;
	entitySelectedEvent.data = selectedData;

	CEventDispatcher::GetInstance().Dispatch(entitySelectedEvent);

	gameController.SetSelectedEntity(clickedEntity);
}

void CEventSystem::handleKeyPress(CGameController& gameController, sf::Keyboard::Key key)
{
	auto selectedEntity = gameController.GetSelectedEntity();
	if (selectedEntity)
	{
		std::string direction;
		switch (key)
		{
		case sf::Keyboard::A:
			direction = "left";
			break;
		case sf::Keyboard::D:
			direction = "right";
			break;
		case sf::Keyboard::W:
			direction = "up";
			break;
		case sf::Keyboard::S:
			direction = "down";
			break;
		default:
			break;
		}
		EntityMovedEventData entityMovedEventData{};
		entityMovedEventData.entity = selectedEntity;
		entityMovedEventData.direction = direction;

		SEvent entityMovedEvent;
		entityMovedEvent.type = EventType::EntityMoved;
		entityMovedEvent.data = entityMovedEventData;

		CEventDispatcher::GetInstance().Dispatch(entityMovedEvent);
	}
}