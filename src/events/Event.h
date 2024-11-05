#include "Common.h"
#include "Entity.h"
#include <string>
#include <variant>

enum class EventType
{
	EntitySelected,
	EntityMoved,
};

struct EntitySelectedEventData
{
	Entity* entity;
};

struct EntityMovedEventData
{
	Entity* entity;
	std::string direction;
};

using EventData = std::variant<EntitySelectedEventData, EntityMovedEventData>;

struct SEvent
{
	EventType type;
	EventData data;
};
