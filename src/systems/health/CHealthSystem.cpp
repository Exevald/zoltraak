#include "CHealthSystem.h"
#include "events/CEventDispatcher.h"
#include <iostream>

void CHealthSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::AttackCollied, [](const SEvent& event) {
		const auto& attackColliedEventData = std::get<AttackColliedEventData>(event.data);
		CHealthSystem::OnAttackCollied(attackColliedEventData.damage, attackColliedEventData.parsonId);
	});
}

void CHealthSystem::Update()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto heroesHealth = entityManager.GetEntitiesWithComponents<HealthComponent>();

	for (int hero : heroesHealth)
	{
		auto healthComp = entityManager.GetComponent<HealthComponent>(hero);
		if (healthComp->currentHealth < 0)
		{
			healthComp->currentHealth = 0;
			return;
		}
	}
}

void CHealthSystem::OnAttackCollied(int damage, int personId)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto healthComp = entityManager.GetComponent<HealthComponent>(personId);
	if (!healthComp)
	{
		return;
	}

	healthComp->currentHealth -= float(damage);
}
