#pragma once

#include <SFML/System/Clock.hpp>
#include <string>
#include <unordered_map>
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
	Weapon,
	Shield,
};

enum class BonusType
{
	DamageBonus,
	ArmorBonus,
	MagicBonus,
};

struct InventoryItem
{
	std::string name;
	ItemType type;
	int ownerId;
	int cost;

	explicit InventoryItem()
		: name("Healing potion")
		, type(ItemType::HealingPotion)
		, ownerId(-1)
		, cost(0)
	{
	}

	explicit InventoryItem(std::string name, ItemType type, int ownerId, int cost)
		: name(std::move(name))
		, type(type)
		, ownerId(ownerId)
		, cost(cost)
	{
	}

	virtual ~InventoryItem() = default;
};

struct HealPotionItem : InventoryItem
{
	int healValue;

	explicit HealPotionItem(int ownerId, const std::string& potionName, int healValue, int cost)
		: InventoryItem(potionName, ItemType::HealingPotion, ownerId, cost)
		, healValue(healValue)
	{
	}
};

struct ManaPotionItem : InventoryItem
{
	int manaValue;

	explicit ManaPotionItem(int ownerId, const std::string& potionName, int manaValue, int cost)
		: InventoryItem(potionName, ItemType::ManaPotion, ownerId, cost)
		, manaValue(manaValue)
	{
	}
};

struct DamagePotionItem : InventoryItem
{
	int damageBonus;

	explicit DamagePotionItem(int ownerId, const std::string& potionName, int damageBonus, int cost)
		: InventoryItem(potionName, ItemType::DamagePotion, ownerId, cost)
		, damageBonus(damageBonus)
	{
	}
};

struct DefensePotionItem : InventoryItem
{
	int armorBonus;

	explicit DefensePotionItem(int ownerId, const std::string& potionName, int armorBonus, int cost)
		: InventoryItem(potionName, ItemType::DefensePotion, ownerId, cost)
		, armorBonus(armorBonus)
	{
	}
};

struct WeaponItem : InventoryItem
{
	std::unordered_map<BonusType, int> bonuses;

	explicit WeaponItem(int ownerId, const std::string& weaponName, const std::unordered_map<BonusType, int>& bonuses, int cost)
		: InventoryItem(weaponName, ItemType::Weapon, ownerId, cost)
		, bonuses(bonuses)
	{
	}
};

struct ShieldItem : InventoryItem
{
	std::unordered_map<BonusType, int> bonuses;

	explicit ShieldItem(int ownerId, const std::string& shieldName, const std::unordered_map<BonusType, int>& bonuses, int cost)
		: InventoryItem(shieldName, ItemType::Shield, ownerId, cost)
		, bonuses(bonuses)
	{
	}
};