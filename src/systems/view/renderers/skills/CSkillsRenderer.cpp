#include "CSkillsRenderer.h"
#include "../../../../CGameController.h"
#include "assets_storage/CFontStorage.h"
#include "assets_storage/CTextureStorage.h"

static auto& font = CFontStorage::GetFont("8bitoperator_jve.ttf");
const int TextSize = 40;
const int SkillTextSize = 35;

void CSkillsRenderer::Init()
{
}

void CSkillsRenderer::Draw(sf::RenderWindow& window)
{
	if (CGameController::GetCurrentGameState() == CurrentState::Skills && CGameController::GetSelectedEntityId() != -1)
	{
		sf::View view = window.getDefaultView();
		window.setView(view);

		CSkillsRenderer::DrawSkillsAreas(window);
		CSkillsRenderer::DrawCurrentSkillInfo(window);
		CSkillsRenderer::DrawSkills(window);
		CSkillsRenderer::DrawSkillDescription(window);
		CSkillsRenderer::DrawHeroInfo(window);
	}
}

void CSkillsRenderer::DrawSkillsAreas(sf::RenderWindow& window)
{
	const float skillAreaTopMargin = 200;
	const sf::Vector2f skillAreaSize = { 350, 450 };
	m_skills.skillsAreas.clear();

	m_skills.skillsAreas.push_back(CSkillsRenderer::CreateSkillArea(
		"Attack",
		{ float(window.getSize().x) / 3 - 125 - skillAreaSize.x,
			float(window.getPosition().y) + skillAreaTopMargin },
		skillAreaSize));

	m_skills.skillsAreas.push_back(CSkillsRenderer::CreateSkillArea(
		"Defence",
		{ float(window.getSize().x) * 2 / 3 - 125 - skillAreaSize.x,
			float(window.getPosition().y) + skillAreaTopMargin },
		skillAreaSize));

	m_skills.skillsAreas.push_back(CSkillsRenderer::CreateSkillArea(
		"Magic",
		{ float(window.getSize().x) - 125 - skillAreaSize.x,
			float(window.getPosition().y) + skillAreaTopMargin },
		skillAreaSize));

	for (int i = 0; i < m_skills.skillsAreas.size(); i++)
	{
		auto& skillArea = m_skills.skillsAreas[i];
		if (i == CGameController::GetCurrentSkillAreaNumber())
		{
			skillArea.skillAreaBorder.setOutlineColor(sf::Color::Yellow);
			skillArea.title.setFillColor(sf::Color::Yellow);
		}

		window.draw(skillArea.skillAreaBorder);
		window.draw(skillArea.title);
	}
}

void CSkillsRenderer::DrawCurrentSkillInfo(sf::RenderWindow& window)
{
	m_skills.selectedSkillInfoArea.setTexture(CTextureStorage::GetTexture("utils_windows.png"));
	m_skills.selectedSkillInfoArea.setTextureRect(sf::IntRect(544, 21, 289, 76));
	m_skills.selectedSkillInfoArea.setScale(3.15f, 2.3f);
	m_skills.selectedSkillInfoArea.setPosition(float(window.getSize().x) / 2 - 289 * 1.5f, float(window.getSize().y) - 250);

	m_skills.selectedSkillInfoText.setFont(font);
	m_skills.selectedSkillInfoText.setCharacterSize(40);

	window.draw(m_skills.selectedSkillInfoArea);
	window.draw(m_skills.selectedSkillInfoText);
}

