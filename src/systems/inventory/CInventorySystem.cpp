#include "CInventorySystem.h"
#include "../../CGameController.h"
#include <iostream>

void CInventorySystem::Init()
{
	CEventDispatcher::GetInstance().Subscribe(EventType::InventoryItemUsed, [](const SEvent& event) {
		const auto& inventoryItemUsedEventData = std::get<InventoryItemUsedEventData>(event.data);
		CInventorySystem::UseItemFromInventory(inventoryItemUsedEventData.inventoryItemNumber, inventoryItemUsedEventData.usageAction);
	});

	CEventDispatcher::GetInstance().Subscribe(EventType::InventoryItemEquipped, [](const SEvent& event) {
		const auto& inventoryItemUsedEventData = std::get<InventoryItemEquippedEventData>(event.data);
		CInventorySystem::EquipItem(inventoryItemUsedEventData.ownerId, inventoryItemUsedEventData.itemNumber, inventoryItemUsedEventData.type);
	});
}

void CInventorySystem::Update()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesWithInventory = entityManager.GetEntitiesWithComponents<InventoryComponent>();

	for (auto entity : entitiesWithInventory)
	{
		auto inventoryComp = entityManager.GetComponent<InventoryComponent>(entity);
		if (inventoryComp->commonItems.size() > inventoryComp->commonItemsInventorySize)
		{
			std::cout << "Inventory overdraft!" << std::endl;
			return;
		}
	}
}

void CInventorySystem::UseItemFromInventory(int itemIndex, const InventoryAction& actionType)
{
	auto& entityManager = CEntityManager::GetInstance();
	std::unordered_map<int, InventoryItem> allItemsMap;
	int index = 0;

	for (auto& [entityId, items] : CGameController::GetAllHeroesInventory())
	{
		for (const auto& item : items)
		{
			allItemsMap[index] = item;
			index++;
		}
	}
	auto it = allItemsMap.find(itemIndex);
	if (it == allItemsMap.end())
	{
		return;
	}

	InventoryItem usedItem = it->second;
	EntityId ownerId = usedItem.ownerId;

	if (actionType == InventoryAction::Use)
	{
		CInventorySystem::UseEffect(usedItem);
	}

	int localItemIndex = it->first;
	auto ownerInventoryComp = entityManager.GetComponent<InventoryComponent>(ownerId);
	if (!ownerInventoryComp)
	{
		return;
	}

	ownerInventoryComp->commonItems.erase(ownerInventoryComp->commonItems.begin() + localItemIndex);
	CGameController::RemoveItemFromHeroInventory(ownerId, localItemIndex);
}

void CInventorySystem::UseEffect(const InventoryItem& item)
{
	auto& entityManager = CEntityManager::GetInstance();

	switch (item.type)
	{
	case ItemType::HealingPotion: {
		auto healthComponent = entityManager.GetComponent<HealthComponent>(item.ownerId);
		if (!healthComponent)
		{
			break;
		}
		healthComponent->currentHealth += 25;
		if (healthComponent->currentHealth > healthComponent->maxHealth)
		{
			healthComponent->currentHealth = healthComponent->maxHealth;
		}
		break;
	}
	case ItemType::ManaPotion: {
		auto manaComponent = entityManager.GetComponent<ManaComponent>(item.ownerId);
		if (!manaComponent)
		{
			break;
		}
		manaComponent->currentMana += 25;
		if (manaComponent->currentMana > manaComponent->maxMana)
		{
			manaComponent->currentMana = manaComponent->maxMana;
		}
		break;
	}
	default:
		break;
	}
}

void CInventorySystem::EquipItem(int ownerId, int equippedItemNumber, const ItemType& itemType)
{
	auto& entityManager = CEntityManager::GetInstance();
	auto inventoryComp = entityManager.GetComponent<InventoryComponent>(ownerId);
	if (!inventoryComp)
	{
		return;
	}

	switch (itemType)
	{
	case ItemType::Weapon: {
		auto newWeapon = GetFilteredItem(inventoryComp->weapons, inventoryComp->activeWeapon, equippedItemNumber);
		if (newWeapon.has_value())
		{
			inventoryComp->activeWeapon = newWeapon.value();
		}
		break;
	}
	case ItemType::Shield: {
		auto newShield = CInventorySystem::GetFilteredItem<ShieldItem>(inventoryComp->shields, inventoryComp->activeShield, equippedItemNumber);
		if (newShield.has_value())
		{
			inventoryComp->activeShield = newShield.value();
		}
		break;
	}
	default:
		break;
	}
}

template <typename T>
std::optional<T> CInventorySystem::GetFilteredItem(const std::vector<T>& items, const T& activeItem, int index)
{
	std::vector<T> filteredItems;
	for (const auto& item : items)
	{
		if (item.name != activeItem.name)
		{
			filteredItems.push_back(item);
		}
	}

	if (index >= 0 && index < static_cast<int>(filteredItems.size()))
	{
		return filteredItems[index];
	}

	return std::nullopt;
}