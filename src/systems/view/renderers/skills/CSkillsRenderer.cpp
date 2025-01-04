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
		for (const auto& skill : skillsInfo.second)
		{
			window.draw(skill);
		}
	}
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