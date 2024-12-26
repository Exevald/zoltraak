#pragma once

#include "skill/Skill.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class CSkillFactory
{
public:
	template <typename T>
	void RegisterSkill(const std::string& name, std::function<std::unique_ptr<T>(int)> creator)
	{
		if (m_creators.find(name) != m_creators.end())
		{
			throw std::runtime_error("Talent with name '" + name + "' is already registered.");
		}

		m_creators[name] = [creator](int ownerId) -> std::unique_ptr<Skill> {
			return creator(ownerId);
		};

		m_registeredTypes[name] = typeid(T).hash_code();
	}

	template <typename T>
	std::unique_ptr<T> CreateTalent(const std::string& name, int ownerId) const
	{
		auto it = m_creators.find(name);
		if (it != m_creators.end())
		{
			if (m_registeredTypes.at(name) != typeid(T).hash_code())
			{
				throw std::runtime_error("Type mismatch for talent '" + name + "'.");
			}

			return std::unique_ptr<T>(static_cast<T*>(it->second(ownerId).release()));
		}
		throw std::runtime_error("Talent '" + name + "' not found in the factory.");
	}

	void ApplyAttackBoost(const std::vector<std::unique_ptr<Skill>>& talents, int& attackPower) const
	{
		for (const auto& talent : talents)
		{
			if (talent->type == SkillType::AttackSkill && talent->currentLevel > 0)
			{
				talent->ApplyAttack(attackPower);
			}
		}
	}

	void ApplyDefenseBoost(const std::vector<std::unique_ptr<Skill>>& talents, int& defenseValue) const
	{
		for (const auto& talent : talents)
		{
			if (talent->type == SkillType::DefenseSkill && talent->currentLevel > 0)
			{
				talent->ApplyDefense(defenseValue);
			}
		}
	}

	void ApplySpellBoost(const std::vector<std::unique_ptr<Skill>>& talents, Spell& spell) const
	{
		for (const auto& talent : talents)
		{
			if (talent->type == SkillType::MagicSkill && talent->currentLevel > 0)
			{
				talent->ApplySpell(spell);
			}
		}
	}

	~CSkillFactory() = default;

private:
	std::unordered_map<std::string, std::function<std::unique_ptr<Skill>(int)>> m_creators;
	std::unordered_map<std::string, size_t> m_registeredTypes;
};
