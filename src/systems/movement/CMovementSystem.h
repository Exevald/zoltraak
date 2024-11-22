#pragma once

#include "CEntityManager.h"

class CMovementSystem
{
public:
	static void Init();
private:
	static void HandleEntityMoved(EntityId movedEntityId, const std::string& direction);
};