void CSkillsRenderer::DrawSkills(sf::RenderWindow& window)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto heroSkills = entityManager.GetComponent<SkillsComponent>(CGameController::GetSelectedEntityId());
	if (!heroSkills)
	{
		return;
	}
	m_skills.allSkillsInfo.clear();

	float yOffset = 0;
	for (const auto& attackSkill : heroSkills->attackSkills)
	{
		int attackSkillsIndex = 0;
		sf::Text attackSkillInfo;
		attackSkillInfo.setFont(font);
		attackSkillInfo.setCharacterSize(SkillTextSize);
		attackSkillInfo.setString(attackSkill.name + " " + std::to_string(attackSkill.currentLevel) + "/" + std::to_string(attackSkill.maxLevel));
		attackSkillInfo.setPosition(m_skills.skillsAreas[attackSkillsIndex].skillAreaBorder.getPosition().x + 30, m_skills.skillsAreas[attackSkillsIndex].skillAreaBorder.getPosition().y + 50 + yOffset);

		m_skills.allSkillsInfo[attackSkillsIndex].push_back(attackSkillInfo);
		yOffset += 50;
	}

	yOffset = 0;
	for (const auto& defenceSkill : heroSkills->defenceSkills)
	{
		int defenceSkillsIndex = 1;
		sf::Text defenceSkillInfo;
		defenceSkillInfo.setFont(font);
		defenceSkillInfo.setCharacterSize(SkillTextSize);
		defenceSkillInfo.setString(defenceSkill.name + " " + std::to_string(defenceSkill.currentLevel) + "/" + std::to_string(defenceSkill.maxLevel));
		defenceSkillInfo.setPosition(m_skills.skillsAreas[defenceSkillsIndex].skillAreaBorder.getPosition().x + 30, m_skills.skillsAreas[defenceSkillsIndex].skillAreaBorder.getPosition().y + 50 + yOffset);

		m_skills.allSkillsInfo[defenceSkillsIndex].push_back(defenceSkillInfo);
		yOffset += 50;
	}

	for (const auto& skillsInfo : m_skills.allSkillsInfo)
	{
		for (int i = 0; i < skillsInfo.second.size(); i++)
		{
			if (CGameController::GetCurrentSkillNumber() > skillsInfo.second.size() - 1)
			{
				CGameController::SetCurrentSkillNumber(int(skillsInfo.second.size()) - 1);
			}

			auto skill = skillsInfo.second[i];
			const auto isCurrentSkillsStateCorrect = CGameController::GetCurrentSkillsState() == SkillsState::ChoosingSkill;
			const auto isCurrentSkillAreaNumberCorrect = skillsInfo.first == CGameController::GetCurrentSkillAreaNumber();
			const auto isCurrentSkillNumberCorrect = i == CGameController::GetCurrentSkillNumber();

			if (isCurrentSkillsStateCorrect && isCurrentSkillAreaNumberCorrect && isCurrentSkillNumberCorrect)
			{
				skill.setFillColor(sf::Color::Yellow);
			}

			window.draw(skill);
		}
	}
}

void CSkillsRenderer::DrawSkillDescription(sf::RenderWindow& window)
{
	if (CGameController::GetCurrentSkillsState() != SkillsState::ChoosingSkill)
	{
		return;
	}

	auto& entityManager = CEntityManager::GetInstance();
	auto heroSkills = entityManager.GetComponent<SkillsComponent>(CGameController::GetSelectedEntityId());
	if (!heroSkills)
	{
		return;
	}

	std::vector<Skill> currentAreaSkills;
	switch (CGameController::GetCurrentSkillAreaNumber())
	{
	case 0: {
		currentAreaSkills.reserve(currentAreaSkills.size() + heroSkills->attackSkills.size());
		currentAreaSkills.insert(currentAreaSkills.end(), heroSkills->attackSkills.begin(), heroSkills->attackSkills.end());
		break;
	}
	case 1: {
		currentAreaSkills.reserve(currentAreaSkills.size() + heroSkills->defenceSkills.size());
		currentAreaSkills.insert(currentAreaSkills.end(), heroSkills->defenceSkills.begin(), heroSkills->defenceSkills.end());
		break;
	}
	case 2: {
		currentAreaSkills.reserve(currentAreaSkills.size() + heroSkills->spellCreationSkills.size());
		currentAreaSkills.insert(currentAreaSkills.end(), heroSkills->spellCreationSkills.begin(), heroSkills->spellCreationSkills.end());
		break;
	}
	default:
		break;
	}

	sf::Text skillDescription;
	skillDescription.setFont(font);
	skillDescription.setString(currentAreaSkills[CGameController::GetCurrentSkillNumber()].description);
	skillDescription.setCharacterSize(38);
	skillDescription.setPosition(m_skills.selectedSkillInfoArea.getPosition().x + 30, m_skills.selectedSkillInfoArea.getPosition().y + 30);

	window.draw(skillDescription);
}

