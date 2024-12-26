#pragma once

#include "view/renderers/IRenderer.h"

struct Skills
{

};

class CSkillsRenderer : public IRenderer
{
public:
	void Init() override;
	void Draw(sf::RenderWindow& window) override;

private:
};
