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
			throw std::runtime_error("Skill with name '" + name + "' is already registered.");
		}

		m_creators[name] = [creator](int ownerId) -> std::unique_ptr<Skill> {
			return creator(ownerId);
		};

		m_registeredTypes[name] = typeid(T).hash_code();
	}

	template <typename T>
	std::unique_ptr<T> CreateSkill(const std::string& name, int ownerId) const
	{
		auto it = m_creators.find(name);
		if (it != m_creators.end())
		{
			if (m_registeredTypes.at(name) != typeid(T).hash_code())
			{
				throw std::runtime_error("Type mismatch for skill '" + name + "'.");
			}

			return std::unique_ptr<T>(static_cast<T*>(it->second(ownerId).release()));
		}
		throw std::runtime_error("Skill '" + name + "' not found in the factory.");
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

	void AddSpellsFromTalents(std::vector<std::unique_ptr<Skill>>& skills, std::vector<std::unique_ptr<Spell>>& spellBook)
	{
		for (const auto& skill : skills)
		{
			if (auto newSpell = skill->CreateNewSpell())
			{
				spellBook.push_back(std::move(newSpell));
			}
		}
	}

	~CSkillFactory() = default;

private:
	std::unordered_map<std::string, std::function<std::unique_ptr<Skill>(int)>> m_creators;
	std::unordered_map<std::string, size_t> m_registeredTypes;
};
