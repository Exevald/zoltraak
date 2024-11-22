#pragma once

#include "../events/CEventDispatcher.h"
#include <SFML/Graphics.hpp>
#include <string>

class CCollisionSystem
{
public:
	static void Init();
private:
	static void HandleEntityMoved(EntityId movedEntityId, const std::string& direction);
	static bool CheckRectangleCollision(const sf::FloatRect& entityBoundary1, const sf::FloatRect& entityBoundary2);
	static void HandleCollision(EntityId movedEntityId, EntityId otherEntityId, const std::string& direction);
	static float CalculateCollisionAngle(EntityId movedEntityId, EntityId otherEntityId);
	static sf::Vector2f CalculatePenetrationDepth(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
};
