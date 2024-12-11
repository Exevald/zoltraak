#pragma once

#include "CEntityManager.h"
#include "actions/Actions.h"
#include <string>
#include <variant>

enum class EventType
{
	EntitySelected,
	EntityMoved,
	FightActionSelected,
	ActiveFightCharacterUpdated,
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

struct FightActionSelectedEventData
{
	EntityId id;
	FightAction selectedAction;
};

struct ActiveFightCharacterUpdatedEventData
{
	EntityId id;
};

using EventData = std::variant<EntitySelectedEventData, EntityMovedEventData, FightActionSelectedEventData, ActiveFightCharacterUpdatedEventData>;

struct SEvent
{
	EventType type;
	EventData data;
};
