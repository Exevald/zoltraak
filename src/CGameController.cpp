#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "systems/MovementSystem.h"
#include "CGameController.h"

void CGameController::AddEntity(Entity* entity)
{
	m_entities.push_back(entity);
}

void CGameController::Update()
{
	for (Entity* entity: m_entities)
	{
		MovementSystem::update(*entity);
	}
}

void CGameController::Draw()
{
	for (Entity* entity: m_entities)
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

void CGameController::handleClick(const sf::Vector2f& mousePos)
{
	for (Entity* entity: m_entities)
	{
		auto& shapeComp = entity->components.shapes[entity->id];
		sf::FloatRect bounds;

		if (shapeComp.type == ShapeType::Rectangle)
		{
			bounds = sf::FloatRect(entity->components.positions[entity->id].x,
					entity->components.positions[entity->id].y, shapeComp.size.x, shapeComp.size.y);
		}
		else if (shapeComp.type == ShapeType::Circle)
		{
			bounds = sf::FloatRect(entity->components.positions[entity->id].x - shapeComp.radius,
					entity->components.positions[entity->id].y - shapeComp.radius,
					shapeComp.radius * 2, shapeComp.radius * 2);
		}

		if (bounds.contains(mousePos))
		{
			m_selectedEntity = entity;
			break;
		}
	}
}

Entity* CGameController::GetSelectedEntity() const
{
	return m_selectedEntity;
}
