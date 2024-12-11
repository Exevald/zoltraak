#include "movement/CMovementSystem.h"
#include "../CGameController.h"
#include "Utils.h"
#include "events/CEventDispatcher.h"

void CMovementSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::EntityMoved, [](const SEvent& event) {
		if (event.type == EventType::EntityMoved)
		{
			const auto& movedEventData = std::get<EntityMovedEventData>(event.data);
			EntityId entityId = movedEventData.id;
			const std::string& direction = movedEventData.direction;

			HandleEntityMoved(entityId, direction);
		}
	});
}

void CMovementSystem::HandleEntityMoved(EntityId movedEntityId, const std::string& direction)
{
	auto& entityManager = CEntityManager::GetInstance();

	auto* positionComponent = entityManager.GetComponent<PositionComponent>(movedEntityId);
	auto* velocityComponent = entityManager.GetComponent<VelocityComponent>(movedEntityId);
	auto* animationComponent = entityManager.GetComponent<AnimationComponent>(movedEntityId);
	auto* imageComponent = entityManager.GetComponent<ImageComponent>(movedEntityId);
	auto* fightSoulComponent = entityManager.GetComponent<FightSoulComponent>(movedEntityId);

	const BattleAreaSettings battleAreaSettings;

	auto level = CLevelGenerator::GetLevel("level1.txt");

	if (!positionComponent || (!animationComponent && !imageComponent))
	{
		return;
	}

	float newX = positionComponent->x;
	float newY = positionComponent->y;
	if (direction == "up" || direction == "down")
	{
		newY += velocityComponent->vy;
	}
	else if (direction == "left" || "right")
	{
		newX += velocityComponent->vx;
	}

	const float baseTileSize = 20.0f;
	const float scaleFactor = 3.0f;
	const float scaledTileSize = baseTileSize * scaleFactor;

	int tileX = static_cast<int>(newX / scaledTileSize);
	int tileY = static_cast<int>(newY / scaledTileSize);

	char tile = level[tileY][tileX];
//
//	if (tile == '5' || tile == '6' || tile == '7' || tile == '8' || tile == '9' || tile == '0')
//	{
//		return;
//	}

	if (animationComponent)
	{
		newX = std::max(0.f, std::min(newX, float(CGameController::GetWindowSizeSettings().x) - animationComponent->sprite.getScale().x * 18));
		newY = std::max(0.f, std::min(newY, float(CGameController::GetWindowSizeSettings().y / 3) - animationComponent->sprite.getScale().y * 37));
	}
	if (imageComponent)
	{
		newX = std::max(0.f, std::min(newX, float(CGameController::GetWindowSizeSettings().x) - 22));
		newY = std::max(0.f, std::min(newY, float(CGameController::GetWindowSizeSettings().y) - 22));
	}
	if (fightSoulComponent)
	{
		newX = std::max(0.f, std::min(newX, 777 + float(battleAreaSettings.areaWidth * 2) - 22));
		newY = std::max(0.f, std::min(newY, 305 + float(battleAreaSettings.areaHeight * 2) - 22));
	}
	if (fightSoulComponent)
	{
		newX = std::clamp(newX, 785.f, 785.f + 142.f * 2 - 28.f);
		newY = std::clamp(newY, 314.f, 314.f + 141.f * 2 - 28.f);
	}

	positionComponent->x = newX;
	positionComponent->y = newY;
}
