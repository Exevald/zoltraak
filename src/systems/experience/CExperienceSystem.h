#pragma once

#include "skill/CSkillFactory.h"
class CExperienceSystem
{
public:
	explicit CExperienceSystem(const CSkillFactory& skillFactory)
		: m_skillFactory(skillFactory)
	{
	}

	void Init();
	static void Update();

private:
	static void LearnSkill(int skillAreaNumber, int skillNumber, const CSkillFactory& skillFactory);

	const CSkillFactory& m_skillFactory;
};
