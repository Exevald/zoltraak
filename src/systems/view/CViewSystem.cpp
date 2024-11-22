#include "CViewSystem.h"
#include "../../events/CEventDispatcher.h"
#include "CEntityManager.h"
#include "Utils.h"

void CViewSystem::Draw()
{
	CViewSystem::DrawField();
	CViewSystem::DrawItems();
}

void CViewSystem::DrawField()
{
	GameFieldSettings gameFieldSettings;
	sf::RectangleShape map(sf::Vector2f(float(gameFieldSettings.gameWidth), float(gameFieldSettings.gameHeight)));
	map.setFillColor(sf::Color(6, 41, 15));

	m_window.draw(map);
}

void CViewSystem::DrawItems()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesWithShapes = entityManager.GetEntitiesWithComponents<ShapeComponent>();

	for (EntityId entityId : entitiesWithShapes)
	{
		auto* shapeComp = entityManager.GetComponent<ShapeComponent>(entityId);
		if (!shapeComp)
		{
			continue;
		}

		auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);
		if (!positionComp)
		{
			continue;
		}

		auto* rotationComp = entityManager.GetComponent<RotationComponent>(entityId);
		float rotationAngle = 0.f;
		if (rotationComp)
		{
			rotationAngle = rotationComp->angle;
		}

		if (shapeComp->type == ShapeType::Rectangle)
		{
			sf::RectangleShape rectangleShape(shapeComp->size);
			rectangleShape.setFillColor(shapeComp->color);
			rectangleShape.setPosition(positionComp->x, positionComp->y);
			rectangleShape.setRotation(rotationAngle);
			m_window.draw(rectangleShape);
		}
		else if (shapeComp->type == ShapeType::Circle)
		{
			sf::CircleShape circleShape(shapeComp->radius);
			circleShape.setFillColor(shapeComp->color);
			circleShape.setPosition(positionComp->x - shapeComp->radius,
				positionComp->y - shapeComp->radius);
			circleShape.setRotation(rotationAngle);
			m_window.draw(circleShape);
		}
	}
}

void CViewSystem::DrawHeroCard()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntitySelected, [](const SEvent& event) {
		if (event.type == EventType::EntitySelected)
		{
			const auto& movedEventData = std::get<EntitySelectedEventData>(event.data);
		}
	});
}
