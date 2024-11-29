#include "event/CEventSystem.h"
#include "Utils.h"

void CEventSystem::handleMouseClick(CGameController& gameController, const sf::Vector2f& mousePos)
{
	EntityId clickedEntity = -1;
	auto& entityManager = CEntityManager::GetInstance();

	for (auto entityId : entityManager.GetEntitiesWithComponents<AnimationComponent, PositionComponent, SelectionComponent>())
	{
		auto* animationComp = entityManager.GetComponent<AnimationComponent>(entityId);
		auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);
		auto* selectionComp = entityManager.GetComponent<SelectionComponent>(entityId);

		if (!animationComp || !positionComp || !selectionComp)
		{
			continue;
		}

		sf::FloatRect bounds = animationComp->sprite.getGlobalBounds();

		if (bounds.contains(mousePos))
		{
			clickedEntity = entityId;
			selectionComp->isSelected = true;
		}
		else
		{
			selectionComp->isSelected = false;
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
	auto& entityManager = CEntityManager::GetInstance();
	EntityId selectedEntityId = gameController.GetSelectedEntityId();
	auto* velocityComp = entityManager.GetComponent<VelocityComponent>(selectedEntityId);

	if (!velocityComp)
	{
		return;
	}

	if (selectedEntityId != -1)
	{
		std::string direction;
		switch (key)
		{
		case sf::Keyboard::A: {
			direction = "left";
			velocityComp->vx = -DefaultVelocity;
			break;
		}
		case sf::Keyboard::D: {
			direction = "right";
			velocityComp->vx = DefaultVelocity;
			break;
		}
		case sf::Keyboard::W: {
			direction = "up";
			velocityComp->vy = -DefaultVelocity;
			break;
		}
		case sf::Keyboard::S: {
			direction = "down";
			velocityComp->vy = DefaultVelocity;
			break;
		}
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
