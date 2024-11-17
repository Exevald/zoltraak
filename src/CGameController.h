#pragma once

#include "CEntityManager.h"
#include "movement/CMovementSystem.h"
#include <SFML/Graphics.hpp>
#include <vector>

class CGameController
{
public:
	static void Init();

	static void Draw(sf::RenderWindow& window);

	void SetSelectedEntityId(EntityId id);

	[[nodiscard]] EntityId GetSelectedEntityId() const;

private:
	EntityId m_selectedEntityId{};
};
