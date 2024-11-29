#include "CAnimationSystem.h"

void CAnimationSystem::Update(float deltaTime)
{
	auto& entityManager = CEntityManager::GetInstance();

	for (auto entityId : entityManager.GetEntitiesWithComponents<AnimationComponent, PositionComponent, VelocityComponent>())
	{
		auto* animComp = entityManager.GetComponent<AnimationComponent>(entityId);
		auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);
		auto* velocityComp = entityManager.GetComponent<VelocityComponent>(entityId);
		auto* selectionComp = entityManager.GetComponent<SelectionComponent>(entityId);

		if (selectionComp && selectionComp->isSelected && velocityComp->vx == 0 && velocityComp->vy == 0)
		{
			animComp->sprite.setPosition(positionComp->x, positionComp->y);
			continue;
		}

		if (velocityComp->vx == 0 && velocityComp->vy == 0)
		{
			animComp->elapsedTime = 0.f;
			animComp->sprite.setPosition(positionComp->x, positionComp->y);
			continue;
		}

		animComp->elapsedTime += deltaTime;
		while (animComp->elapsedTime >= animComp->frameDuration)
		{
			animComp->elapsedTime -= animComp->frameDuration;
			animComp->currentFrameNumber = (animComp->currentFrameNumber + 1) % animComp->totalFrames;
		}

		int spriteX, spriteY, row;

		if (velocityComp->vy < 0)
		{
			row = 3;
			spriteX = animComp->initialSpriteX + animComp->currentFrameNumber * (animComp->frameSize.x + 6);
			spriteY = animComp->initialSpriteY + row * (animComp->frameSize.y + 8) - 2;
		}
		else if (velocityComp->vy > 0)
		{
			row = 0;
			spriteX = animComp->initialSpriteX + animComp->currentFrameNumber * (animComp->frameSize.x + 6);
			spriteY = animComp->initialSpriteY + row * (animComp->frameSize.y + 8) + 1;
		}
		else if (velocityComp->vx < 0)
		{
			row = 1;
			spriteX = animComp->initialSpriteX + animComp->currentFrameNumber * (animComp->frameSize.x + 6);
			spriteY = animComp->initialSpriteY + row * (animComp->frameSize.y + 8);
		}
		else if (velocityComp->vx > 0)
		{
			row = 2;
			spriteX = animComp->initialSpriteX + animComp->currentFrameNumber * (animComp->frameSize.x + 6);
			spriteY = animComp->initialSpriteY + row * (animComp->frameSize.y + 8) - 1;
		}
		animComp->sprite.setTextureRect(sf::IntRect(
			spriteX,
			spriteY,
			animComp->frameSize.x,
			animComp->frameSize.y));

		animComp->sprite.setScale(3.f, 3.f);
		animComp->sprite.setPosition(positionComp->x, positionComp->y);
	}
}
