#include "CAnimationSystem.h"
#include "../../CGameController.h"

void CAnimationSystem::Update()
{
	auto& entityManager = CEntityManager::GetInstance();

	for (auto entityId : entityManager.GetEntitiesWithComponents<AnimationComponent, PositionComponent>())
	{
		auto* animComp = entityManager.GetComponent<AnimationComponent>(entityId);
		auto* positionComp = entityManager.GetComponent<PositionComponent>(entityId);
		auto* velocityComp = entityManager.GetComponent<VelocityComponent>(entityId);

		if (!animComp || !positionComp || !velocityComp)
		{
			continue;
		}

		if (animComp->currentAnimation.empty() || (velocityComp->vx == 0 && velocityComp->vy == 0 && CGameController::GetCurrentGameState() == CurrentState::Player && animComp->currentAnimation != "idle"))
		{
			continue;
		}

		auto& animData = animComp->animations[animComp->currentAnimation];
		animComp->elapsedTime += CGameController::GetDeltaTime();

		if (animComp->elapsedTime >= animData.frameDuration)
		{
			animComp->elapsedTime -= animData.frameDuration;
			animComp->currentFrameNumber++;

			if (animComp->currentFrameNumber >= animData.totalFrames)
			{
				if (animData.isLooping)
				{
					animComp->currentFrameNumber = 0;
				}
				else
				{
					animComp->currentFrameNumber = animData.totalFrames - 1;
					animComp->isAnimationFinished = true;

					if (animComp->currentAnimation == "attack")
					{
						animComp->SetAnimation("idle");
						continue;
					}
				}
			}
		}

		int spriteX = animData.initialSpriteX + animComp->currentFrameNumber * (animData.frameSize.x + 6);
		int spriteY = animData.initialSpriteY;

		animComp->sprite.setTextureRect(sf::IntRect(spriteX, spriteY, animData.frameSize.x, animData.frameSize.y));
		animComp->sprite.setPosition(positionComp->x, positionComp->y);
	}
}