SkillArea CSkillsRenderer::CreateSkillArea(const std::string& title, const sf::Vector2f& areaPos, const sf::Vector2f& skillAreaSize)
{
	const float skillAreaTopMargin = 200;
	sf::RectangleShape skillAreaBorder;
	sf::Text skillAreaTitle;

	skillAreaBorder.setSize(skillAreaSize);
	skillAreaBorder.setPosition(areaPos);
	skillAreaBorder.setFillColor(sf::Color::Black);
	skillAreaBorder.setOutlineThickness(4);
	skillAreaBorder.setOutlineColor(sf::Color::White);
	skillAreaTitle.setFont(font);
	skillAreaTitle.setString(title);
	skillAreaTitle.setCharacterSize(TextSize);
	skillAreaTitle.setPosition(skillAreaBorder.getPosition().x + skillAreaBorder.getSize().x / 3, skillAreaTopMargin - 70);

	return SkillArea(skillAreaBorder, skillAreaTitle);
}

void CSkillsRenderer::DrawHeroInfo(sf::RenderWindow& window)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto avatarComp = entityManager.GetComponent<AvatarComponent>(CGameController::GetSelectedEntityId());
	auto nameComp = entityManager.GetComponent<NameComponent>(CGameController::GetSelectedEntityId());
	auto experienceComp = entityManager.GetComponent<ExperienceComponent>(CGameController::GetSelectedEntityId());
	if (!avatarComp || !nameComp || !experienceComp)
	{
		return;
	}

	m_skills.heroInfo.heroInfoArea.setSize({ 350, 165 });
	m_skills.heroInfo.heroInfoArea.setPosition(100, float(window.getSize().y) - 245);
	m_skills.heroInfo.heroInfoArea.setFillColor(sf::Color::Black);
	m_skills.heroInfo.heroInfoArea.setOutlineThickness(3);
	m_skills.heroInfo.heroInfoArea.setOutlineColor(sf::Color::White);

	m_skills.heroInfo.heroIcon.setTexture(CTextureStorage::GetTexture(avatarComp->avatarFilePath));
	m_skills.heroInfo.heroIcon.setPosition(m_skills.heroInfo.heroInfoArea.getPosition().x + 30, m_skills.heroInfo.heroInfoArea.getPosition().y + 30);
	m_skills.heroInfo.heroIcon.setScale(3.f, 3.f);

	m_skills.heroInfo.heroName.setFont(font);
	m_skills.heroInfo.heroName.setString(nameComp->name);
	m_skills.heroInfo.heroName.setCharacterSize(30);
	m_skills.heroInfo.heroName.setPosition(m_skills.heroInfo.heroInfoArea.getPosition().x + 35, m_skills.heroInfo.heroInfoArea.getPosition().y + m_skills.heroInfo.heroInfoArea.getSize().y - 60);

	m_skills.heroInfo.heroLevel.setFont(font);
	m_skills.heroInfo.heroLevel.setString("LEVEL:  " + std::to_string(experienceComp->currentHeroLevel));
	m_skills.heroInfo.heroLevel.setCharacterSize(30);
	m_skills.heroInfo.heroLevel.setPosition(m_skills.heroInfo.heroInfoArea.getPosition().x + 150, m_skills.heroInfo.heroInfoArea.getPosition().y + 30);

	m_skills.heroInfo.heroAvailableSkillPoints.setFont(font);
	m_skills.heroInfo.heroAvailableSkillPoints.setString("SP:  " + std::to_string(experienceComp->availableSkillPoints));
	m_skills.heroInfo.heroAvailableSkillPoints.setCharacterSize(30);
	m_skills.heroInfo.heroAvailableSkillPoints.setPosition(m_skills.heroInfo.heroInfoArea.getPosition().x + 150, m_skills.heroInfo.heroInfoArea.getPosition().y + 80);

	window.draw(m_skills.heroInfo.heroInfoArea);
	window.draw(m_skills.heroInfo.heroIcon);
	window.draw(m_skills.heroInfo.heroName);
	window.draw(m_skills.heroInfo.heroLevel);
	window.draw(m_skills.heroInfo.heroAvailableSkillPoints);
}
