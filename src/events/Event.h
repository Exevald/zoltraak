#pragma once

#include "CEntityManager.h"
#include <string>
#include <variant>

enum class EventType
{
	EntitySelected,
	EntityMoved,
};

struct EntitySelectedEventData
{
	EntityId id;
};

struct EntityMovedEventData
{
	EntityId id;
	std::string direction;
};

using EventData = std::variant<EntitySelectedEventData, EntityMovedEventData>;

struct SEvent
{
	EventType type;
	EventData data;
};
