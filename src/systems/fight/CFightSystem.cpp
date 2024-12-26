#include "CFightSystem.h"
#include "../../CGameController.h"

size_t CFightSystem::m_currentHeroIndex = 0;
sf::Clock CFightSystem::m_enemiesTurnClock;
bool CFightSystem::m_isEnemiesTurnActive = false;
std::mt19937 CFightSystem::m_randomEngine{ std::random_device{}() };
std::vector<AttackData> CFightSystem::m_attacks;
sf::Clock CFightSystem::m_attackGenerationClock;
std::vector<std::function<void(std::vector<AttackData>&)>> CFightSystem::m_attackPatterns;

void CFightSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::FightActionSelected, [](const SEvent& event) {
		CFightSystem::OnHeroActionSelected();
	});
}

void CFightSystem::LoadAttackPatterns()
{
	static BattleAreaSettings battleAreaSettings;

	m_attackPatterns.emplace_back([](std::vector<AttackData>& attacks) {
		std::uniform_real_distribution<float> spawnXDist(30, float(battleAreaSettings.areaWidth));
		AttackData attack{};
		attack.x = spawnXDist(CFightSystem::m_randomEngine);
		if (attack.x < 10)
		{
			attack.x = 10;
		}

		attack.y = 0.f;
		attack.velocityX = 0.f;
		attack.velocityY = 85.f;
		attack.type = 0;
		attacks.push_back(attack);
	});

	m_attackPatterns.emplace_back([](std::vector<AttackData>& attacks) {
		std::uniform_real_distribution<float> spawnXDist(30, float(battleAreaSettings.areaWidth));
		AttackData attack1{ spawnXDist(CFightSystem::m_randomEngine), 0, 0.f, 85.f, 1 };
		AttackData attack2{ spawnXDist(CFightSystem::m_randomEngine), 0, 0.f, 85.f, 1 };
		attacks.push_back(attack1);
		attacks.push_back(attack2);
	});

	for (const auto& pattern : m_attackPatterns)
	{
		pattern(m_attacks);
	}
}

void CFightSystem::GenerateAttackPattern()
{
	if (m_attackPatterns.empty())
	{
		return;
	}

	std::uniform_int_distribution<int> patternDist(0, static_cast<int>(m_attackPatterns.size()) - 1);
	int patternIndex = patternDist(m_randomEngine);

	m_attackPatterns[patternIndex](m_attacks);
}

void CFightSystem::Update()
{
	auto currentFightPhase = CGameController::GetCurrentFightPhase();
	static BattleAreaSettings battleAreaSettings;
	static WindowSettings windowSettings;

	if (currentFightPhase == FightPhase::EnemiesTurn && m_isEnemiesTurnActive)
	{
		for (int i = 0; i < m_attacks.size(); i++)
		{
			auto& attack = m_attacks[i];
			attack.x += attack.velocityX * CGameController::GetDeltaTime();
			attack.y += attack.velocityY * CGameController::GetDeltaTime();
			CFightSystem::CheckFightCollision(sf::Vector2f(attack.x, attack.y), i);
		}

		m_attacks.erase(std::remove_if(m_attacks.begin(), m_attacks.end(),
							[](const AttackData& attack) {
								return attack.y > float(battleAreaSettings.areaHeight * 2 - 40);
							}),
			m_attacks.end());

		if (m_attackGenerationClock.getElapsedTime().asSeconds() >= 1.f)
		{
			CFightSystem::GenerateAttackPattern();
			m_attackGenerationClock.restart();
		}

		if (m_enemiesTurnClock.getElapsedTime().asSeconds() >= 10.f)
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

void CFightSystem::CheckFightCollision(const sf::Vector2f& attackPos, int attackIndex)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto fightSoul = entityManager.GetEntitiesWithComponents<FightSoulComponent>().front();
	auto fightSoulPositionComp = entityManager.GetComponent<PositionComponent>(fightSoul);
	auto fightSoulImageComp = entityManager.GetComponent<ImageComponent>(fightSoul);
	static WindowSettings windowSettings;

	if (!fightSoulPositionComp || !fightSoulImageComp)
	{
		return;
	}

	sf::FloatRect fightSoulBounds(
		fightSoulPositionComp->x,
		fightSoulPositionComp->y,
		28,
		28);

	sf::FloatRect attackBounds(
		float(windowSettings.windowWidth) / 2 + attackPos.x,
		float(windowSettings.windowHeight) / 2 + attackPos.y,
		28,
		28);

	if (attackBounds.intersects(fightSoulBounds))
	{
		std::uniform_real_distribution<float> randomPersonId(1, 2);
		SEvent attackColliedEvent;
		AttackColliedEventData eventData{};

		eventData.damage = 15;
		eventData.parsonId = int(randomPersonId(CFightSystem::m_randomEngine));
		attackColliedEvent.type = EventType::AttackCollied;
		attackColliedEvent.data = eventData;

		m_attacks.erase(m_attacks.begin() + attackIndex);
		CEventDispatcher::GetInstance().Dispatch(attackColliedEvent);
	}
}
