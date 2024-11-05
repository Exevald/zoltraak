#include "CGameController.h"
#include "Entity.h"
#include "movement/CMovementSystem.h"
#include <SFML/Graphics.hpp>

void CGameController::AddEntity(Entity* entity)
{
	m_entities.push_back(entity);
}

void CGameController::Init()
{
	CMovementSystem::Init();
}

void CGameController::Draw()
{
	for (Entity* entity : m_entities)
	{
		auto& shapeComp = entity->components.shapes[entity->id];
		if (shapeComp.type == ShapeType::Rectangle)
		{
			sf::RectangleShape squareShape(shapeComp.size);
			squareShape.setFillColor(shapeComp.color);
			squareShape.setPosition(entity->components.positions[entity->id].x,
				entity->components.positions[entity->id].y);
			m_window.draw(squareShape);
		}
		else if (shapeComp.type == ShapeType::Circle)
		{
			sf::CircleShape circleShape(shapeComp.radius);
			circleShape.setFillColor(shapeComp.color);
			circleShape.setPosition(entity->components.positions[entity->id].x - shapeComp.radius,
				entity->components.positions[entity->id].y - shapeComp.radius);
			m_window.draw(circleShape);
		}
	}
}

std::vector<Entity*> CGameController::GetEntitiesWithShapes()
{
	std::vector<Entity*> entitiesWithShapes;
	for (Entity* entity : m_entities)
	{
		if (!entity->components.shapes.empty())
		{
			entitiesWithShapes.push_back(entity);
		}
	}
	return entitiesWithShapes;
}

std::vector<Entity*> CGameController::GetEntitiesWithMovement()
{
	std::vector<Entity*> entitiesWithMovement;
	for (Entity* entity : m_entities)
	{
		if (entity->components.velocities.find(entity->id) != entity->components.velocities.end())
		{
			entitiesWithMovement.push_back(entity);
		}
	}
	return entitiesWithMovement;
}

void CGameController::SetSelectedEntity(Entity* entity)
{
	m_selectedEntity = entity;
}

Entity* CGameController::GetSelectedEntity() const
{
	return m_selectedEntity;
}
