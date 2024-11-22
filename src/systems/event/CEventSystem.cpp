#include "event/CEventSystem.h"

void CEventSystem::handleMouseClick(CGameController& gameController, const sf::Vector2f& mousePos)
{
	EntityId clickedEntity = -1;
	auto& entityManager = CEntityManager::GetInstance();

	for (auto entityId : entityManager.GetEntitiesWithComponents<ShapeComponent, PositionComponent>())
	{
		auto* shapeComp = entityManager.GetComponent<ShapeComponent>(entityId);
		auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);
		auto* selectionComp = entityManager.GetComponent<SelectionComponent>(entityId);

		if (!shapeComp || !positionComp || !selectionComp)
		{
			continue;
		}

		sf::FloatRect bounds;

		if (shapeComp->type == ShapeType::Rectangle)
		{
			bounds = sf::FloatRect(positionComp->x, positionComp->y,
				shapeComp->size.x, shapeComp->size.y);
		}
		else if (shapeComp->type == ShapeType::Circle)
		{
			bounds = sf::FloatRect(positionComp->x - shapeComp->radius,
				positionComp->y - shapeComp->radius,
				shapeComp->radius * 2, shapeComp->radius * 2);
		}

		if (bounds.contains(mousePos))
		{
			clickedEntity = entityId;
			break;
		}
	}

	EntitySelectedEventData selectedData{};
	selectedData.id = clickedEntity;

	SEvent entitySelectedEvent;
	entitySelectedEvent.type = EventType::EntitySelected;
	entitySelectedEvent.data = selectedData;

	CEventDispatcher::GetInstance().Dispatch(entitySelectedEvent);

	gameController.SetSelectedEntityId(clickedEntity);
}

void CEventSystem::handleKeyPress(CGameController& gameController, sf::Keyboard::Key key)
{
	EntityId selectedEntityId = gameController.GetSelectedEntityId();
	if (selectedEntityId != -1)
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

		if (!direction.empty())
		{
			EntityMovedEventData entityMovedEventData{};
			entityMovedEventData.id = selectedEntityId;
			entityMovedEventData.direction = direction;

			SEvent entityMovedEvent;
			entityMovedEvent.type = EventType::EntityMoved;
			entityMovedEvent.data = entityMovedEventData;

			CEventDispatcher::GetInstance().Dispatch(entityMovedEvent);
		}
	}
}
