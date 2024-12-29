#pragma once

#include "spells/CSpellFactory.h"

enum class  SkillType
{
	AttackSkill,
	DefenseSkill,
	SpellCreationSkill,
};

struct Skill
{
	std::string name;
	SkillType type;
	std::string description;
	int maxLevel;
	int currentLevel;
	int ownerId;

	explicit Skill(std::string name, const SkillType& type, std::string description, int maxLevel, int ownerId)
		: name(std::move(name))
		, type(type)
		, description(std::move(description))
		, maxLevel(maxLevel)
		, currentLevel(0)
		, ownerId(ownerId)
	{
	}

	virtual void ApplyAttack(int& attackPower) const {}
	virtual void ApplyDefense(int& defenseValue) const {}
	[[nodiscard]] virtual std::unique_ptr<Spell> CreateNewSpell() const { return nullptr; }

	virtual ~Skill() = default;
};

struct AttackSkill : public Skill
{
	int damageIncrease;

	explicit AttackSkill(int ownerId, const std::string& name, const std::string& description, int maxLevel, int damageIncrease)
		: Skill(name, SkillType::AttackSkill, description, maxLevel, ownerId)
		, damageIncrease(damageIncrease)
	{
	}

	void ApplyAttack(int& attackPower) const override
	{
		attackPower += currentLevel * damageIncrease;
	}
};

struct DefenceSkill : public Skill
{
	int defenceIncrease;

	explicit DefenceSkill(int ownerId, const std::string& name, const std::string& description, int maxLevel, int defenceIncrease)
		: Skill(name, SkillType::DefenseSkill, description, maxLevel, ownerId)
		, defenceIncrease(defenceIncrease)
	{
	}

	void ApplyDefense(int& defenceValue) const override
	{
		defenceValue += currentLevel * defenceIncrease;
	}
};

struct SpellCreationSkill : public Skill
{
	std::string spellName;
	CSpellFactory& spellFactory;

	explicit SpellCreationSkill(int ownerId, const std::string& name, const std::string& description, int maxLevel, CSpellFactory& spellFactory)
		: Skill(name, SkillType::SpellCreationSkill, description, maxLevel, ownerId)
		, spellFactory(spellFactory)
	{
	}

	[[nodiscard]] std::unique_ptr<Spell> CreateNewSpell() const override
	{
		if (currentLevel >= maxLevel)
		{
			return spellFactory.CreateSpell<Spell>(spellName, ownerId);
		}
		return nullptr;
	}
};