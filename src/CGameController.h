#pragma once

#include "Entity.h"
#include "movement/CMovementSystem.h"
#include <SFML/Graphics.hpp>
#include <vector>

class CGameController
{
public:
	explicit CGameController(sf::RenderWindow& window)
		: m_window(window)
		, m_selectedEntity(nullptr)
	{
	}

	void AddEntity(Entity* entity);

	static void Init();

	void Draw();

	void SetSelectedEntity(Entity* entity);

	std::vector<Entity*> GetEntitiesWithShapes();

	std::vector<Entity*> GetEntitiesWithMovement();

	[[nodiscard]] Entity* GetSelectedEntity() const;

private:
	Entity* m_selectedEntity;
	sf::RenderWindow& m_window;
	std::vector<Entity*> m_entities;
};
