#include "CAnimationSystem.h"
#include "../../CGameController.h"

void CAnimationSystem::Update()
{
	auto& entityManager = CEntityManager::GetInstance();

	for (auto entityId : entityManager.GetEntitiesWithComponents<AnimationComponent, PositionComponent>())
	{
		auto* animComp = entityManager.GetComponent<AnimationComponent>(entityId);
		auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);

		if (animComp->currentAnimation.empty())
		{
			continue;
		}

		auto& animData = animComp->animations[animComp->currentAnimation];
		animComp->elapsedTime += CGameController::GetDeltaTime();

		while (animComp->elapsedTime >= animData.frameDuration)
		{
			animComp->elapsedTime -= animData.frameDuration;
			animComp->currentFrameNumber = (animComp->currentFrameNumber + 1) % animData.totalFrames;
		}

		int spriteX = animData.initialSpriteX + animComp->currentFrameNumber * (animData.frameSize.x + 6);
		int spriteY = animData.initialSpriteY;

		animComp->sprite.setTextureRect(sf::IntRect(spriteX, spriteY, animData.frameSize.x, animData.frameSize.y));
		animComp->sprite.setPosition(positionComp->x, positionComp->y);
	}
}
