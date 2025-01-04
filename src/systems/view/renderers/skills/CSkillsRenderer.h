#pragma once

#include "view/renderers/IRenderer.h"
#include <unordered_map>

struct SkillArea
{
	sf::RectangleShape skillAreaBorder;
	sf::Text title;
};

struct Skills
{
	std::vector<SkillArea> skillsAreas;
	sf::Sprite selectedSkillInfoArea;
	sf::Text selectedSkillInfoText;
	std::unordered_map<int, std::vector<sf::Text>> allSkillsInfo;
};

class CSkillsRenderer : public IRenderer
{
public:
	explicit CSkillsRenderer(Skills& skills)
		: m_skills(skills)
	{
	}

	void Init() override;
	void Draw(sf::RenderWindow& window) override;

private:
	void DrawSkillsAreas(sf::RenderWindow& window);
	void DrawCurrentSkillInfo(sf::RenderWindow& window);
	void DrawSkills(sf::RenderWindow& window);
	void DrawSkillDescription(sf::RenderWindow& window);

	static SkillArea CreateSkillArea(const std::string& title, const sf::Vector2f& areaPos, const sf::Vector2f& skillAreaSize);

	Skills& m_skills;
};
