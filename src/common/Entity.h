#pragma once

#include "../components/Components.h"

class Entity
{
public:
	EntityID id;
	Components components;

	Entity(EntityID id, float pos_x, float pos_y, float vel_x, float vel_y)
	{
		this->id = id;
		components.positions[id] = { pos_x, pos_y };
		components.velocities[id] = { vel_x, vel_y };
	}
};
