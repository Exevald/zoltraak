#include "CCollisionSystem.h"
#include "../../CGameController.h"
#include "Utils.h"

void CCollisionSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntityMoved, [](const SEvent& event) {
		if (event.type == EventType::EntityMoved)
		{
			const auto& movedEventData = std::get<EntityMovedEventData>(event.data);
			HandleEntityMoved(movedEventData.id, movedEventData.direction);
		}
	});
}

void CCollisionSystem::HandleEntityMoved(EntityId movedEntityId, const std::string& direction)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto level = CLevelGenerator::GetLevel("level1.txt");

	auto* positionComp = entityManager.GetComponent<PositionComponent>(movedEntityId);
	auto* animationComp = entityManager.GetComponent<AnimationComponent>(movedEntityId);

	if (!positionComp || !animationComp)
	{
		return;
	}

	auto entities = entityManager.GetAllEntities();
	for (EntityId otherEntityId : entities)
	{
		if (movedEntityId == otherEntityId)
		{
			continue;
		}

		auto* collisionComp = entityManager.GetComponent<CollisionComponent>(otherEntityId);
		auto* otherPositionComp = entityManager.GetComponent<PositionComponent>(otherEntityId);

		if (!collisionComp || !otherPositionComp)
		{
			continue;
		}

		if (CheckRectangleCollision(
				{ positionComp->x, positionComp->y, animationComp->sprite.getScale().x * 18 / 1.5f, animationComp->sprite.getScale().y * 37 / 1.5f },
				{ otherPositionComp->x, otherPositionComp->y, 60.f, 60.f }))
		{
			HandleCollision(movedEntityId, otherEntityId, direction);
		}
	}
}

bool CCollisionSystem::CheckRectangleCollision(const sf::FloatRect& entityBoundary1, const sf::FloatRect& entityBoundary2)
{
	return entityBoundary1.intersects(entityBoundary2);
}

void CCollisionSystem::HandleCollision(EntityId movedEntityId, EntityId otherEntityId, const std::string& direction)
{
	auto& entityManager = CEntityManager::GetInstance();

	auto* positionComp = entityManager.GetComponent<PositionComponent>(movedEntityId);
	auto* otherPositionComp = entityManager.GetComponent<PositionComponent>(otherEntityId);
	auto* velocityComp = entityManager.GetComponent<VelocityComponent>(movedEntityId);
	auto* otherCollisionComp = entityManager.GetComponent<CollisionComponent>(otherEntityId);
	auto* animationComp = entityManager.GetComponent<AnimationComponent>(movedEntityId);

	if (!otherCollisionComp || !otherPositionComp)
	{
		return;
	}

	sf::FloatRect movedRect = { positionComp->x, positionComp->y, animationComp->sprite.getScale().x * 18, animationComp->sprite.getScale().y * 37 / 1.5f };
	if (direction == "up")
	{
		movedRect.top /= 2;
		movedRect.height /= 2;
	}

	sf::FloatRect otherRect = { otherPositionComp->x, otherPositionComp->y, 60.f, 60.f };

	sf::Vector2f penetrationDepth = CalculatePenetrationDepth(movedRect, otherRect);

	if (otherCollisionComp->type == CollisionType::MovableObject)
	{
		if (std::abs(penetrationDepth.x) < std::abs(penetrationDepth.y))
		{
			otherPositionComp->x += penetrationDepth.x;
		}
		else
		{
			otherPositionComp->y += penetrationDepth.y;
		}
	}

	float objectMass = 1.0f;
	if (auto* massComp = entityManager.GetComponent<MassComponent>(otherEntityId))
	{
		objectMass = massComp->mass;
	}

	float adjustedVx = velocityComp->vx / objectMass;
	float adjustedVy = velocityComp->vy / objectMass;
	float newX = otherPositionComp->x;
	float newY = otherPositionComp->y;

	if (direction == "up" || direction == "down")
	{
		newY += adjustedVy;
	}
	else if (direction == "left" || direction == "right")
	{
		newX += adjustedVx;
	}

	newX = std::max(0.f, std::min(newX, float(CGameController::GetGameSizeSettings().x) - 20));
	newY = std::max(0.f, std::min(newY, float(CGameController::GetGameSizeSettings().y) - 20));

	otherPositionComp->x = newX;
	otherPositionComp->y = newY;
}

sf::Vector2f CCollisionSystem::CalculatePenetrationDepth(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	float overlapX = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - std::max(rect1.left, rect2.left);
	float overlapY = std::min(rect1.top + rect1.height, rect2.top + rect2.height) - std::max(rect1.top, rect2.top);

	sf::Vector2f penetrationDepth(0.f, 0.f);

	if (overlapX > 0 && overlapY > 0)
	{
		if (overlapX < overlapY)
		{
			penetrationDepth.x = rect1.left < rect2.left ? -overlapX : overlapX;
		}
		else
		{
			penetrationDepth.y = rect1.top < rect2.top ? -overlapY : overlapY;
		}
	}

	return penetrationDepth;
}
