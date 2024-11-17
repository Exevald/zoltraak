#include "CViewSystem.h"
#include "CEntityManager.h"

void CViewSystem::Draw()
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

		if (shapeComp->type == ShapeType::Rectangle)
		{
			sf::RectangleShape rectangleShape(shapeComp->size);
			rectangleShape.setFillColor(shapeComp->color);
			rectangleShape.setPosition(positionComp->x, positionComp->y);
			m_window.draw(rectangleShape);
		}
		else if (shapeComp->type == ShapeType::Circle)
		{
			sf::CircleShape circleShape(shapeComp->radius);
			circleShape.setFillColor(shapeComp->color);
			circleShape.setPosition(positionComp->x - shapeComp->radius,
				positionComp->y - shapeComp->radius);
			m_window.draw(circleShape);
		}
	}
}