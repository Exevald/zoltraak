#pragma once

#include <string>

enum class SpellType
{
	DamageSpell,
	HealSpell,
	BuffSpell,
};

struct Spell
{
	int ownerId;
	std::string name;
	SpellType type;
	int manaCost;
	int level;

	explicit Spell()
		: ownerId(-1)
		, name("New spell")
		, type(SpellType::DamageSpell)
		, manaCost(30)
		, level(1)
	{
	}

	explicit Spell(int ownerId, std::string name, SpellType type, int manaCost, int level)
		: ownerId(ownerId)
		, name(std::move(name))
		, type(type)
		, manaCost(manaCost)
		, level(level)
	{
	}

	virtual ~Spell() = default;
};

struct HealSpell : Spell
{
	int healValue;

	explicit HealSpell(int ownerId, const std::string& name, int manaConst, int healValue, int level)
		: Spell(ownerId, name, SpellType::HealSpell, manaConst, level)
		, healValue(healValue)
	{
	}
};

struct DamageSpell : Spell
{
	int damageValue;

	explicit DamageSpell(int ownerId, const std::string& name, int manaCost, int damageValue, int level)
		: Spell(ownerId, name, SpellType::DamageSpell, manaCost, level)
		, damageValue(damageValue)
	{
	}
};