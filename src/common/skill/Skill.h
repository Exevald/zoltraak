#pragma once

enum class SkillType
{
	AttackSkill,
	DefenseSkill,
	MagicSkill,
};

struct Skill
{
	std::string name;
	SkillType type;
	std::string description;
	int maxLevel;
	int currentLevel;

	explicit Skill(std::string name, const SkillType& type, std::string description, int maxLevel)
		: name(std::move(name))
		, type(type)
		, description(std::move(description))
		, maxLevel(maxLevel)
		, currentLevel(0)
	{
	}

	virtual void ApplyAttack(int& attackPower) const {}
	virtual void ApplyDefense(int& defenseValue) const {}
	virtual void ApplySpell(Spell& spell) const {}

	virtual ~Skill() = default;
};