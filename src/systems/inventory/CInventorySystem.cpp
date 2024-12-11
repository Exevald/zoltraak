#include "CInventorySystem.h"
#include "../../common/CEntityManager.h"
#include <iostream>

void CInventorySystem::Update()
{
	auto& entityManager = CEntityManager::GetInstance();
	auto entitiesWithInventory = entityManager.GetEntitiesWithComponents<InventoryComponent>();

	for (auto entity : entitiesWithInventory)
	{
		auto inventoryComp = entityManager.GetComponent<InventoryComponent>(entity);
		if (inventoryComp->items.size() > inventoryComp->size)
		{
			std::cout << "Inventory overdraft!" << std::endl;
			return;
		}
	}
}
