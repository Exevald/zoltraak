#pragma once

#include "CEntityManager.h"
#include "camera/CCameraSystem.h"
#include "collision/CCollisionSystem.h"
#include "movement/CMovementSystem.h"
#include "view/CViewSystem.h"
#include <SFML/Graphics.hpp>
#include <vector>

class CGameController
{
public:
	static void InitSystems();

	static void Draw(sf::RenderWindow& window);

	void SetSelectedEntityId(EntityId id);

	[[nodiscard]] EntityId GetSelectedEntityId() const;

private:
	EntityId m_selectedEntityId{};
};
