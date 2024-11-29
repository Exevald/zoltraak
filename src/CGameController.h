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
	static void InitGameSettings(const Level& level);

	static void InitSystems();

	static void Update(float deltaTime);

	static void Draw(sf::RenderWindow& window, Level& level);

	void SetSelectedEntityId(EntityId id);

	[[nodiscard]] EntityId GetSelectedEntityId() const;

	static sf::Vector2f GetWindowSizeSettings();

private:
	EntityId m_selectedEntityId{};
	static float m_levelWidth;
	static float m_levelHeight;
};
