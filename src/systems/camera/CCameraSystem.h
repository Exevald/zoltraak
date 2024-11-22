#pragma once

#include "CEntityManager.h"
#include <SFML/Graphics.hpp>

class CCameraSystem
{
public:
	explicit CCameraSystem(sf::RenderWindow& window)
		: m_window(window)
	{
	}

	void Init();

private:
	void UpdateCameraPosition(EntityId movedEntityId);

	sf::RenderWindow& m_window;
};