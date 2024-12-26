#pragma once

#include "CEntityManager.h"
#include <functional>
#include <iostream>
#include <random>

struct AttackData
{
	float x, y;
	float velocityX;
	float velocityY;
	int type;
};

class CFightSystem
{
public:
	static void Init();
	static void Update();
	static void OnHeroActionSelected();
	static void LoadAttackPatterns();
	static void GenerateAttackPattern();

	static const std::vector<AttackData>& GetAttacks() { return m_attacks; }

private:
	static void ProcessFightTurn();
	static void StartHeroesTurn();
	static void EndEnemiesTurn();
	static void StartEnemiesTurn();
	static void CheckFightCollision(const sf::Vector2f& attackPos, int attackIndex);

	static size_t m_currentHeroIndex;
	static sf::Clock m_enemiesTurnClock;
	static sf::Clock m_attackGenerationClock;
	static bool m_isEnemiesTurnActive;
	static std::vector<AttackData> m_attacks;
	static std::vector<std::function<void(std::vector<AttackData>&)>> m_attackPatterns;
	static std::mt19937 m_randomEngine;
};
