#include "CFightSystem.h"
#include "../../CGameController.h"
#include <iostream>

size_t CFightSystem::m_currentHeroIndex = 0;
sf::Clock CFightSystem::m_enemiesTurnClock;
bool CFightSystem::m_isEnemiesTurnActive = false;

void CFightSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::FightActionSelected, [](const SEvent& event) {
		CFightSystem::OnHeroActionSelected();
	});
}

void CFightSystem::Update()
{
	auto currentFightPhase = CGameController::GetCurrentFightPhase();

	if (currentFightPhase == FightPhase::EnemiesTurn && m_isEnemiesTurnActive)
	{
		if (m_enemiesTurnClock.getElapsedTime().asSeconds() >= 3.f)
		{
			m_isEnemiesTurnActive = false;
			EndEnemiesTurn();
		}
	}
}

void CFightSystem::StartHeroesTurn()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesInFight = entityManager.GetEntitiesWithComponents<FightTurnComponent>();

	for (auto entity : entitiesInFight)
	{
		auto* fightTurnComp = entityManager.GetComponent<FightTurnComponent>(entity);
		fightTurnComp->hasEntityActed = false;
	}
	m_currentHeroIndex = 0;
}

void CFightSystem::EndEnemiesTurn()
{
	CGameController::SetFightPhase(FightPhase::CharactersTurn);
	StartHeroesTurn();
}

void CFightSystem::StartEnemiesTurn()
{
	CGameController::SetFightPhase(FightPhase::EnemiesTurn);
	m_isEnemiesTurnActive = true;
	m_enemiesTurnClock.restart();
}

void CFightSystem::OnHeroActionSelected()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesInFight = entityManager.GetEntitiesWithComponents<FightTurnComponent>();
	size_t numHeroes = entitiesInFight.size();

	auto* currentHeroFightTurnComp = entityManager.GetComponent<FightTurnComponent>(entitiesInFight[m_currentHeroIndex]);
	if (currentHeroFightTurnComp)
	{
		currentHeroFightTurnComp->hasEntityActed = true;
	}

	do
	{
		m_currentHeroIndex = (m_currentHeroIndex + 1) % numHeroes;
		auto* healthComp = entityManager.GetComponent<HealthComponent>(entitiesInFight[m_currentHeroIndex]);
		if (healthComp->currentHealth > 0)
		{
			break;
		}
	} while (m_currentHeroIndex != 0);

	ProcessFightTurn();
}

void CFightSystem::ProcessFightTurn()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesInFight = entityManager.GetEntitiesWithComponents<FightTurnComponent>();
	bool allHeroesActed = true;

	for (size_t i = 0; i < entitiesInFight.size(); ++i)
	{
		auto entity = entitiesInFight[i];
		auto* fightTurnComp = entityManager.GetComponent<FightTurnComponent>(entity);
		auto* healthComp = entityManager.GetComponent<HealthComponent>(entity);

		if (healthComp->currentHealth > 0 && CGameController::GetCurrentFightPhase() == FightPhase::CharactersTurn)
		{
			fightTurnComp->isHeroTurn = (i == m_currentHeroIndex);
			if (fightTurnComp->isHeroTurn)
			{
				SEvent activeFightCharacterSelectedEvent;
				ActiveFightCharacterUpdatedEventData eventData{};

				eventData.id = entity;
				activeFightCharacterSelectedEvent.type = EventType::ActiveFightCharacterUpdated;
				activeFightCharacterSelectedEvent.data = eventData;
				CEventDispatcher::GetInstance().Dispatch(activeFightCharacterSelectedEvent);
			}
			if (!fightTurnComp->hasEntityActed)
			{
				allHeroesActed = false;
			}
		}
		else
		{
			fightTurnComp->isHeroTurn = false;
		}
	}

	if (allHeroesActed)
	{
		m_currentHeroIndex = -1;
		StartEnemiesTurn();
	}
}