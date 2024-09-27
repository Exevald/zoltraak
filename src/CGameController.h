#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "systems/MovementSystem.h"

class CGameController
{
public:
	explicit CGameController(sf::RenderWindow& window)
			: m_window(window), m_selectedEntity(nullptr)
	{
	}

	void AddEntity(Entity* entity);

	void Update();

	void Draw();

	void handleClick(const sf::Vector2f& mousePos);

	[[nodiscard]] Entity* GetSelectedEntity() const;

private:
	Entity* m_selectedEntity;
	sf::RenderWindow& m_window;
	std::vector<Entity*> m_entities;
};
