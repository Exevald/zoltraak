#pragma once

#include "../../common/Utils.h"
#include "actions/Actions.h"
#include "inventory/item_factory/CInventoryItemFactory.h"
#include <optional>
#include <vector>

class CInventorySystem
{
public:
	explicit CInventorySystem(const CInventoryItemFactory& inventoryItemFactory)
		: m_inventoryItemFactory(inventoryItemFactory)
	{
	}

	void Init();
	static void Update();

private:
	static void UseItemFromInventory(int itemIndex, const InventoryAction& actionType);
	static void UseEffect(const InventoryItem& item);
	static void EquipItem(int ownerId, int equippedItemNumber, const ItemType& itemType);
	static void BuyItem(int boughtItemNumber, const CInventoryItemFactory& factory);
	static void SellItem(int soldItemNumber);
	static void UseFightItem(int ownerId, int itemIndex);

	template <typename T>
	static std::optional<T> GetFilteredItem(const std::vector<T>& items, const T& activeItem, int index);

	const CInventoryItemFactory& m_inventoryItemFactory;
};