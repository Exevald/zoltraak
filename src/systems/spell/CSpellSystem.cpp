#include "CSpellSystem.h"
#include "events/CEventDispatcher.h"

void CSpellSystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::FightSpellUsed, [](const SEvent& event) {
		const auto& fightSpellUsedEventData = std::get<FightSpellUsedEventData>(event.data);
		CSpellSystem::UseFightSpell(fightSpellUsedEventData.heroId, fightSpellUsedEventData.spellIndex);
	});
}

void CSpellSystem::UseFightSpell(int ownerId, int spellIndex)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto spellComp = entityManager.GetComponent<SpellComponent>(ownerId);
	auto manaComp = entityManager.GetComponent<ManaComponent>(ownerId);
	auto animComp = entityManager.GetComponent<AnimationComponent>(ownerId);
	if (!spellComp || !manaComp || !animComp)
	{
		return;
	}

	auto usedSpell = spellComp->spells[spellIndex];
	CSpellSystem::UseSpellEffect(usedSpell);
	manaComp->currentMana -= float(usedSpell.manaCost);
	animComp->SetAnimation("magic");
}

void CSpellSystem::UseSpellEffect(const Spell& spell)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto manaComp = entityManager.GetComponent<ManaComponent>(spell.ownerId);
	auto healthComp = entityManager.GetComponent<HealthComponent>(spell.ownerId);
	auto enemy = entityManager.GetEntitiesWithComponents<EnemyComponent>().front();
	auto enemyHealthComp = entityManager.GetComponent<HealthComponent>(enemy);

	if (!manaComp || !healthComp)
	{
		return;
	}

	switch (spell.type)
	{
	case SpellType::DamageSpell: {
		enemyHealthComp->currentHealth -= 25;
		break;
	}
	case SpellType::HealSpell: {
		healthComp->currentHealth += 25;
		break;
	}
	case SpellType::BuffSpell: {
		break;
	}
	}
}
