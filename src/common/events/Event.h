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
	InventoryItemCreated,
	InventoryActionChanged,
	InventoryStateChanged,
	InventoryItemSelected,
	InventoryItemUsed
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

struct InventoryItemCreatedEventData
{
	EntityId id;
	InventoryItem inventoryItem;
};

struct InventoryActionChangedEventData
{
	InventoryAction selectedAction;
};

struct InventoryStateChangedEventData
{
	InventoryState changedState;
};

struct InventoryItemSelectedEventData
{
	int inventoryItemNumber;
};

struct InventoryItemUsedEventData
{
	int inventoryItemNumber;
	InventoryAction usageAction;
};

using EventData = std::variant<
	EntitySelectedEventData,
	EntityMovedEventData,
	FightActionSelectedEventData,
	ActiveFightCharacterUpdatedEventData,
	InventoryItemCreatedEventData,
	InventoryActionChangedEventData,
	InventoryStateChangedEventData,
	InventoryItemSelectedEventData,
	InventoryItemUsedEventData>;

struct SEvent
{
	EventType type;
	EventData data;
};
