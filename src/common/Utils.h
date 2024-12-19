#pragma once

#include <string>
#include <utility>

const float DefaultVelocity = 8.f;

struct WindowSettings
{
	unsigned int windowWidth = 1920;
	unsigned int windowHeight = 1080;
	std::string windowTitle = "Zoltraak";
};

struct BattleAreaSettings
{
	int areaWidth = 150;
	int areaHeight = 149;
};

enum class ItemType
{
	HealingPotion,
	ManaPotion,
	DamagePotion,
	DefensePotion,
};

struct InventoryItem
{
	std::string name;
	ItemType type;
	int ownerId;

	InventoryItem()
		: name("Healing potion")
		, type(ItemType::HealingPotion)
		, ownerId(-1)
	{
	}

	InventoryItem(std::string name, ItemType type, int ownerId)
		: name(std::move(name))
		, type(type)
		, ownerId(ownerId)
	{
	}
};