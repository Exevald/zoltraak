#pragma once

#include "../Entity.h"

class MovementSystem
{
public:
	static void update(Entity& entity)
	{
		entity.components.positions[entity.id].x += entity.components.velocities[entity.id].x;
		entity.components.positions[entity.id].y += entity.components.velocities[entity.id].y;
	}
};
