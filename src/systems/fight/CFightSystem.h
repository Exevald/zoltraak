#pragma once

#include "CEntityManager.h"
class CFightSystem
{
public:
	static void Init();
	static void Update();
	static void OnHeroActionSelected();

private:
	static void ProcessFightTurn();
	static void StartHeroesTurn();
	static void EndEnemiesTurn();
	static void StartEnemiesTurn();

	static size_t m_currentHeroIndex;
	static sf::Clock m_enemiesTurnClock;
	static bool m_isEnemiesTurnActive;
};
