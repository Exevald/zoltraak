#include "CExperienceSystem.h"
#include "../../CGameController.h"
#include "events/CEventDispatcher.h"

void CExperienceSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::SkillLearned, [factory = m_skillFactory](const SEvent& event) {
		const auto& skillLearnedEventData = std::get<SkillLearnedEventData>(event.data);
		CExperienceSystem::LearnSkill(skillLearnedEventData.learnedSkillAreaNumber, skillLearnedEventData.learnedSkillNumber, factory);
	});
}

void CExperienceSystem::Update()
{
}

void CExperienceSystem::LearnSkill(int skillAreaNumber, int skillNumber, const CSkillFactory& skillFactory)
{
	auto selectedEntity = CGameController::GetSelectedEntityId();
	auto& entityManager = CEntityManager::GetInstance();
	auto heroSkills = entityManager.GetComponent<SkillsComponent>(selectedEntity);
	auto experienceComp = entityManager.GetComponent<ExperienceComponent>(selectedEntity);
	auto attackComp = entityManager.GetComponent<AttackComponent>(selectedEntity);
	auto defenceComp = entityManager.GetComponent<DefenceComponent>(selectedEntity);

	if (!heroSkills || !experienceComp || !attackComp || !defenceComp)
	{
		return;
	}

	if (experienceComp->availableSkillPoints > 0)
	{
		switch (skillAreaNumber)
		{
		case 0: {
			heroSkills->attackSkills[skillNumber].currentLevel++;
			skillFactory.ApplyAttackBoost(heroSkills->attackSkills, attackComp->attackValue);
			break;
		}
		case 1: {
			heroSkills->defenceSkills[skillNumber].currentLevel++;
			skillFactory.ApplyDefenseBoost(heroSkills->defenceSkills, defenceComp->defenseValue);
			break;
		}
		case 2: {
			heroSkills->spellCreationSkills[skillNumber].currentLevel++;
			break;
		}
		default:
			break;
		}

		experienceComp->availableSkillPoints--;
	}
}
