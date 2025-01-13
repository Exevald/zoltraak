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
	InventoryActionChanged,
	InventoryStateChanged,
	InventoryItemSelected,
	InventoryItemUsed,
	ActiveInventoryCharacterChanged,
	InventoryItemEquipped,
	VendorItemBought,
	HeroItemSold,
	AttackCollied,
	FightItemUsed,
	SkillLearned,
	FightActionEnded,
	FightSpellUsed,
	EnemyStrikeCreated,
	FightEnded,
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

struct ActiveInventoryCharacterNumberChangedEventData
{
	int characterNumber;
};

struct InventoryItemEquippedEventData
{
	int ownerId;
	int itemNumber;
	ItemType type;
};

struct VendorItemBoughtEventData
{
	int itemIndex;
};

struct HeroItemSoldEventData
{
	int itemIndex;
};

struct AttackColliedEventData
{
	int damage;
	int parsonId;
};

struct FightItemUsedEventData
{
	int heroId;
	int itemIndex;
};

struct SkillLearnedEventData
{
	int learnedSkillAreaNumber;
	int learnedSkillNumber;
};

struct FightActionEndedEventData
{
	FightAction action;
};

struct FightSpellUsedEventData
{
	int heroId;
	int spellIndex;
};

struct EnemyStrikeCreatedEventData
{
	float damage;
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
	InventoryItemUsedEventData,
	ActiveInventoryCharacterNumberChangedEventData,
	InventoryItemEquippedEventData,
	VendorItemBoughtEventData,
	HeroItemSoldEventData,
	AttackColliedEventData,
	FightItemUsedEventData,
	SkillLearnedEventData,
	FightActionEndedEventData,
	FightSpellUsedEventData,
	EnemyStrikeCreatedEventData>;

struct SEvent
{
	EventType type;
	EventData data;
};
